#include <iostream>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "SimulationTime.h"
#include "DistributionFunction.h"
#include "Solvers.h"

int main() {
    // - - - - - - - - - preprocessing - - - - - - - - - -//

    // initialize runTime
    SimulationTime runTime("input/time_grid.csv");

    // Create Mesh
    Mesh2D mesh("input/mesh_grid.csv");

    // Declare electric field
    VectorField E("E", mesh);

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

    // std::cout << "- -- Outputs -- -" << std::endl;

    // ---------- the time loop - - - - - - - - -//
    while ( runTime.run() ) {

        // advance time
        runTime.advance();

        // solve for f with iterative implicit solver function
        iterative_implicit_solver(mesh, f_old, f_new, E, dt, qm);

        // solve for charge density with poisson solver
        poisson(mesh,rho,V);


        // compute voltage

        // integrate voltage 

        // superimpose the electric field of laser

    }

    return 0;
}
