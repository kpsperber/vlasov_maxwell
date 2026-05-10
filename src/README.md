# Source Code

This directory contains all C++ source and header files for the Vlasov-Maxwell solver.

## Files

- **Matrix.{cpp,h}** - Matrix construction, RHS assembly, and sparse linear solver
- **FileIO.{cpp,h}** - Key-value input .csv file reader
- **Mesh2D.{cpp,h}** - Spatial and velocity mesh
- **ScalarField.{cpp,h}** - Scalar field data structures
- **SimulationTime.{cpp,h}** - Time management
- **Solvers.{cpp,h}** - Solver for Poisson Equation and Backward Euler
- **VectorField.{cpp,h}** - Vector field data structures
- **DistributionFunction.{cpp,h}** - Source term data structure
- **main.cpp** - Main program
