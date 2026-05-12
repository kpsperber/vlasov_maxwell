import numpy as np
import matplotlib.pyplot as plt
import glob

def load_mesh():
    mesh = {}
    with open("input/mesh_Grid.csv", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            key, _, rest = line.partition(",")
            key = key.strip()
            val = rest.strip()
            if not key:
                continue
            mesh[key] = val

    Nx = int(mesh["Nx"])
    Ny = int(mesh["Ny"])
    Nvx = int(mesh["Nvx"])
    Nvy = int(mesh["Nvy"])
    Lx = float(mesh["Lx"])
    Ly = float(mesh["Ly"])
    Lvx = float(mesh["Lvx"])
    Lvy = float(mesh["Lvy"])
    return Nx, Ny, Nvx, Nvy, Lx, Ly, Lvx, Lvy

def load_files():
    files = glob.glob("output/rho_*.dat")
    Nx, Ny, Nvx, Nvy, Lx, Ly, Lvx, Lvy = load_mesh()
    rho_data = []
    for file in files:
        print(file)
        time = file.split("_")[1]
        time = float(time.replace(".dat", ""))
        print(time)
        rho = np.loadtxt(file)
        rho = rho.reshape(Nx, Ny)
        rho_data.append((time, rho))
    return rho_data

def plot_rho(rho_data):
    
    for time, rho in rho_data:
        plt.imshow(rho, cmap="viridis")
        plt.colorbar()
        plt.title(f"Charge Density at Time {time:.4e}")
        plt.xlabel("x")
        plt.ylabel("y")
        #plt.show()
        plt.tight_layout()
        plt.savefig(f"output/rho_{time:.4e}.png")
        plt.close()

if __name__ == "__main__":
    rho_data = load_files()
    plot_rho(rho_data)
