#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>

#include "../src/Mesh2D.h"
#include "../src/DistributionFunction.h"
#include "../src/VectorField.h"
#include "../src/Solvers.h"

/**
 * Tests the backward Euler using a manufactured solution in the E = 0 case.
 * 
 * Solutions should converge to sin(x-vx*t)sin(y-vy*t)sin(vx)sin(vy), t = n*dt
 * with 1st order accuracy
 */

static std::string write_be_mesh_csv(int N_space, int N_vel, double L) {
    const std::string path = "mesh_backward_euler_tmp.csv";
    std::ofstream out(path);
    out << "Nx," << N_space << "\n";
    out << "Ny," << N_space << "\n";
    out << "Lx," << L << "\n";
    out << "Ly," << L << "\n";
    out << "Nvx," << N_vel << "\n";
    out << "Nvy," << N_vel << "\n";
    out << "Lvx," << L << "\n";
    out << "Lvy," << L << "\n";
    out.close();
    return path;
}

static void fill_initial_sine(Mesh2D& mesh, DistributionFunction& f) {
    const int Ny   = mesh.get_Ny();
    const int Nvx  = mesh.get_Nvx();
    const int Nvy  = mesh.get_Nvy();
    const int Nx2  = mesh.get_Nx2();
    const int Ny2  = mesh.get_Ny2();
    const int Nvx2 = mesh.get_Nvx2();
    const int Nvy2 = mesh.get_Nvy2();

    for (int j = 0; j < Ny2; ++j) {
        for (int i = 0; i < Nx2; ++i) {
            for (int l = 0; l < Nvy2; ++l) {
                for (int k = 0; k < Nvx2; ++k) {
                    f.set(i, j, k, l, 0.0);
                }
            }
        }
    }

    const int Nx = mesh.get_Nx();
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            const double x  = mesh.get_x(i);
            const double y  = mesh.get_y(j);
            for (int ivy = 0; ivy < Nvy; ++ivy) {
                for (int ivx = 0; ivx < Nvx; ++ivx) {
                    const double vx = mesh.get_vx(ivx);
                    const double vy = mesh.get_vy(ivy);
                    const double val =
                        std::sin(x) * std::sin(y) * std::sin(vx) * std::sin(vy);
                    f.set(i, j, ivx, ivy, val);
                }
            }
        }
    }
}

static void copy_distribution(
    const Mesh2D& mesh,
    const DistributionFunction& src,
    DistributionFunction& dst
) {
    const int Nx2 = mesh.get_Nx2();
    const int Ny2 = mesh.get_Ny2();
    const int Nvx2 = mesh.get_Nvx2();
    const int Nvy2 = mesh.get_Nvy2();
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    dst.set(i, j, ivx, ivy, src.get(i, j, ivx, ivy));
                }
            }
        }
    }
}

static double linf_vs_characteristics(
    const Mesh2D& mesh,
    const DistributionFunction& f,
    double t_adv
) {
    const int Nx  = mesh.get_Nx();
    const int Ny  = mesh.get_Ny();
    const int Nvx = mesh.get_Nvx();
    const int Nvy = mesh.get_Nvy();

    double err = 0.0;
    for (int j = 0; j < Ny; ++j) {
        for (int i = 0; i < Nx; ++i) {
            const double x = mesh.get_x(i);
            const double y = mesh.get_y(j);
            for (int ivy = 0; ivy < Nvy; ++ivy) {
                for (int ivx = 0; ivx < Nvx; ++ivx) {
                    const double vx = mesh.get_vx(ivx);
                    const double vy = mesh.get_vy(ivy);
                    const double ref =
                        std::sin(x - vx * t_adv) * std::sin(y - vy * t_adv)
                        * std::sin(vx) * std::sin(vy);
                    const double num = f.get(i, j, ivx, ivy);
                    err = std::max(err, std::fabs(num - ref));
                }
            }
        }
    }
    return err;
}

static double run_case(int N_space, int N_vel, int n_steps) {
    const double L = 2.0 * std::acos(-1.0);
    const std::string mesh_path = write_be_mesh_csv(N_space, N_vel, L);
    Mesh2D mesh(mesh_path);

    DistributionFunction f(mesh, "f");
    DistributionFunction f_new(mesh, "f_new");
    VectorField E("E", mesh);
    const double qm = 1.0;

    const double dx    = mesh.get_dx();
    const double vmax  = L;
    const double dt    = 0.15 * dx / vmax;
    const double t_tot = dt * static_cast<double>(n_steps);

    fill_initial_sine(mesh, f);

    for (int n = 0; n < n_steps; ++n) {
        backward_euler(mesh, f, f_new, E, dt, qm);
        copy_distribution(mesh, f_new, f);
    }

    const double err = linf_vs_characteristics(mesh, f, t_tot);
    std::remove(mesh_path.c_str());
    return err;
}

int main() {
    const std::vector<int> sizes = {8, 16, 32, 64};
    const int n_steps = 1;

    std::vector<double> errors;
    for (int N : sizes) {
        errors.push_back(run_case(N, N, n_steps));
    }

    const double pi = std::acos(-1.0);

    std::cout << "\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "  Backward Euler (E=0):\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "  Reference: sin(x-vx*t)sin(y-vy*t)sin(vx)sin(vy), t = n*dt\n";
    std::cout << "  Steps n = " << n_steps << "\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::setw(6)  << "N"
              << std::setw(12) << "dx"
              << std::setw(16) << "Linf error"
              << std::setw(10) << "rate" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (std::size_t k = 0; k < sizes.size(); ++k) {
        const double dx = pi * 2.0 / sizes[k];
        std::cout << std::setw(6)  << sizes[k]
                  << std::setw(12) << std::fixed      << std::setprecision(5) << dx
                  << std::setw(16) << std::scientific << std::setprecision(4) << errors[k];
        if (k > 0) {
            const double rate = std::log2(errors[k - 1] / errors[k]);
            std::cout << std::setw(10) << std::fixed << std::setprecision(2) << rate;
        } else {
            std::cout << std::setw(10) << "--";
        }
        std::cout << "\n";
    }
    std::cout << std::string(60, '=') << "\n";

    const double rate = std::log2(errors[errors.size() - 2] / errors.back());
    // Implicit upwind backward Euler here is dominated by ~first-order truncation;
    // require clear mesh refinement convergence (near first order).
    const bool pass = rate > 0.85 && errors.front() > errors.back();
    std::cout << (pass ? "PASS" : "FAIL")
              << "  (convergence rate = "
              << std::fixed << std::setprecision(2) << rate << ", expected >= ~1.0)\n";

    const double err_multi = run_case(32, 32, 10);
    std::cout << "  Multi-step diagnostic (N=32, n=10): Linf error = "
              << std::scientific << std::setprecision(4) << err_multi << "\n";

    return pass ? 0 : 1;
}
