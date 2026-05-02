#include <iostream>
#include "Mesh.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "Time.h"


using namespace std;

int main() {
    Mesh spaceGrid("Space", "input/space_grid.csv");
    Mesh velocityGrid("Velocity", "input/velocity_grid.csv");
    VectorField E("E", spaceGrid);
    ScalarField rho(spaceGrid, "rho");
    ScalarField V(spaceGrid, "V");
    Time runTime("time_grid.csv");

    return 0;
}