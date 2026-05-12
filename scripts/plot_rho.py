import glob

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import Normalize
from PIL import Image


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
        time = file.split("_")[1]
        time = float(time.replace(".dat", ""))
        rho = np.loadtxt(file)
        rho = rho.reshape(Nx, Ny)
        rho_data.append((time, rho))
    return rho_data


def render_frame(rho, time, cmap, norm):
    fig, ax = plt.subplots()
    im = ax.imshow(rho, cmap=cmap, norm=norm)
    fig.colorbar(im, ax=ax)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title(f"Charge Density at Time {time:.4e}")

    fig.canvas.draw()
    buf = np.asarray(fig.canvas.buffer_rgba())[..., :3]
    img = Image.fromarray(buf)
    plt.close(fig)
    return img


def plot_rho(rho_data, out_path="output/rho.gif", fps=10, cmap_name="viridis"):
    rho_data = sorted(rho_data, key=lambda x: x[0])
    if not rho_data:
        print("No rho files found; nothing to plot.")
        return

    all_rho = np.stack([r for _, r in rho_data])
    vmin, vmax = float(all_rho.min()), float(all_rho.max())

    cmap = plt.get_cmap(cmap_name)
    norm = Normalize(vmin=vmin, vmax=vmax)

    frames = [render_frame(rho, t, cmap, norm) for t, rho in rho_data]

    palette_source = frames[0].quantize(colors=256)
    quantized = [f.quantize(palette=palette_source, dither=0) for f in frames]

    duration_ms = int(1000 / max(fps, 1e-6))
    quantized[0].save(
        out_path,
        save_all=True,
        append_images=quantized[1:],
        loop=0,
        duration=duration_ms,
    )
    print(f"Saved {out_path}")


if __name__ == "__main__":
    rho_data = load_files()
    plot_rho(rho_data)
