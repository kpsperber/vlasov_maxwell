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
$$
\begin{equation*}
    \frac{\partial f}{\partial t}
+ \vec{v}\cdot\nabla_{\vec{r}} f
+ \frac{q}{m}\vec{E}\cdot\nabla_{\vec{v}} f = 0
\end{equation*}
$$

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
To solve this at each time step we use a Backward Euler time stepping scheme where we first solve the electric field and then solve the Vlasov equation. [MORE DETAILS AS WE BUILD]

## Verification

The solver is verified using the Method of Manufactured Solutions. The test solution

$$f = \sin(x)\sin(y)\sin(v_x)\sin(v_y)\sin(t)$$

is prescribed over the domain $L_x = L_y = \pi$ with a manufactured forcing function $g(x, y, v_x, v_y, t)$ derived by substituting $f$ into the Vlasov equation analytically. The solver is run with this forcing and the computed solution is compared to the exact $f$, confirming spatial and temporal convergence at the expected rates.

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
│   ├── main.cpp
│   ├── Matrix.{cpp,h}        # Linear solver
│   ├── Mesh.{cpp,h}          # Spatial discretization
│   ├── Time.{cpp,h}          # Time stepping  
│   ├── VolumeScalarField.{cpp,h}
│   ├── VectorScalarField.{cpp,h}
│   └── README.md
├── input/                    # Configuration files
│   ├── space_grid.csv        # Spatial mesh parameters
│   ├── velocity_grid.csv     # Velocity mesh parameters
│   └── README.md
├── output/                   # Results
├── scripts/                  # Postprocessing scripts
├── makefile                  # Build configuration
└──README.md                  # This file
```

## References

- Brewer, Dustin & Pankavich, Stephen. (2011). Computational Methods for a One-Directional Plasma Model with Transport Field. SIAM Undergraduate Research Online. 4. 10.1137/11S010906. 