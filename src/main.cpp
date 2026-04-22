#include <iostream>
#include "Matrix.h"
#include "Mesh.h"
#include "Time.h"
#include "VolumeScalarField.h"
#include "VectorScalarField.h"

using namespace std;

int main() {
    Mesh spaceGrid("Space", "input/space_grid.csv");
    Mesh velocityGrid("Velocity", "input/velocity_grid.csv");


    return 0;
}