#include "Mesh2D.h"

Mesh2D::Mesh2D(
    int Nx_in, int Ny_in,
    int Nvx_in, int Nvy_in,
    double xmin_in, double xmax_in,
    double ymin_in, double ymax_in,
    double vxmin_in, double vxmax_in,
    double vymin_in, double vymax_in
) {
    Nx = Nx_in;
    Ny = Ny_in;
    Nvx = Nvx_in;
    Nvy = Nvy_in;

    Nx2 = Nx + 2;
    Ny2 = Ny + 2;
    Nvx2 = Nvx + 2;
    Nvy2 = Nvy + 2;

    xmin = xmin_in;
    xmax = xmax_in;
    ymin = ymin_in;
    ymax = ymax_in;

    vxmin = vxmin_in;
    vxmax = vxmax_in;
    vymin = vymin_in;
    vymax = vymax_in;

    dx = (xmax - xmin) / Nx;
    dy = (ymax - ymin) / Ny;

    dvx = (vxmax - vxmin) / Nvx;
    dvy = (vymax - vymin) / Nvy;

    x.assign(Nx2, 0.0);
    y.assign(Ny2, 0.0);
    vx.assign(Nvx2, 0.0);
    vy.assign(Nvy2, 0.0);

    // Cell-centered coordinates with ghost cells.
    // Interior cells are 1 through Nx, 1 through Ny, etc.
    for (int i = 0; i < Nx2; ++i) {
        x[i] = xmin + (i - 0.5) * dx;
    }

    for (int j = 0; j < Ny2; ++j) {
        y[j] = ymin + (j - 0.5) * dy;
    }

    for (int k = 0; k < Nvx2; ++k) {
        vx[k] = vxmin + (k - 0.5) * dvx;
    }

    for (int l = 0; l < Nvy2; ++l) {
        vy[l] = vymin + (l - 0.5) * dvy;
    }
}

int Mesh2D::get_Nx() const {
    return Nx;
}

int Mesh2D::get_Ny() const {
    return Ny;
}

int Mesh2D::get_Nvx() const {
    return Nvx;
}

int Mesh2D::get_Nvy() const {
    return Nvy;
}

int Mesh2D::get_Nx2() const {
    return Nx2;
}

int Mesh2D::get_Ny2() const {
    return Ny2;
}

int Mesh2D::get_Nvx2() const {
    return Nvx2;
}

int Mesh2D::get_Nvy2() const {
    return Nvy2;
}

double Mesh2D::get_dx() const {
    return dx;
}

double Mesh2D::get_dy() const {
    return dy;
}

double Mesh2D::get_dvx() const {
    return dvx;
}

double Mesh2D::get_dvy() const {
    return dvy;
}

double Mesh2D::get_x(int i) const {
    return x[i];
}

double Mesh2D::get_y(int j) const {
    return y[j];
}

double Mesh2D::get_vx(int k) const {
    return vx[k];
}

double Mesh2D::get_vy(int l) const {
    return vy[l];
}

int Mesh2D::idx_space(int i, int j) const {
    return i + j * Nx2;
}

int Mesh2D::idx_phase(int i, int j, int k, int l) const {
    return i + Nx2 * (j + Ny2 * (k + Nvx2 * l));
}

void Mesh2D::write_coordinates() const {
    std::ofstream file("mesh_coordinates.dat");

    file << "# x coordinates\n";
    for (int i = 0; i < Nx2; ++i) {
        file << i << " " << x[i] << "\n";
    }

    file << "\n# y coordinates\n";
    for (int j = 0; j < Ny2; ++j) {
        file << j << " " << y[j] << "\n";
    }

    file << "\n# vx coordinates\n";
    for (int k = 0; k < Nvx2; ++k) {
        file << k << " " << vx[k] << "\n";
    }

    file << "\n# vy coordinates\n";
    for (int l = 0; l < Nvy2; ++l) {
        file << l << " " << vy[l] << "\n";
    }

    file.close();
}