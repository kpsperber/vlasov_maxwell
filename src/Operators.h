#ifndef OPERATORS_H
#define OPERATORS_H

void integrate(const Mesh2D& mesh, const DistributionFunction& f, ScalarField& rho);
void gradient(const Mesh2D& mesh, const ScalarField& V, VectorField& E);


#endif