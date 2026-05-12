#ifndef OPERATORS_H
#define OPERATORS_H

#include "Mesh2D.h"
#include "DistributionFunction.h"
#include "ScalarField.h"
#include "VectorField.h"

void integrate(const Mesh2D& mesh, const DistributionFunction& f, ScalarField& rho);
void gradient(const Mesh2D& mesh, const ScalarField& V, VectorField& E);

#endif