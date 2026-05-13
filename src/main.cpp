#include <iostream>
#include <cmath>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "SimulationTime.h"
#include "DistributionFunction.h"
#include "Solvers.h"
#include "Operators.h"

double laser(double x, double y, double t, double Lx, double Ly) {
    double val = 10 * std::exp(-(std::pow(x - Lx / 2, 2) + std::pow(y - Ly / 2, 2)) / (0.5));

    return val;
}

double g(double x, double y, double vx, double vy, double t, double Lvx, double Lvy) {
    double val =
        -std::sin(x) * std::sin(y) * std::sin(t) / Lvx / Lvy
        + vx * std::cos(x) * std::sin(y) * std::cos(t) / Lvx / Lvy
        + vy * std::sin(x) * std::cos(y) * std::cos(t) / Lvx / Lvy;

    return -0.0 * val;
}

int main() {
    // - - - - - - - - - preprocessing - - - - - - - - - -//

    // initialize runTime
    SimulationTime runTime("input/time_grid.csv");

    // Create Mesh
    Mesh2D mesh("input/mesh_grid.csv");
    int Nx2 = mesh.get_Nx2();
    int Ny2 = mesh.get_Ny2();
    int Nvx2 = mesh.get_Nvx2();
    int Nvy2 = mesh.get_Nvy2();
    double Lx = mesh.get_xmax();
    double Ly = mesh.get_ymax();

    // Declare electric field
    VectorField E_charge("E_Charge", mesh);
    VectorField E_laser("E_Laser", mesh);
    VectorField E_total("E_Total", mesh);

    // Create charge density field
    ScalarField rho(mesh, "rho");

    // Declare voltage field
    ScalarField V(mesh, "V");

    // Define initial distribution function
    DistributionFunction f_old(mesh, "f_old");
    DistributionFunction f_new(mesh,"f_new");

    double sigma = 2;

    // Set global parameters
    double dt = runTime.get_dt();
    double qm = -1.0;

    // Initial Conditions
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            double x = mesh.get_x(i);
            double y = mesh.get_y(j);

            E_charge.set_x(i, j, 0.0);
            E_charge.set_y(i, j, 0.0);

            E_laser.set_x(i, j, laser(x, y, 0.0, Lx, Ly));
            E_laser.set_y(i, j, laser(x, y, 0.0, Lx, Ly) * 0.0);

            rho.set(i, j, 0.0);
        }
    }

    E_total = E_charge + E_laser;

    // Initialize distribution
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    double vx = mesh.get_vx(ivx);
                    double vy = mesh.get_vy(ivy);

                    double value = std::exp(-0.5 * (vx * vx) / sigma - 0.5 * vy * vy / sigma) / (2.0 * 3.14159265) / 2; //std::sin(x) * std::sin(y) / Lvx / Lvy; //

                    f_old.set(i, j, ivx, ivy, value);
                }
            }
        }
    }

    integrate(mesh, f_old, rho);
    rho.write(runTime.time);
    E_total = E_charge + E_laser;
    E_total.write(runTime.time);

    // ---------- the time loop - - - - - - - - -//
    while (runTime.run()) {

        // increment time
        runTime.advance();
        std::cout << std::string(50, '=') << endl;
        std::cout << "Running: t = " << runTime.time << std::endl;

        for (int i = 0; i < Nx2; ++i) {
            for (int j = 0; j < Ny2; ++j) {
                double x = mesh.get_x(i);
                double y = mesh.get_y(j);

                E_laser.set_x(i, j, laser(x, y, runTime.time, Lx, Ly) * std::cos(3.14159265 * runTime.time));
                E_laser.set_y(i, j, laser(x, y, runTime.time, Lx, Ly) * std::sin(3.14159265 * runTime.time));

            }
        }
        


        // superimpose electric field
        E_total = E_charge + E_laser;

        // solve for f with iterative implicit solver
        iterative_implicit_solver(mesh, f_old, f_new, E_total, runTime.time, dt, qm, g);

        // solve for charge/voltage 
        integrate(mesh, f_new, rho);
        poisson(mesh, rho, V);

        // solve for electric field
        gradient(mesh, V, E_charge);

        // output charge and E field
        if (runTime.write_now()) {
            rho.write(runTime.time);
            E_total.write(runTime.time);
        }

        // update f
        f_old = f_new;
    }
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Complete!";

    return 0;
}
