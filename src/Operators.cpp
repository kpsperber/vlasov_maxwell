#include <vector>
#include "Mesh2D.h"
#include "VectorField.h"
#include "ScalarField.h"
#include "DistributionFunction.h"

using namespace std;

void integrate(const Mesh2D& mesh, const DistributionFunction& f, ScalarField& rho) {
    int Nx = mesh.get_Nx();
    int Ny = mesh.get_Ny();

    int Nx2 = Nx + 2;
    int Ny2 = Ny + 2;

    int Nvx = mesh.get_Nvx();
    int Nvy = mesh.get_Nvy();

    double dvx = mesh.get_dvx();
    double dvy = mesh.get_dvy();

    double q = 1;

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {

            double sum = 0.0;

            for (int ivx = 1; ivx <= Nvx; ++ivx) {
                for (int ivy = 1; ivy <= Nvy; ++ivy) {
                    sum += f.get(i, j, ivx, ivy) * dvx * dvy;
                }
            }

            rho.set(i, j, q * sum);
        }
    }
}

void gradient(const Mesh2D& mesh, const ScalarField& V, VectorField& E) {
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
                E.set_x(i, j, -ddx);
            }
            else if (i == Nx2 - 1) {
                ddx = (V.get(i, j) - V.get(i - 1, j)) / dx;
                E.set_x(i, j, -ddx);
            }
            else {
                ddx = (V.get(i + 1, j) - V.get(i - 1, j)) / (2.0 * dx);
                E.set_x(i, j, -ddx);
            }

            // dV/dy
            if (j == 0) {
                ddy = (V.get(i, j + 1) - V.get(i, j)) / dy;
                E.set_y(i, j, -ddy);
            }

            else if (j == Ny2 - 1) {
                ddy = (V.get(i, j) - V.get(i, j - 1)) / dy;
                E.set_y(i, j, -ddy);
            }

            else {
                ddy = (V.get(i, j + 1) - V.get(i, j - 1)) / (2.0 * dy);
                E.set_y(i, j, -ddy);
            }
        }
    }
}