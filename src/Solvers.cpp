#include <iostream>
#include <vector>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include "Solvers.h"
#include "DistributionFunction.h"
#include "VectorField.h"
#include <functional>
#include "SimulationTime.h"

void poisson(const Mesh2D& mesh, const ScalarField& density, ScalarField& voltage) {
	const double epsilon = 1;
	const int Nx   = mesh.get_Nx();
	const int Ny   = mesh.get_Ny();
	const int Nx2  = Nx + 2;
	const int Ny2  = Ny + 2;
	const int Ntot = Nx2 * Ny2;
	const double dx = mesh.get_dx();
	const double dy = mesh.get_dy();

	std::vector<Eigen::Triplet<double>> triplets;
	int row;

	// Corner ghost cells
	triplets.emplace_back(0, 0, 1.0);
	triplets.emplace_back(Nx2 - 1, Nx2 - 1, 1.0);
	triplets.emplace_back((Ny2-1)*Nx2, (Ny2-1)*Nx2, 1.0);
	triplets.emplace_back(Ny2*Nx2 - 1, Ny2*Nx2 - 1, 1.0);

	// South boundary
	for (int i = 1; i <= Nx; ++i) {
		row = i;
		triplets.emplace_back(row, row,       0.5);
		triplets.emplace_back(row, row + Nx2, 0.5);
	}

	// North boundary
	for (int i = 1; i <= Nx; ++i) {
		row = (Ny + 1) * Nx2 + i;
		triplets.emplace_back(row, row,       0.5);
		triplets.emplace_back(row, row - Nx2, 0.5);
	}

	// West boundary
	for (int j = 1; j <= Ny; ++j) {
		row = j * Nx2;
		triplets.emplace_back(row, row,     0.5);
		triplets.emplace_back(row, row + 1, 0.5);
	}

	// East boundary
	for (int j = 1; j <= Ny; ++j) {
		row = j * Nx2 + (Nx + 1);
		triplets.emplace_back(row, row,     0.5);
		triplets.emplace_back(row, row - 1, 0.5);
	}

	// 5 pt stencil 
	const double ae = 1.0 / (dx * dx);
	const double aw = 1.0 / (dx * dx);
	const double an = 1.0 / (dy * dy);
	const double as = 1.0 / (dy * dy);
	const double ap = -(ae + aw + an + as);

	// Fill interior
	for (int j = 1; j <= Ny; ++j) {
		for (int i = 1; i <= Nx; ++i) {
			row = j * Nx2 + i;

			triplets.emplace_back(row, row,          ap);
			triplets.emplace_back(row, row + 1,      ae);
			triplets.emplace_back(row, row - 1,      aw);
			triplets.emplace_back(row, row + Nx2,    an);
			triplets.emplace_back(row, row - Nx2,    as);
		}
	}

	Eigen::SparseMatrix<double> A(Ntot, Ntot);
	A.setFromTriplets(triplets.begin(), triplets.end());

	// Build RHS
	Eigen::VectorXd b(Ntot);
	b.setZero();

    // Set RHS to density / epsilon
	for (int j = 1; j <= Ny; ++j) {
		for (int i = 1; i <= Nx; ++i) {
			b(j * Nx2 + i) = density.get(i, j) / epsilon;
		}
	}

	// Solve
	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
	solver.compute(A);
	if (solver.info() != Eigen::Success) {
		std::cerr << "Error: Poisson matrix factorization failed." << std::endl;
		return;
	}

	const Eigen::VectorXd x = solver.solve(b);
	if (solver.info() != Eigen::Success) {
		std::cerr << "Error: Poisson linear solve failed." << std::endl;
		return;
	}

    // Record solution into voltage field
	for (int j = 0; j <= Ny + 1; ++j) {
		for (int i = 0; i <= Nx + 1; ++i) {
			voltage.set(i, j, x(j * Nx2 + i));
		}
	}
}

void iterative_implicit_solver(
    const Mesh2D& mesh,
    const DistributionFunction& f_old,
    DistributionFunction& f_new,
    const VectorField& E,
    double t,
    double dt,
    double qm,
    std::function<double(double, double, double, double, double, double, double)> g
) {

    int Nx2 = mesh.get_Nx() + 2;
    int Ny2 = mesh.get_Ny() + 2;
    int Nvx2 = mesh.get_Nvx() + 2;
    int Nvy2 = mesh.get_Nvy() + 2;

    double dx = mesh.get_dx();
    double dy = mesh.get_dy();
    double dvx = mesh.get_dvx();
    double dvy = mesh.get_dvy();
    double Lvx = mesh.get_vxmax();
    double Lvy = mesh.get_vymax();

    // Initial guess is just the previous timestep
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    f_new.set(i, j, ivx, ivy, f_old.get(i, j, ivx, ivy));
                }
            }
        }
    }

    // Paramters for iterations 
    int max_iter = 200;
    double tol = 1.0e-10;

    for (int iter = 0; iter < max_iter; ++iter) {
        double max_change = 0.0; // Checks if greater than tolerance at the end

        for (int i = 1; i < Nx2 - 1; ++i) {
            for (int j = 1; j < Ny2 - 1; ++j) {

                double Ex = E.get_x(i, j); 
                double Ey = E.get_y(i, j);

                double ax = qm * Ex; // coefficient for partial of f with respect to velocity
                double ay = qm * Ey;

                for (int ivx = 1; ivx < Nvx2 - 1; ++ivx) {
                    for (int ivy = 1; ivy < Nvy2 - 1; ++ivy) {
                        double x = mesh.get_x(i);
                        double y = mesh.get_y(j);
                        double vx = mesh.get_vx(ivx);
                        double vy = mesh.get_vy(ivy);

                        // Compute CFL (transport) coefficients 
                        double Cx  = dt * std::abs(vx) / dx; 
                        double Cy  = dt * std::abs(vy) / dy; 
                        double Cvx = dt * std::abs(ax) / dvx;
                        double Cvy = dt * std::abs(ay) / dvy;

                        // determines direction of upwinding stencil
                        int i_up;
                        if (vx > 0.0) {
                            i_up = i - 1;
                        } else {
                            i_up = i + 1;
                        }

                        int j_up;
                        if (vy > 0.0) {
                            j_up = j - 1;
                        } else {
                            j_up = j + 1;
                        }

                        int ivx_up;
                        if (ax > 0.0) {
                            ivx_up = ivx - 1;
                        } else {
                            ivx_up = ivx + 1;
                        }

                        int ivy_up;
                        if (ay > 0.0) {
                            ivy_up = ivy - 1;
                        } else {
                            ivy_up = ivy + 1;
                        }

                        double old_value = f_new.get(i, j, ivx, ivy);

                        // Algebraic form after discretization ( known terms + upwind neighbor contributions ) / (1 + transport coefficients)
                        double numerator =
                            f_old.get(i, j, ivx, ivy)
                          + Cx  * f_new.get(i_up, j, ivx, ivy)
                          + Cy  * f_new.get(i, j_up, ivx, ivy)
                          + Cvx * f_new.get(i, j, ivx_up, ivy)
                          + Cvy * f_new.get(i, j, ivx, ivy_up)
                          - dt * g(x, y, vx, vy, t, Lvx, Lvy);

                        double denominator =
                            1.0 + Cx + Cy + Cvx + Cvy;

                        double new_value = numerator / denominator;

                        f_new.set(i, j, ivx, ivy, new_value);

                        // computes change to show if answer is converging 
                        double change = std::abs(new_value - old_value);
                        if (change > max_change) {
                            max_change = change;
                        }
                    }
                }
            }
        }

        if (max_change < tol) {
            break;
        }
    }
}
