*This project has been created as part of the 42 curriculum by jchartie, bribot.*

# miniRT

## Description

**miniRT** is a small ray tracer written in C  on top of the MiniLibX graphics library. The program reads a scene description file (`.rt`) and renders the scene in a window, as seen from a camera placed anywhere in 3D space.

For every pixel of the window, a ray is cast from the camera through that pixel, the closest object it hits is found, and the pixel colour is computed from the lighting at the hit point.

### Features

- **Objects:** sphere, plane and cylinder (with caps), including correct handling of intersections and of the inside of objects.
- **Resizing:** sphere diameter, cylinder diameter and height.
- **Transformations:** translation and rotation of objects and camera (spheres and lights are not rotated, as they have no orientation).
- **Lighting:** ambient lighting (objects are never fully dark), diffuse lighting, adjustable spot brightness and hard shadows.
- **Window management:** `ESC` or the window's red cross closes the window and quits cleanly, with all heap memory freed.
- **Parsing:** strict validation of the scene file. Any misconfiguration makes the program print `Error` followed by an explicit message and exit properly.

## Instructions

### Requirements

- `cc`, `make`
- **Linux:** X11 development packages (`libxext-dev`, `libx11-dev`) and `zlib` (`-lm -lz` are linked)
- **macOS:** OpenGL / AppKit frameworks (the Makefile picks `lib/mlx_macos` automatically)

MiniLibX and libft are bundled in `lib/`; the Makefile builds them for you.

### Compilation

```bash
make        # build ./miniRT
make clean  # remove object files
make fclean # remove objects, libraries and the executable
make re     # full rebuild
```

The project is compiled with `-Wall -Wextra -Werror`.

### Execution

```bash
./miniRT scenes/shadows.rt
```

The program takes exactly one argument: a scene file with the `.rt` extension. Press `ESC` or click the window's close button to quit.

### Scene file format

Each element is on its own line (blank lines allowed). Values of one element are separated by spaces; the components of a vector or colour are separated by commas. Elements can appear in any order. `A`, `C` and `L` (capital letters) can only be declared once.

| Element | Identifier | Parameters |
|---|---|---|
| Ambient light | `A` | ratio `[0.0, 1.0]`, colour `R,G,B` `[0-255]` |
| Camera | `C` | position `x,y,z`, normalized orientation `x,y,z` `[-1,1]`, horizontal FOV `[0,180]` |
| Light | `L` | position `x,y,z`, brightness `[0.0, 1.0]`, colour `R,G,B` (unused in the mandatory part) |
| Sphere | `sp` | center `x,y,z`, diameter, colour `R,G,B` |
| Plane | `pl` | point `x,y,z`, normalized normal `x,y,z` `[-1,1]`, colour `R,G,B` |
| Cylinder | `cy` | center `x,y,z`, normalized axis `x,y,z` `[-1,1]`, diameter, height, colour `R,G,B` |

Example (`scenes/shadows.rt`):

```
A 0.1 255,255,255
C 0,40,-100 0,-0.20,0.97980 70
L -60,80,0 0.9 255,255,255
pl 0,0,0 0,1,0 200,200,200
sp 0,20,40 40 255,0,0
cy 50,25,40 0,1,0 20 50 0,0,255
sp -70,10,80 20 0,255,0
```

### Provided test scenes

The `scenes/` folder contains scenes that each target one edge case: camera inside a sphere or a tube, camera looking down, object or plane behind the camera, light inside a sphere or behind an object, tangent and tiny objects, intersecting objects, tilted cylinders, occlusion and shadows, and a multi-object scene. For example:

```bash
./miniRT scenes/camera_inside_sphere.rt
./miniRT scenes/cylinder_tilted.rt
```

### Project structure

```
include/miniRT.h   main header
src/main.c         entry point
src/parser/        file reading, scene initialisation, viewport setup
src/checker/       validation of every line of the .rt file
src/solver_eq/     ray intersections (sphere, plane, cylinder, quadratic solver)
src/engine/        rendering loop, shading, colours, MLX window and hooks
src/utils/         vector operations
lib/               libft, get_next_line, MiniLibX (Linux and macOS)
scenes/            test scenes
docs/              personal study notes (ray tracing, viewport, MiniLibX)
```

## Resources

### References

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html), Peter Shirley
- [Scratchapixel](https://www.scratchapixel.com/): ray-sphere, ray-plane, ray-cylinder intersections, shading and cameras
- [Ray tracing (graphics)](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) and [Phong reflection model](https://en.wikipedia.org/wiki/Phong_reflection_model), Wikipedia
- [MiniLibX documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- `man 3 math`, `man 2 open`, `man 2 read`

### Use of AI

This README was drafted with AI.