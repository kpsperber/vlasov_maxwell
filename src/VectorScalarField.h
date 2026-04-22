#ifndef VECTOR_SCALAR_FIELD_H
#define VECTOR_SCALAR_FIELD_H

#include <fstream>
using namespace std;

class VectorScalarField {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        double Lx, Ly;
    
    public:
        VectorScalarField();
        VectorScalarField(const string& filename);

};

#endif