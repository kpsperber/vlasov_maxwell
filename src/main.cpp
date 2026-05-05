#include <iostream>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "SimulationTime.h"

int main() {
    Mesh2D mesh;
    VectorField E("E", mesh);
    ScalarField rho(mesh, "rho");
    ScalarField V(mesh, "V");
    SimulationTime runTime("input/time_grid.csv");

    return 0;
}
