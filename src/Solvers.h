#ifndef SOLVERS_H
#define SOLVERS_H

#include <functional>

#include "Mesh2D.h"
#include "ScalarField.h"

class DistributionFunction;
class VectorField;

/**
 * Solve the Poisson equation for a given charge density field and voltage boundary conditions.
 * 
 * Applies zero boundary conditions for voltage.
 * 
 * @param mesh Reference to the computational mesh
 * @param density Scalar field representing charge density (ρ)
 * @param voltage Scalar field representing voltage to be solved for. This is modified in-place.
 */
void poisson(const Mesh2D& mesh, const ScalarField& density, ScalarField& voltage);

/**
 * @brief Backward Euler step for Vlasov equation.
 *
 * @param mesh Mesh2D defining the spatial and velocity mesh
 * @param f_old Distribution at the beginning of the step
 * @param f_new Distribution after the step (buffer to write into)
 * @param E Electric field
 * @param dt Time step
 * @param qm Charge-to-mass ratio
 * @param g Function representing the source term, used to test the solver using the Method of Manufactured Solutions. Defaults to 0.
 */
void backward_euler(const Mesh2D& mesh, const DistributionFunction& f_old, DistributionFunction& f_new, const VectorField& E, double dt, double qm, std::function<double(int, int, int, int)> g = [] (int i, int j, int ivx, int ivy) -> double { return 0.0; });

#endif // SOLVERS_H