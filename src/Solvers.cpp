#include <iostream>
#include <vector>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include "Solvers.h"

void poisson(const Mesh& mesh, const ScalarField& density, ScalarField& voltage) {
	const double epsilon = 8.85e-12;
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
