#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <vector>
#include <Eigen/Sparse>
#include "Mesh.h"
#include "VectorScalarField.h"
#include "VolumeScalarField.h"
#include "Time.h"
using namespace std;

class Matrix {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        int Ntot;
        vector<int> ROW, COL;
        vector<double> VAL;

    
    public:
        Matrix();
        Matrix(const string& filename);
        void add_entry(int row, int col, double val);
        void set_rhs(int i, int j, double val);
        double get_rhs(int i, int j);
        void build_A(const Mesh& mesh, const Time& time);
        void build_rhs(const Mesh& mesh, const Time& time);
        void solve(Volume_Scalar_Field field);

};

#endif