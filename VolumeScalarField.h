#ifndef VOLUME_SCALAR_FIELD_H
#define VOLUME_SCALAR_FIELD_H

#include <fstream>
using namespace std;

class Volume_Scalar_Field {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        double Lx, Ly;
    
    public:
        Volume_Scalar_Field();
        Volume_Scalar_Field(const string& filename);

};

#endif