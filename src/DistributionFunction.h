#ifndef DISTRIBUTIONFUNCTION_H
#define DISTRIBUTIONFUNCTION_H

#include <string>
#include <vector>

#include "Mesh2D.h"

/**
 * @brief Distribution function on a two spatial (x,y) and two velocity (vx,vy) phase-space mesh.
 *
 * Stores the value of the distribution function f(i,j,k,l) at every point in a 2D (x,y) mesh and a 2D (vx,vy) velocity space.
 */
class DistributionFunction {
private:
    int Nx, Ny, Nvx, Nvy;
    int Nx2, Ny2, Nvx2, Nvy2;
    std::string name;
    std::vector<double> data;

    /**
     * @brief Map 4D mesh indices to data index.
     *
     * @param i Spatial x index
     * @param j Spatial y index
     * @param k Velocity x index
     * @param l Velocity y index
     * @return Data index
     */
    int index(int i, int j, int k, int l) const;

public:
    /**
     * @brief Construct a zero-initialized distribution.
     *
     * @param fieldName Name of the distribution function
     * @param mesh Mesh2D defining the spatial and velocity mesh
     */
    DistributionFunction(const std::string& fieldName = "f", const Mesh2D& mesh);

    /**
     * @brief Get the value of the distribution function at mesh indices (i,j,k,l).
     *
     * @param i Spatial x index
     * @param j Spatial y index
     * @param k Velocity x index
     * @param l Velocity y index
     * @return Value of the distribution function at (i,j,k,l)
     */
    double get(int i, int j, int k, int l) const;

    /**
     * @brief Set the value of the distribution function at mesh indices (i,j,k,l).
     *
     * @param i Spatial x index
     * @param j Spatial y index
     * @param k Velocity x index
     * @param l Velocity y index
     * @param value Value to set at (i,j,k,l)
     */
    void set(int i, int j, int k, int l, double value);

    /**
     * @brief Add to the current value of the distribution function at mesh indices (i,j,k,l).
     *
     * @param i Spatial x index
     * @param j Spatial y index
     * @param k Velocity x index
     * @param l Velocity y index
     * @param value Value to add at (i,j,k,l)
     */
    void add(int i, int j, int k, int l, double value);

    /**
     * @brief Get the number of stored samples (Nx2 * Ny2 * Nvx2 * Nvy2).
     */
    int size() const;

    /**
     * @brief Set every entry in the backing array to the same constant.
     *
     * @param val Value to assign to all cells.
     */
    void fill(double val);

    /**
     * @brief Write the distribution function to a file.
     */
    void write() const;
};

#endif // DISTRIBUTIONFUNCTION_H
