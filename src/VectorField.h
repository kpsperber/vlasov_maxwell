#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include <vector>
#include <string>
#include <fstream>
using namespace std;

#include "Mesh.h"

class VectorField {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        vector<double> vx;
        vector<double> vy;
        std::string name;

    public:
        VectorField(const string& fieldName, const Mesh& mesh);
        double get_x_component(const int i, const int j);
        double get_y_component(const int i, const int j);
        void set_x_component(const int i, const int j, const double val);
        void set_y_component(const int i, const int j, const double val);
        void write();
};

#endif