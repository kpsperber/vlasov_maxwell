#include <iostream>
#include <cmath>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "SimulationTime.h"
#include "DistributionFunction.h"
#include "Solvers.h"
#include "Operators.h"

double laser(double x, double y) {
    double val = std::exp(-(std::pow(x, 2) + std::pow(y, 2)) / (2));

    return val;
}

double g(double x, double y, double vx, double vy, double t, double Lvx, double Lvy) {
    double val = std::sin(x) * std::sin(y) * std::cos(t) / Lvx / Lvy + vx * std::cos(x) * std::sin(y) * std::sin(t) / Lvx / Lvy + vx * std::sin(x) * std::cos(y) * std::sin(t) / Lvx / Lvy;

    return val;
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
    double Lvx = 2 * mesh.get_vxmax();
    double Lvy = 2 * mesh.get_vymax();

    // Declare electric field
    VectorField E_charge("E Charge", mesh);
    VectorField E_laser("E Laser", mesh);
    VectorField E_total("E Total", mesh);

    // Create charge density field
    ScalarField rho(mesh, "rho");

    // Declare voltage field
    ScalarField V(mesh, "V");

    // Define initial distribution function
    DistributionFunction f_old(mesh, "f_old");
    DistributionFunction f_new(mesh,"f_new");

    // Set global parameters
    double dt = runTime.get_dt();
    double qm = 1.0;

    // Initial Conditions
    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int ivx = 0; ivx < Nvx2; ++ivx) {
                for (int ivy = 0; ivy < Nvy2; ++ivy) {
                    // Velocity field
                    double x = mesh.get_x(i);
                    double y = mesh.get_y(j);
                    // double vx = mesh.get_vx(ivx);
                    // double vy = mesh.get_vy(ivy);

                    double value = std::sin(x) * std::sin(y) / Lvx / Lvy;
                    f_old.set(i, j, ivx, ivy, value);

                    // Zero electric field
                    E_charge.set_x(i, j, 0.0);
                    E_charge.set_y(i, j, 0.0);
                    E_laser.set_x(i, j, 0.0);
                    E_laser.set_y(i, j, 0.0);
                    E_total.set_x(i, j, 0.0);
                    E_total.set_y(i, j, 0.0);
                    
                    // Charge 
                    rho.set(i, j, 0.0);
                }
            }
        }
    }

    // ---------- the time loop - - - - - - - - -//
    while (runTime.run()) {

        runTime.advance();

        E_total = E_charge + E_laser;

        iterative_implicit_solver(mesh, f_old, f_new, E_total, runTime.time, dt, qm, g);

        integrate(mesh, f_new, rho);

        poisson(mesh, rho, V);

        gradient(mesh, V, E_charge);

        if (runTime.write_now()) {
            rho.write(runTime.time);
            E_total.write(runTime.time);
        }

        f_old = f_new;
    }

    return 0;
}
