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
 * Tests the iterative_implicit_solver using a manufactured solution 
 * 
 * Solutions should converge to , t = n*dt
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


static void copy_distribution(const Mesh2D& mesh,const DistributionFunction& src,DistributionFunction& dst) {
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



static double run_case(int N_space, int N_vel, int n_steps) {
    
}

int main() {
    const std::vector<int> sizes = {8, 16, 32, 64};
    const int n_steps = 1;

    std::vector<double> errors;
    for (int N : sizes) {
        errors.push_back(run_case(N, N, n_steps));
    }

    const double pi = std::acos(-1.0);


}
