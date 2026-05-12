#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include <vector>
#include <string>
#include "Mesh2D.h"

/**
* @brief Scalar field data structure
*
* Stores the value of a scalar field at every point in a 2D (x,y) mesh.
*/
class ScalarField {
    private:
        int Nx, Ny;
        int Nx2, Ny2;
        std::vector<double> data;
        std::string fieldName;

    public:
        /**
         * @brief Constructor to initialize the scalar field with the mesh dimensions and a name.
         * 
         * @param mesh Reference to the mesh
         * @param fieldName Name of the scalar field
         */
        ScalarField(const Mesh2D& mesh, std::string fieldName);

        /**
         * @brief Get the value of the scalar field at mesh indices (i,j).
         * 
         * @param i Index in x-direction
         * @param j Index in y-direction
         * @return Value of the scalar field at (i,j)
         */
        double get(const int i, const int j) const;

        /**
         * @brief Set the value of the scalar field at mesh indices (i,j).
         * 
         * @param i Index in x-direction
         * @param j Index in y-direction
         * @param value Value to set at (i,j)
         */
        void set(const int i, const int j, const double value);

        /**
         * @brief Save the scalar field to a file.
         *
         * @param filename Optional output file path. Defaults to "output/{fieldName}.out"
         */
        void write(const std::string& filename = "", double t) const;
};

#endif // SCALARFIELD_H