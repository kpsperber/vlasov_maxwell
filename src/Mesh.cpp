#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Mesh.h"

using namespace std;

int Mesh::get_Nx() const {
    return Nx;
}

int Mesh::get_Ny() const {
    return Ny;
}

double Mesh::get_Lx() const {
    return Lx;
}

double Mesh::get_Ly() const {
    return Ly;
}

double Mesh::get_dx() const {
    return dx;
}

double Mesh::get_dy() const {
    return dy;
}

void Mesh::read(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    string line;
    while(getline(file, line)) {
        auto pos = line.find(',');
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        
        if (key == "Nx") {Nx = stoi(value);}
        if (key == "Ny") {Ny = stoi(value);}
        if (key == "Lx") {Lx = stof(value);}
        if(key == "Ly") {Ly = stof(value);}
    }

    file.close();
}

void Mesh::disp() const {
    cout << endl;
    cout << string(50, '=') << endl;
    cout << meshName << " Mesh Properties" << endl;
    cout << string(50, '=') << endl;
    cout << Nx << "; Nx (number of cells in x)" << endl;
    cout << Ny << "; Ny (number of cells in y)" << endl;
    cout << Lx << "; Lx (domain lenght in x, m)" << endl;
    cout << Ly << "; Ly (domain length in y, m)" << endl;
}

void Mesh::build() {
    dx = Lx / Nx;
    dy = Ly / Ny;

    xc.assign(Nx + 2, 0.0);
    xf.assign(Nx + 1, 0.0);
    yc.assign(Ny + 2, 0.0);
    yf.assign(Ny + 1, 0.0);

    for (int i = 0; i < Nx + 2; i++) {
        xc[i] = -0.5 * dx + i * dx;
    }

    for (int i = 0; i < Nx + 1; i++) {
        xf[i] = dx * i;
    }

    for (int i = 0; i < Ny + 2; i++) {
        yc[i] = -0.5 * dy + i * dy;
    }

    for (int i = 0; i < Ny + 1; i++) {
        yf[i] = dy * i;
    }
}

void Mesh::write() const {
    ofstream file("output/grid.csv");

    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    file << "x,y\n";

    for (int j = 1; j <= Ny; j++) {
        for (int i = 1; i <= Nx; i++) {
            file << xc[i] << "," << yc[j] << "\n";
        }
    }

    file.close();
}

double Mesh::get_xc(int i) const {
    return xc[i];
}

double Mesh::get_xf(int i) const {
    return xf[i];
}

double Mesh::get_yc(int j) const {
    return yc[j];
}

double Mesh::get_yf(int j) const {
    return yf[j];
}

Mesh::Mesh(string name, const string& filename) {
    meshName = name;
    this->read(filename);
    this->disp();
    this->build();
}