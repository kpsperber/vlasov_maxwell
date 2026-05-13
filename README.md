# Laser-Plasma Interactions 
This project implements a C++ solver for modeling the evolution of a collisionless plasma under electromagnetic forcing using the reduced Vlasov-Poisson equation. 

**Course:** MEGN651 - Advanced Computational Fluid Dynamics  
**Team Members:**
- Jenna Ramsey-Rutledge
- Byron Selvage
- Kyle Sperber


## Background

Using lasers to drive a plasma is an active field of research in High Energy Density Physics, Nuclear Fusion, and Atomic, Molecular and Optical Physics. When an intense laser pulse interacts with matter, it can deliver enough energy to ionize atoms and molecules, stripping electrons from their bound states and produce a quasi-neutral plasma consisting of ions and free electrons. Once formed, this plasma can strongly interact with the incident electromagnetic field producing a wide array on nonlinear phenomena.  

The Vlasov-Maxwell system provides a governing set of PDEs that model the evolution of a plasma distribution under electromagnetic forcing. The electromagnetic field due to the plasma charges and the laser electric field evolve with respect to the Maxwell Equations, while the Vlasov equation describes the evolution of the particle distribution function in phase space under the action of the Lorentz force. Together, these equations self-consistently couple particle motion to the electromagnetic field evolution, enabling the study of wave-particle interactions, plasma instabilities, and nonlinear laser-plasma coupling.

## Physics and Mathematical Model

### Governing Equations
In this work, we model the evolution of a collisionless plasma under electromagnetic forcing using the reduced Vlasov-Poisson equation over the spatial domain $\Omega = [0, L_x] \times [0, L_y]$ and the velocity domain $V$. The particle distribution $f(\vec{r}, \vec{v}, t)$ evolves according to the collisionless Vlasov equation,

$$ \frac{\partial f}{\partial t} + \vec{v}\cdot\nabla_{\vec{r}} f + \frac{q}{m}\vec{E}\cdot\nabla_{\vec{v}} f = 0 $$

where $q$ and $m$ denote the particle charge and mass, respectively, and $\vec{E}$ is the applied electric field. This equation will need to be solved for both the electrons and the resulting cations which have charge $-q_{e^-} = q_{\text{cations}} = q$. The electric field is determined from Poisson's Equation,

$$
\begin{equation*}
    \nabla \cdot \vec{E} = \frac{\rho}{\varepsilon_0}
\end{equation*}
$$


where the charge density is computed from the velocity moments of the distribution function,

$$
\begin{equation*}
    \rho = q \int f(\vec{r}, \vec{v})\, d\vec{v} 
\end{equation*}
$$


and is superimposed with the electric field from the laser;

$$
\begin{equation*}
    \vec{E}(x, y) = E_0 e^{-\frac{x^2 + y^2}{w^2}} (\hat{x} + \hat{y})
\end{equation*}
$$


where $w$ is the beam waist and is about 1 mm. Since electrons move in the direction opposite of the electric field line, we expect electrons to cluster toward the center of the laser and the cations to get repelled.

### Initial and Boundary Conditions
To model this we must set initial and boundary conditions. To model the boundary conditions we are going to set it such that inflow is not allowed i.e.

$$
\begin{equation*}
    f(\vec{r}_\text{boundary}, \vec{v}, t) = 0 \quad \vec{v} \cdot \hat{n} <0
\end{equation*}
$$

otherwise we will extrapolate the interior points onto the boundary points. For the initial conditions we will first assume that $f$ is separable into its spatial and velocity components. This implies that $f(\vec{r}, \vec{v}, 0) = g(\vec{r}) h(\vec{v})$. To establish the velocity initial condition we will assume that our particles are non-interacting and non-relativistic. From statistical mechanics this means that for the velocity component $h$ we can follow the Maxwell-Boltzmann Distribution

$$
\begin{equation*}
    h(\vec{v}) = \left(\frac{m}{2 \pi k_B T}\right) \exp\left( -\frac{m \vec{v}^2}{2 k_B T} \right).
\end{equation*}
$$

where $k_B$ is the Boltzmann constant, and $T$ is the temperature. Here we will assume room temperature at about $278.15^\circ K$.

For the spatial initial condition, we will assume that the particles are uniformly distributed about our domain, giving the following.

$$
\begin{equation*}
    g(\vec{r}) = \mathcal{U}(\vec{r})
\end{equation*}
$$

this acts to ensure that our atmosphere is initially charge neutral as we get

$$
\begin{align*}
    \rho &= \int_\Omega \int_V f(\vec{r}, \vec{v})\\
    &= \int_\Omega \int_V g(\vec{r}) h(\vec{v})\\
    &= \int_\Omega \left(q_{e^-}\mathcal{U}_{e^-} + q_{\text{cations}} \, \mathcal{U}_\text{cations}\right) \int_V h(\vec{v})\\
    &= \int_\Omega \left(q_{e^-}\mathcal{U}_{e^-} + q_{\text{cations}} \, \mathcal{U}_\text{cations}\right) \cdot 1\\
    \rho &= (-q + q) \cdot 1 = 0
\end{align*}
$$

For the electric field the boundary conditions are quite simple. We will assume that as we leave the main region of interaction the field goes to zero quickly giving the Dirichlet condition $\vec{E}(\vec{r}_\text{boundary}) = 0$.


## Numerical Methods
The system is solved numerically on a discretized phase-space mesh consisting of both spatial and velocity coordinates. An implicit upwinding, finite difference scheme is used to evolve the distribution function in time, while Poisson’s equation is solved at each timestep to compute the self-consistent electric field generated by the plasma charge density.

The system is solved on a structured Cartesian phase-space mesh, where spatial coordinates are discretized over the domain 

$$\Omega = \[0, L_x\] \times \[0,L_y\]$$

and velocity space is discretized over

$$V = \[v_{x,min},v_{x,max}\] \times \[v_{y,min},v_{y_max}\].$$

Ghost cells are included on each boundary to simplify the implementation of boundary conditions and finite difference stencils. To approximate the spatial derivatives and velocity-space derivatives, we used first order upwind finite differences. To advance the PDE in time, we used first order implicit backward Euler discretization. The resulting update equation is written as 

$$\frac{f^{n+1}-f^n}{\delta t} + \vec \cdot \nambla_{\vec{r}} f + \frac{q}{m}\vec{E}\cdot \nambla_v f^{n+1} = 0.$$

All advective terms were dealt with upwinding. The upwind direction was selected dynamically based on the sign of local velocity or acceleration. For instance, a positive particle velocity would use a backward spatial stencil while a negative one would use a forward stencil. 

The fully implicit discretization produces a nonlinear algebraic system for the updated distribution function. This system is solved using fixed-point iteration. At each iteration, neighboring phase-space values from the previous iterate are used to compute an updated value of the distribution function. Iteration continues until the maximum change between consecutive iterations falls below 1.0e-10. 


## Verification
First, we verified that our Poisson solver converged. We used the manufactured solution 

$$v(x,y) = \sin(x)\sin(y).$$

As a result, we confirmed that the solver had an average convergence rate of 1.99. 

Next, we checked that the iterative implicit solver ran with minimal error. In order to do this, we set the electric field equal to zero then ran an iteration of the solver. Our maximum error was 8.97014e-024.

Finally, the solver was verified using the Method of Manufactured Solutions. The test solution

$$f = \frac{sin(x)sin(y)cos(t)}{L_{vx} L_{vy}}$$

is prescribed over the domain $L_x = L_y = 2\pi$ with a manufactured forcing function $g(x, y, v_x, v_y, t)$ derived by substituting $f$ into the Vlasov equation analytically. We found

$$g = \frac{-\sin(x)\sin(y)\sin(t)}{L_{vx}L_{vy}} + \frac{v_x\cos(x)\sin(y)\cos(t)}{L_{vx}L_{vy}} + \frac{v_y\sin(x)\cos(y)\cos(t)}{L_{vx}L_{vy}}.$$

The solver is run with this forcing and the computed solution is compared to the exact $f$. The accuracy of $f$ was verified qualitatively side by side with the expected solution. These results matched the manufactured solution.

## Applying the Solver
Implementing the gaussian electric field and the intial conditions as described above we were able to get physical results. The charge density for the electrons start uniformly distributed across the domain. Then, when the laser is turned on the electron distribution is ejected away from the laser forming a gaussian shaped void. This makes sense as in a real scenario the electrons are going to be repelled by the electric field of the laser.

## Quick Start

### Prerequisites

- C++17-compatible compiler
- GNU Make
- Eigen 5.0.0+

### Building and Running

```bash
# Clone the repository
git clone <repo-url>
cd vlasov_maxwell

# Compile the project
make build

# Run the program
./build/solver.exe

# Remove compiled artifacts
make clean

# Build and run in one step
make all
```

### Adjusting the Simulation

Edit the CSV files in `input/` to change the spatial domain, velocity domain, or resolution before building


## Project Structure

```
vlasov_maxwell/
├── src/                      # Source and header files
│   ├── DistributionFunction.{cpp,h}    # Particle distribution function
│   ├── FileIO.{cpp,h}                  # Helper functions for reading inputs
│   ├── Mesh2D.{cpp,h}                  # Spatial + velocity mesh
│   ├── Operators.{cpp,h}               # Gradient and integrate operators
│   ├── ScalarField.{cpp,h}             # Scalar field data structure for charge density
│   ├── SimulationTime.{cpp,h}          # Time stepping
│   ├── Solvers.cpp                     # Poisson and iterative implicit solver
│   ├── VectorField.{cpp,h}             # Vector field data structure for electric field 
│   ├── main.cpp
│   └── README.md
├── input/                    # Configuration files
│   ├── mesh_grid.csv         # Spatial + velocity mesh parameters
│   ├── time_grid.csv         # Time stepping parameters
│   └── README.md
├── output/                   # Results
├── scripts/                  # Postprocessing scripts
├── tests/                    # Test Module
│   ├── test_poisson.csv               # Runs tests for building the poisson equation 
│   ├── test_solver.csv                # Runs tests for the iterative implicit solver
│   ├── makefile                       # Build test suites
├── makefile                  # Build configuration
└──README.md                  # This file
```

## References

- Brewer, Dustin & Pankavich, Stephen. (2011). Computational Methods for a One-Directional Plasma Model with Transport Field. SIAM Undergraduate Research Online. 4. 10.1137/11S010906. 
