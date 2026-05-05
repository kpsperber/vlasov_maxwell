#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include "../src/Mesh2D.h"
#include "../src/ScalarField.h"
#include "../src/Solvers.h"

/**
 * Manufactured solution convergence test for the Poisson solver.
 * 
 * Solutions should converge to v(x,y) = sin(x) sin(y)
 * with second-order accuracy
 */
static std::string write_poisson_mesh_csv(int N, double L) {
    const std::string path = "mesh_poisson_convergence_tmp.csv";
    std::ofstream out(path);
    out << "Nx," << N << "\n";
    out << "Ny," << N << "\n";
    out << "Lx," << L << "\n";
    out << "Ly," << L << "\n";
    out << "Nvx,1\n";
    out << "Nvy,1\n";
    out << "Lvx,1\n";
    out << "Lvy,1\n";
    out.close();
    return path;
}

static double run(int N) {
    const double epsilon = 8.85e-12;
    const double L = 3.14159265358979;

    const std::string mesh_path = write_poisson_mesh_csv(N, L);
    Mesh2D mesh(mesh_path);

    const int Nx = mesh.get_Nx();
    const int Ny = mesh.get_Ny();
    ScalarField density(mesh, "density");
    ScalarField voltage(mesh, "voltage");

    for (int j = 1; j <= Ny; ++j) {
        for (int i = 1; i <= Nx; ++i) {
            const double x = mesh.get_x(i);
            const double y = mesh.get_y(j);
            density.set(i, j, epsilon * (-2.0) * std::sin(x) * std::sin(y));
        }
    }

    poisson(mesh, density, voltage);

    double linf_error = 0.0;
    for (int j = 1; j <= Ny; ++j) {
        for (int i = 1; i <= Nx; ++i) {
            const double x = mesh.get_x(i);
            const double y = mesh.get_y(j);
            const double diff = voltage.get(i, j) - std::sin(x) * std::sin(y);
            linf_error = std::max(linf_error, std::fabs(diff));
        }
    }
    return linf_error;
}

int main() {
    const std::vector<int> sizes = {8, 16, 32, 64, 128};
    std::vector<double> errors;

    for (int N : sizes) {
        errors.push_back(run(N));
    }

    const double pi = std::acos(-1.0);

    std::cout << "\n";
    std::cout << std::string(56, '=') << "\n";
    std::cout << "  Poisson Convergence (manufactured solution)\n";
    std::cout << std::string(56, '=') << "\n";
    std::cout << std::setw(6)  << "N"
              << std::setw(12) << "dx"
              << std::setw(16) << "Linf error"
              << std::setw(10) << "rate" << "\n";
    std::cout << std::string(56, '-') << "\n";

    for (std::size_t k = 0; k < sizes.size(); ++k) {
        const double dx = pi / sizes[k];
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
    std::cout << std::string(56, '=') << "\n";

    const double rate = std::log2(errors[errors.size() - 2] / errors.back());
    const bool   pass = rate > 1.8;
    std::cout << (pass ? "PASS" : "FAIL")
              << "  (convergence rate = "
              << std::fixed << std::setprecision(2) << rate << ", expected ~2.0)\n";

    const std::string mesh_path = "mesh_poisson_convergence_tmp.csv";
    std::remove(mesh_path.c_str());
    return pass ? 0 : 1;
}
