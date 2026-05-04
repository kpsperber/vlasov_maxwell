#ifndef SOLVERS_H
#define SOLVERS_H

#include "Mesh2D.h"
#include "ScalarField.h"


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

#endif // SOLVERS_H