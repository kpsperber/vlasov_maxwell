# Input Data

This directory contains input files required for the solver.

## Files

- **mesh_grid.csv** - Spatial and velocity mesh (keys `Nx`, `Ny`, `Lx`, `Ly`, `Nvx`, `Nvy`, `Lvx`, `Lvy`; domains `[0,Lx]×[0,Ly]` and `[0,Lvx]×[0,Lvy]`)
- **time_grid.csv** - Time stepping (`t0`, `tf`, `dt`, `Nt`, `writeInterval`)

## Format

CSV files contain comma-separated key-value pairs (optional `#` comments). Values may end with `;`.
```
Nx, 256
Ny, 256
Lx, 0.001
Ly, 0.001
Nvx, 64
Nvy, 64
Lvx, 1.0
Lvy, 1.0
```

