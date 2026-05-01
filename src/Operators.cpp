#include <vector>
#include "Mesh.h"
#include "VectorField.h"
#include "ScalarField.h"

using namespace std;

void gradient(const Mesh& mesh, const ScalarField& V, VectorField& E) {
    int Nx = mesh.get_Nx();
    int Ny = mesh.get_Ny();

    int Nx2 = Nx + 2;
    int Ny2 = Ny + 2;

    double dx = mesh.get_dx();
    double dy = mesh.get_dy();
    double ddx, ddy;

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {

            if (i == 0) {
                ddx = (V.get(i + 1, j) - V.get(i, j)) / dx;
                E.set_x_component(i, j, -ddx);        
            }
            else if (i == Nx2 - 1) {
                ddx = (V.get(i, j) - V.get(i - 1, j)) / dx;
                E.set_x_component(i, j, -ddx);
            }
            else {
                ddx = (V.get(i + 1, j) - V.get(i - 1, j)) / (2.0 * dx);
                E.set_x_component(i, j, -ddx);
            }

            // dV/dy
            if (j == 0) {
                ddy = (V.get(i, j + 1) - V.get(i, j)) / dy;
                E.set_y_component(i, j, -ddy);
            }

            else if (j == Ny2 - 1) {
                ddy = (V.get(i, j) - V.get(i, j - 1)) / dy;
                E.set_y_component(i, j, -ddy);
            }

            else {
                ddy = (V.get(i, j + 1) - V.get(i, j - 1)) / (2.0 * dy);
                E.set_y_component(i, j, -ddy);
            }
        }
    }
}