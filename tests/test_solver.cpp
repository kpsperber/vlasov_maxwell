#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cassert>

#include "../src/Mesh2D.h"
#include "../src/DistributionFunction.h"
#include "../src/VectorField.h"
#include "../src/Solvers.h"
#include "../src/Operators.h"



/** TEST 1: E=0
 * Tests the iterative_implicit_solver for indexing errors, logic issues, or other bad iterative behavior
 * 
 * The exact solution is f^{n+1} = f^{n}. 
 * 
 */
void test_1(const Mesh2D& mesh) {

    // Create distribution functions
    DistributionFunction f_old(mesh);
    DistributionFunction f_new(mesh);

    // Create zero electric field
    VectorField E("E", mesh);

    int Nx2  = mesh.get_Nx()  + 2;
    int Ny2  = mesh.get_Ny()  + 2;
    int Nvx2 = mesh.get_Nvx() + 2;
    int Nvy2 = mesh.get_Nvy() + 2;

    // Initialize velocity field and zero electric field
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    // Velocity field
                    double x = mesh.get_x(i);
                    double y = mesh.get_y(j);
                    double vx = mesh.get_vx(ivx);
                    double vy = mesh.get_vy(ivy);

                    double value = std::sin(0.1 * x) + std::cos(0.2 * y) + 0.01 * vx - 0.02 * vy;
                    f_old.set(i, j, ivx, ivy, value);

                    // Zero electric field
                    E.set_x(i, j, 0.0);
                    E.set_y(i, j, 0.0);
                }
            }
        }
    }

    // Advance one time step
    iterative_implicit_solver(mesh, f_old, f_new, E, 0.001, 1.0); 

    // Compare solutions
    double max_error = 0.0;

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {

                    double error =
                        std::abs(
                            f_new.get(i,j,ivx,ivy)
                          - f_old.get(i,j,ivx,ivy)
                        );

                    if (error > max_error) {
                        max_error = error;
                    }
                }
            }
        }
    }

    std::cout << "Zero Dynamics Test\n";
    std::cout << "Max Error = " << max_error << "\n";

    // Tight tolerance because exact solution should not evolve
    assert(max_error < 1.0e-4);

    std::cout << "PASSED\n";
}

void test_2(const Mesh2D& mesh) {

    // Create distribution functions
    DistributionFunction f_old(mesh);
    DistributionFunction f_new(mesh);

    // Create zero electric field
    VectorField E("E", mesh);
    ScalarField rho(mesh, "rho");

    int Nx2  = mesh.get_Nx()  + 2;
    int Ny2  = mesh.get_Ny()  + 2;
    int Nvx2 = mesh.get_Nvx() + 2;
    int Nvy2 = mesh.get_Nvy() + 2;
    double Lvx = mesh.get_vxmax();
    double Lvy = mesh.get_vymax();

    // Initialize velocity field and zero electric field
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    // Velocity field
                    double x = mesh.get_x(i);
                    double y = mesh.get_y(j);
                    double vx = mesh.get_vx(ivx);
                    double vy = mesh.get_vy(ivy);

                    double value = std::sin(x) * std::sin(y) / Lvx / Lvy;
                    f_old.set(i, j, ivx, ivy, value);

                    // Zero electric field
                    E.set_x(i, j, 0.0);
                    E.set_y(i, j, 0.0);
                }
            }
        }
    }

    // Advance one time step
    integrate(mesh, f_old, rho);
    iterative_implicit_solver(mesh, f_old, f_new, E, 0.00000000000000001, 1.0);

    // Compare solutions
    double max_error = 0.0;

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    double x = mesh.get_x(i);
                    double y = mesh.get_y(j);
                    double vx = mesh.get_vx(ivx);
                    double vy = mesh.get_vy(ivy);
                    

                    double error =
                        std::abs(
                            f_new.get(i,j,ivx,ivy)
                          - (sin(x) * sin(y) / Lvx / Lvy)
                        );

                    if (error > max_error) {
                        max_error = error;
                    }
                }
            }
        }
    }

    std::cout << "Zero Dynamics Test\n";
    std::cout << "Max Error = " << max_error << "\n";

    // Tight tolerance because exact solution should not evolve
    assert(max_error < 1.0e-4);

    std::cout << "PASSED\n";
}

int main() {
    // determine 
    Mesh2D mesh("../input/mesh_grid.csv");
    test_2(mesh);
}
