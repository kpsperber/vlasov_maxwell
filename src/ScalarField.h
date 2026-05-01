#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include <vector>
#include <string>
#include "Mesh.h"

class Mesh;

class ScalarField {
    private:
        std::vector<double> data;
        double Nx, Ny;
        double Nx2, Ny2;
        std::string fieldName;

    public:
        /**
         * Constructor to initialize the scalar field with the mesh dimensions and a name.
         * 
         * @param mesh Reference to the mesh
         * @param fieldName Name of the scalar field
         */
        ScalarField(Mesh& mesh, std::string fieldName);

        /**
         * Get the value of the scalar field at mesh indices (i,j).
         * 
         * i = 0, Nx + 1 are ghost cells
         * j = 0, Ny + 1 are ghost cells
         * 
         * Values are mapped to a 1D array according to [i + j * (Nx + 2)] indexing.
         * 
         * @param i Index in x-direction
         * @param j Index in y-direction
         * @return Value of the scalar field at (i,j)
         */
        double get(const int i, const int j) const;

        /**
         * Set the value of the scalar field at mesh indices (i,j).
         * 
         * i = 0, Nx + 1 are ghost cells
         * j = 0, Ny + 1 are ghost cells
         * 
         * Values are mapped to a 1D array according to [i + j * (Nx + 2)] indexing.
         * 
         * @param i Index in x-direction
         * @param j Index in y-direction
         * @param value Value to set at (i,j)
         */
        void set(const int i, const int j, const double value);

        /**
         * Save the scalar field to a file.
         *
         * @param filename Optional output file path. Defaults to "output/{fieldName}.out"
         */
        void save(const std::string& filename = "") const;
};

#endif // SCALARFIELD_H