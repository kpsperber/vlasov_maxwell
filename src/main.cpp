#include <iostream>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "SimulationTime.h"
#include "DistributionFunction.h"

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

    // Declare velocity field
    ScalarField V(mesh, "V");

    // Define initial distribution function
    DistributionFunction f(mesh, "f");

    // std::cout << "- -- Outputs -- -" << std::endl;

    // ---------- the time loop - - - - - - - - -//
    while ( runTime.run() ) {

        // advance time
        runTime.advance();

        // solve for f with iterative implicit solver function

        // solve for charge density with poisson solver

        // compute voltage

        // integrate voltage 

        // superimpose the electric field of laser

    }

    return 0;
}
