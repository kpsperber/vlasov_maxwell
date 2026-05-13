import glob
from pathlib import Path

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


def load_rho_data(Nx, Ny):
    files = glob.glob("output/rho_*.dat")
    rho_data = []
    for file in files:
        time = Path(file).stem.replace("rho_", "")
        time = float(time)
        rho = np.loadtxt(file)
        rho = rho.reshape(Nx, Ny)
        rho_data.append((time, rho))
    return rho_data


def load_E_data(Nx, Ny):
    files = glob.glob("output/E_Charge_plus_E_Laser_*.dat")
    e_data = []
    for file in files:
        time = Path(file).stem.replace("E_Charge_plus_E_Laser_", "")
        time = float(time)
        ex_values = []
        ey_values = []

        with open(file, encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line.startswith("#"):
                    continue
                cols = [float(col) for col in line.split()]
                ex_values.append(cols[0])
                ey_values.append(cols[1])
        ex_values = np.asarray(ex_values).reshape(Nx, Ny)
        ey_values = np.asarray(ey_values).reshape(Nx, Ny)
        e_data.append((time, ex_values, ey_values))
    return e_data


def render_rho_frame(rho, time, cmap, norm):
    fig, ax = plt.subplots()
    im = ax.imshow(rho, cmap=cmap, norm=norm)
    fig.colorbar(im, ax=ax)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title(f"Charge Density at Time {time:.4f}")

    fig.canvas.draw()
    buf = np.asarray(fig.canvas.buffer_rgba())[..., :3]
    img = Image.fromarray(buf)
    plt.close(fig)
    return img


def render_E_frame(Ex, Ey, rho, time, rho_cmap, rho_norm, quiver_step=4):
    Ny, Nx = Ex.shape

    # Physical domain: 0 to 2pi in both directions
    x = np.linspace(0, 2 * np.pi, Nx)
    y = np.linspace(0, 2 * np.pi, Ny)

    X, Y = np.meshgrid(x[::quiver_step], y[::quiver_step])

    U = Ex[::quiver_step, ::quiver_step]
    V = Ey[::quiver_step, ::quiver_step]

    fig, ax = plt.subplots()

    im = ax.imshow(
        rho,
        cmap=rho_cmap,
        norm=rho_norm,
        origin="lower",
        aspect="auto",
        extent=[0, 2 * np.pi, 0, 2 * np.pi],
    )

    fig.colorbar(im, ax=ax, label="Charge Density")

    ax.quiver(
        X,
        Y,
        U,
        V,
        color="white",
        pivot="mid",
        alpha=0.8,
    )

    ax.set_xlim(0, 2 * np.pi)
    ax.set_ylim(0, 2 * np.pi)

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title(f"E Field & Charge Density at Time {time:.4f}")

    fig.canvas.draw()
    buf = np.asarray(fig.canvas.buffer_rgba())[..., :3]
    img = Image.fromarray(buf)
    plt.close(fig)

    return img


def _save_gif(frames, out_path, fps):
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


def plot_E(
    E_data,
    rho_data,
    out_path="output/E.gif",
    fps=10,
    cmap_name="viridis",
    quiver_step=4,
):
    E_data = sorted(E_data, key=lambda x: x[0])
    if not E_data:
        print("No E files found; nothing to plot.")
        return

    rho_lookup = {}
    if rho_data:
        rho_times = np.array([t for t, _ in rho_data])
        rho_arrays = [r for _, r in rho_data]
        for t, _, _ in E_data:
            idx = int(np.argmin(np.abs(rho_times - t)))
            rho_lookup[t] = rho_arrays[idx]

    if rho_lookup:
        all_rho = np.stack(list(rho_lookup.values()))
        rho_cmap = plt.get_cmap(cmap_name)
        rho_norm = Normalize(vmin=float(all_rho.min()), vmax=float(all_rho.max()))

        overlay_out = Path(out_path).with_stem(Path(out_path).stem + "_on_rho")
        overlay_frames = [
            render_E_frame(
                Ex, Ey, rho_lookup[t], t, rho_cmap, rho_norm, quiver_step=quiver_step
            )
            for t, Ex, Ey in E_data
        ]
        _save_gif(overlay_frames, str(overlay_out), fps)
    else:
        print("No rho data available; skipping overlay GIF.")


def plot_rho(rho_data, out_path="output/rho.gif", fps=10, cmap_name="viridis"):
    rho_data = sorted(rho_data, key=lambda x: x[0])
    if not rho_data:
        print("No rho files found; nothing to plot.")
        return

    all_rho = np.stack([r for _, r in rho_data])
    vmin, vmax = float(all_rho.min()), float(all_rho.max())

    cmap = plt.get_cmap(cmap_name)
    norm = Normalize(vmin=vmin, vmax=vmax)

    frames = [render_rho_frame(rho, t, cmap, norm) for t, rho in rho_data]

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
    Nx, Ny, Nvx, Nvy, Lx, Ly, Lvx, Lvy = load_mesh()
    rho_data = load_rho_data(Nx, Ny)
    plot_rho(rho_data)

    E_data = load_E_data(Nx, Ny)
    plot_E(E_data, rho_data)
