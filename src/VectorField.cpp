#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "VectorField.h"

VectorField::VectorField(const string& fieldName, const Mesh2D& mesh) {
    name = fieldName;
    Nx = mesh.get_Nx();
    Ny = mesh.get_Ny();
    Nx2 = Nx + 2;
    Ny2 = Ny + 2;

    vx.assign(Nx2 * Ny2, 0.0);
    vy.assign(Nx2 * Ny2, 0.0);
}

double VectorField::get_x(const int i, const int j) const {
    double val = vx[i + j * Nx2];

    return val;
}

double VectorField::get_y(const int i, const int j) const {
    double val = vy[i + j * Nx2];

    return val;
}

void VectorField::set_x(const int i, const int j, const double val) {
    vx[i + j * Nx2] = val;
}

void VectorField::set_y(const int i, const int j, const double val) {
    vy[i + j * Nx2] = val;
}

VectorField VectorField::operator+(const VectorField& other) const {
    if (Nx2 != other.Nx2 || Ny2 != other.Ny2) {
        throw std::runtime_error("VectorField::operator+: field sizes do not match");
    }

    VectorField result = *this;
    result.name = name + "_plus_" + other.name;

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            result.set_x(i, j, get_x(i, j) + other.get_x(i, j));
            result.set_y(i, j, get_y(i, j) + other.get_y(i, j));
        }
    }

    return result;

}
    
}

void VectorField::write(double t) {
    std::string path = "output/" + name + "_" + std::to_string(t) + ".dat";
    std::ofstream file(path);

    file << "\n# vx values\n";
    for (int k = 0; k < Nx2; ++k) {
        file << k << " " << vx[k] << "\n";
    }

    file << "\n# vy values\n";
    for (int l = 0; l < Ny2; ++l) {
        file << l << " " << vy[l] << "\n";
    }

    file.close();

}
