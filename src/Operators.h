#ifndef OPERATORS_H
#define OPERATORS_H

#include "Mesh2D.h"
#include "DistributionFunction.h"
#include "ScalarField.h"
#include "VectorField.h"

/**
 * @brief Integrate the distribution over velocity space to obtain charge density rho(x,y).
 *
 * @param mesh Phase-space mesh
 * @param f Distribution function f(x,y,vx,vy)
 * @param rho Scalar field to fill with rho on the spatial grid
 */
void integrate(const Mesh2D& mesh, const DistributionFunction& f, ScalarField& rho);

/**
 * @brief Centered finite-difference gradient of a scalar potential
 *
 * @param mesh Spatial mesh
 * @param V Scalar potential
 * @param E Vector field to receive (Ex, Ey) at each spatial point
 */
void gradient(const Mesh2D& mesh, const ScalarField& V, VectorField& E);

#endif // OPERATORS_H