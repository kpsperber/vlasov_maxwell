#include "DistributionFunction.h"

DistributionFunction::DistributionFunction(const Mesh2D& mesh, const std::string& fieldName) {
    name = fieldName;

    Nx = mesh.get_Nx();
    Ny = mesh.get_Ny();
    Nvx = mesh.get_Nvx();
    Nvy = mesh.get_Nvy();

    Nx2 = mesh.get_Nx2();
    Ny2 = mesh.get_Ny2();
    Nvx2 = mesh.get_Nvx2();
    Nvy2 = mesh.get_Nvy2();

    data.assign(Nx2 * Ny2 * Nvx2 * Nvy2, 0.0);
}

int DistributionFunction::index(const int i, const int j, const int k, const int l) const {
    return i + Nx2 * (j + Ny2 * (k + Nvx2 * l));
}

double DistributionFunction::get(const int i, const int j, const int k, const int l) const {
    return data[index(i, j, k, l)];
}

void DistributionFunction::set(const int i, const int j, const int k, const int l, const double val) {
    data[index(i, j, k, l)] = val;
}

void DistributionFunction::add(const int i, const int j, const int k, const int l, const double val) {
    data[index(i, j, k, l)] += val;
}

int DistributionFunction::size() const {
    return data.size();
}

void DistributionFunction::fill(const double val) {
    std::fill(data.begin(), data.end(), val);
}

void DistributionFunction::write() const {
    std::ofstream file(name + ".dat");

    for (int i = 0; i < Nx2; ++i) {
        for (int j = 0; j < Ny2; ++j) {
            for (int k = 0; k < Nvx2; ++k) {
                for (int l = 0; l < Nvy2; ++l) {
                    file << i << " "
                         << j << " "
                         << k << " "
                         << l << " "
                         << get(i, j, k, l)
                         << "\n";
                }
            }
        }
    }

    file.close();
}