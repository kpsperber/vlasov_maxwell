#include <vector>
#include <string>
using namespace std;

#include "VectorField.h"
#include "Mesh.h"

VectorField::VectorField(const string& fieldName, const Mesh& mesh) {
    name = fieldName;
    Nx = mesh.get_Nx();
    Ny = mesh.get_Ny();
    Nx2 = Nx + 2;
    Ny2 = Ny + 2;

    vx.assign(Nx2 * Ny2, 0.0);
    vy.assign(Nx2 * Ny2, 0.0);
}

double VectorField::get_x_component(const int i, const int j) const {
    double val = vx[i + j * Nx2];

    return val;
}

double VectorField::get_y_component(const int i, const int j) const {
    double val = vy[i + j * Nx2];

    return val;
}

void VectorField::set_x_component(const int i, const int j, const double val) {
    vx[i + j * Nx2] = val;
}

void VectorField::set_y_component(const int i, const int j, const double val) {
    vy[i + j * Nx2] = val;
}
