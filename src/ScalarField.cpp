#include <iostream>
#include <fstream>
#include <iomanip>
#include "ScalarField.h"

ScalarField::ScalarField(const Mesh2D& mesh, std::string fieldName)
    : Nx(mesh.get_Nx()), Ny(mesh.get_Ny()),
      Nx2(mesh.get_Nx() + 2), Ny2(mesh.get_Ny() + 2),
      fieldName(fieldName)
{
    data.assign((mesh.get_Nx() + 2) * (mesh.get_Ny() + 2), 0.0);
}

double ScalarField::get(const int i, const int j) const {
    return data[i + j * Nx2];
}

void ScalarField::set(const int i, const int j, const double value) {
    data[i + j * Nx2] = value;
}

void ScalarField::write(double t) const {
    std::string path = "output/" + fieldName + "_" + std::to_string(t) + ".dat";
    std::ofstream file(path);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }

    file << std::scientific << std::setprecision(16);

    for (int j = 1; j <= Ny; j++) {
        for (int i = 1; i <= Nx; i++) {
            file << get(i, j) << "\n";
        }
    }

    file.close();
}
