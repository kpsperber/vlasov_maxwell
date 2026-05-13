#ifndef SOLVERS_H
#define SOLVERS_H

#include <functional>

#include "Mesh2D.h"
#include "ScalarField.h"

class DistributionFunction;
class VectorField;

/**
 * @brief Solve the Poisson equation for charge density with zero Dirichlet voltage boundaries.
 *
 * @param mesh Reference to the computational mesh
 * @param density Scalar field representing charge density (ρ)
 * @param voltage Scalar field representing voltage to be solved for. This is modified in-place.
 */
void poisson(const Mesh2D& mesh, const ScalarField& density, ScalarField& voltage);

/**
 * @brief Advances the Vlasov distribution function one timestep using
 * an implicit backward Euler upwind discretization.
 *
 * This routine solves the equation in phase space using a first-order implicit finite difference 
 * scheme with upwind advection in both physical and velocity space. The implicit system is solved 
 * using fixed-point iteration.
 *
 * @param mesh Mesh2D defining the spatial and velocity mesh
 * @param f_old Distribution at the beginning of the step
 * @param f_new Distribution after the step (buffer to write into)
 * @param E Electric field
 * @param t Time
 * @param dt Time step
 * @param qm Charge-to-mass ratio
 * @param g Function representing the source term, used to test the solver using the Method of Manufactured Solutions. Defaults to 0.
 */
void iterative_implicit_solver(
    const Mesh2D& mesh,
    const DistributionFunction& f_old,
    DistributionFunction& f_new,
    const VectorField& E,
    double t,
    double dt,
    double qm,
    std::function<double(double, double, double, double, double, double, double)> g = 
        [] (double x, double y, double vx, double vy, double t, double Lvx, double Lvy) -> double {
            return 0.0;
        }
);


#endif // SOLVERS_H