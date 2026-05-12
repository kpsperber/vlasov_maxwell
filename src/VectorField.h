#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include <vector>
#include <string>
#include <fstream>
using namespace std;

#include "Mesh2D.h"

/**
* @brief Vector field data structure
*
* Stores the x and y components of a vector field at every point in a 2D (x,y) mesh.
*/
class VectorField {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        vector<double> vx;
        vector<double> vy;
        std::string name;

    public:
        /**
        * @brief Constructor to initialize the vector field with the mesh dimensions and a name.
        *
        * @param fieldName Name of the vector field
        * @param mesh Reference to the mesh
        */
        VectorField(const string& fieldName, const Mesh2D& mesh);

        /**
        * @brief Get the x-component of the vector field at mesh indices (i,j).
        *
        * @param i Index in x-direction
        * @param j Index in y-direction
        * @return x-component of the vector field at (i,j)
        */
        double get_x(const int i, const int j) const;

        /**
        * @brief Get the y-component of the vector field at mesh indices (i,j).
        *
        * @param i Index in x-direction
        * @param j Index in y-direction
        * @return y-component of the vector field at (i,j)
        */
        double get_y(const int i, const int j)const;

        /**
        * @brief Set the x-component of the vector field at mesh indices (i,j).
        *
        * @param i Index in x-direction
        * @param j Index in y-direction
        * @param val Value to set at (i,j)
        */
        void set_x(const int i, const int j, const double val);

        /**
        * @brief Set the y-component of the vector field at mesh indices (i,j).
        *
        * @param i Index in x-direction
        * @param j Index in y-direction
        * @param val Value to set at (i,j)
        */
        void set_y(const int i, const int j, const double val);

        /**
        * @brief Write the vector field to a file.
        */
        void write();

        VectorField operator+(const VectorField& other) const;
};

#endif