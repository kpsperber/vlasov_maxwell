#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Mesh {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        double Lx, Ly;
        double dx, dy;
        string meshName;
        vector<double> xc, yc;
        vector<double> xf, yf;
    
    public:
        int get_Nx() const;
        int get_Ny() const;
        double get_Lx() const;
        double get_Ly() const;
        double get_dx() const;
        double get_dy() const;
        double get_xf(int i) const;
        double get_xc(int i) const;
        double get_yf(int j) const;
        double get_yc(int j) const;
        void read(const string& filename);
        void disp() const;
        void build();
        void write() const;
        Mesh();
        Mesh(string name, const string& filename);

};

#endif