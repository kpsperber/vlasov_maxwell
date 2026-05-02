#ifndef MESH2D_H
#define MESH2D_H

#include <vector>
#include <string>
#include <fstream>

class Mesh2D {
private:
    // Number of interior cells
    int Nx, Ny;
    int Nvx, Nvy;

    // Number including ghost cells
    int Nx2, Ny2;
    int Nvx2, Nvy2;

    // Domain limits
    double xmin, xmax;
    double ymin, ymax;

    double vxmin, vxmax;
    double vymin, vymax;

    // Grid spacings
    double dx, dy;
    double dvx, dvy;

    // Coordinates including ghost cells
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> vx;
    std::vector<double> vy;

public:
    Mesh2D(
        int Nx, int Ny,
        int Nvx, int Nvy,
        double xmin, double xmax,
        double ymin, double ymax,
        double vxmin, double vxmax,
        double vymin, double vymax
    );

    int get_Nx() const;
    int get_Ny() const;
    int get_Nvx() const;
    int get_Nvy() const;

    int get_Nx2() const;
    int get_Ny2() const;
    int get_Nvx2() const;
    int get_Nvy2() const;

    double get_dx() const;
    double get_dy() const;
    double get_dvx() const;
    double get_dvy() const;

    double get_x(int i) const;
    double get_y(int j) const;
    double get_vx(int k) const;
    double get_vy(int l) const;

    int idx_space(int i, int j) const;
    int idx_phase(int i, int j, int k, int l) const;

    void write_coordinates() const;
};

#endif