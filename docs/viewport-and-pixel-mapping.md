# From Pixels to Rays: the Viewport, Step by Step

> These notes explain how a ray tracer decides **which direction to shoot a ray for each pixel** of the window. They cover where the "viewport" goes relative to the camera, how big it is, and how to turn a pixel number into a 3D direction, then back again. Every step is shown, and nothing is assumed beyond basic arithmetic and square roots. Nothing here is code: it's the geometry you need before writing any.

**Written:** 2026-09-17
**Builds on:** `lecture-notes-ray-tracing-fundamentals-for-minirt.md` (ray equation, sphere intersection)

---

## Table of contents

1. [The goal: a flat red disc](#1-the-goal-a-flat-red-disc)
2. [Where the viewport goes](#2-where-the-viewport-goes)
3. [Why the viewport's distance doesn't matter](#3-why-the-viewports-distance-doesnt-matter)
4. [The viewport's width comes from the FOV](#4-the-viewports-width-comes-from-the-fov)
5. [The viewport's height comes from the window's shape](#5-the-viewports-height-comes-from-the-windows-shape)
6. [Two coordinate systems that must be translated](#6-two-coordinate-systems-that-must-be-translated)
7. [Pixel → viewport, step 1: aim at the pixel's center](#7-step-1-aim-at-the-pixels-center)
8. [Pixel → viewport, step 2: normalize to a fraction](#8-step-2-normalize-to-a-fraction)
9. [Pixel → viewport, step 3: from fractions to u and v](#9-step-3-from-fractions-to-u-and-v)
10. [Pixel → viewport, step 4: the 3D point and the ray direction](#10-step-4-the-3d-point-and-the-ray-direction)
11. [Going backwards: world → pixel](#11-going-backwards-world--pixel)
12. [Full worked example (do it yourself)](#12-full-worked-example)
13. [Practice exercise with answers](#13-practice-exercise)
14. [Self-check](#14-self-check)
15. [Cheat sheet](#15-cheat-sheet)

---

## 1. The goal: a flat red disc

The target image is a solid red disc on a black background. There's no lighting, so the sphere doesn't look 3D yet.

```
   +--------------------------------------+
   |                                      |
   |              ████████                |
   |           ██████████████             |
   |          ████████████████            |
   |           ██████████████             |
   |              ████████                |
   |                                      |
   +--------------------------------------+
            1920 × 1080 window
```

The recipe for every pixel:

1. Work out **which direction** the ray for this pixel travels. *(These notes.)*
2. Put that direction into the ray equation `P(t) = O + t·D`, substitute it into the sphere equation, and solve the quadratic for `t`. *(Already known.)*
3. If there's a solution, paint the pixel **red**. Otherwise paint it **black**.

The only missing piece is step 1: **how do you get `D` from a pixel?** That's what the **viewport** is for.

---

## 2. Where the viewport goes

### What the viewport is

Imagine holding an empty picture frame in front of your eye. Everything you see *through* the frame is what ends up in the image. The **viewport** is that frame: an imaginary flat rectangle floating in the 3D scene, in front of the camera.

The window's pixels are laid out as a grid **on** that rectangle. For each pixel, you shoot a ray that starts at the camera and passes through that pixel's spot on the viewport.

```
                         viewport (the "frame")
                        +---+---+---+---+
                        |   |   |   |   |
     camera  ●━━━━━━━━━━━━━━━━━━━━●━━━━━━━━━━━━━━━━━━━━►  ray for one pixel
                        |   |   |   |   |
                        +---+---+---+---+
                         each cell = one pixel
```

### Its three placement rules

The viewport is:

- **perpendicular** to the direction the camera looks: it faces the camera squarely, not at an angle,
- **centered** on that direction: the camera's line of sight hits the viewport's exact middle,
- at some **distance `d`** in front of the camera.

Seen from above:

```
                     viewport (seen edge-on, as a line)
                  |<------- width ------->|
                  +-----------+-----------+
                   \          |          /
                    \         |         /
                     \        | d      /
                      \       |       /
                       \ FOV/2|FOV/2 /
                        \     |     /
                         \    |    /
                          \   |   /
                           \  |  /
                            [cam]
```

The dashed middle line is the camera's line of sight. The two slanted lines are the left and right edges of what the camera can see. The angle between them is the **field of view (FOV)**.

That leaves two questions: **what is `d`?** and **how wide is the viewport?**

---

## 3. Why the viewport's distance doesn't matter

A ray only cares about its **direction**, meaning the angle it leaves the camera at. The spot where it crosses the viewport is just a tool for finding that angle.

Now move the viewport twice as far away **and** make it twice as wide:

```
     d = 1, width = 2                  d = 2, width = 4

      +-----+-----+                  +-----------+-----------+
       \    |    /                    \          |          /
        \   |   /                      \         |         /
         \  |  /                        \        |        /
          \ | /                          \       |       /
           cam                            \      |      /
                                           \     |     /
                                            \    |    /
                                             \   |   /
                                              \  |  /
                                                cam
```

Both drawings have the **same shape**, only at a different scale. Geometry calls these **similar triangles**. A ray aimed at "the right edge" of the small viewport leaves the camera at exactly the same angle as a ray aimed at "the right edge" of the big one, and the same goes for every other point.

So the distance is a **free choice**, as long as the width grows with it. Everyone picks the simplest number:

```
d = 1
```

---

## 4. The viewport's width comes from the FOV

With `d = 1` fixed, the width is **no longer** a free choice. It has to match the FOV given in the scene file (the last number on the `C` line).

### Cutting the view in half

Cut the view in half down the line of sight. You get a **right triangle** (a triangle with one 90° corner):

```
          width/2
       +-----------+
       |          /
       |         /
     d |        /
       |       /
       | FOV/2/
       |     /
       |    /
       |   /
       |  /
       | /
      cam
```

- The side along the line of sight has length `d`.
- The side along the viewport has length `width / 2`.
- The angle at the camera is `FOV / 2`.

### The tangent

In a right triangle, **tangent** (`tan`) is the name for this ratio:

```
tan(angle) = (side opposite the angle) / (side next to the angle)
```

Here the opposite side is `width / 2` and the side next to the angle is `d`:

```
tan(FOV / 2) = (width / 2) / d
```

With `d = 1`, the division by `d` disappears:

```
tan(FOV / 2) = width / 2
```

Multiply both sides by 2:

```
width = 2 · tan(FOV / 2)
```

### Degrees vs radians

The scene file gives the FOV in **degrees** (0 to 180). The C `tan` function expects **radians**, a different unit for angles. The conversion is:

```
radians = degrees × π / 180
```

### Examples

| FOV | FOV / 2 | tan(FOV / 2) | width |
|---|---|---|---|
| 90° | 45° | 1.0000 | **2.0000** |
| 70° | 35° | 0.7002 | **1.4004** |
| 30° | 15° | 0.2679 | **0.5359** |

### What a smaller FOV does

With a smaller FOV the viewport shrinks, but the sphere's outline on it stays the same size, since the sphere, the camera and `d` haven't moved. The same 1920 pixels now cover a smaller area, so more of them fall inside the sphere's outline. **The disc looks bigger on screen.** This is how zooming works.

---

## 5. The viewport's height comes from the window's shape

### The rule: every pixel covers a square patch

The window is a grid of 1920 columns by 1080 rows. Each pixel's ray passes through a small patch of the viewport. Screen pixels are square, so those patches must be **square** too. Otherwise the image gets stretched.

### Following the numbers (FOV 90°, so width = 2)

**Horizontally:** a width of 2 is shared by 1920 columns.

```
one patch's width = 2 / 1920 ≈ 0.00104
```

**Vertically:** for square patches, each patch must also be 0.00104 tall. There are 1080 rows of them:

```
height = 1080 × (2 / 1920)
```

Reorder the same multiplication:

```
height = 2 × (1080 / 1920)
       = width × (Y_MAX / X_MAX)
       = 2 × 0.5625
       = 1.125
```

So `width × (Y_MAX / X_MAX)` is just "keep the patches square", written in one line. In other words, the viewport has the **same shape (aspect ratio)** as the window.

### What goes wrong if you skip it

Suppose you chose `height = width = 2`:

```
one patch's height = 2 / 1080 ≈ 0.00185      (taller than 0.00104 wide)
```

Every row now covers more of the scene vertically than every column covers horizontally. The sphere fits into fewer rows than columns, so it gets squashed:

```
   correct (h = 1.125)        wrong (h = 2)
         ████
       ████████                 ██████████
       ████████                ████████████
       ████████                 ██████████
         ████
       a circle                an ellipse
```

---

## 6. Two coordinate systems that must be translated

A **coordinate system** is a way of naming positions with numbers. A pixel position and a viewport position use two different ones:

```
  WINDOW (pixels)                         VIEWPORT (world units)

  (0,0)                                              +v (up)
    +---------------------------+ → x         +------+------+
    |                           |             |      |      |
    |                           |             |------O------|──► +u (right)
    |                           |             |      |      |
    +---------------------------+             +------+------+
    ↓ y                  (1919,1079)
                                          u goes from −w/2 (left) to +w/2 (right)
  x goes from 0 to 1919                   v goes from +h/2 (top) to −h/2 (bottom)
  y goes from 0 to 1079                   (0, 0) is the CENTER
  (0, 0) is the TOP-LEFT corner
```

(`w` = viewport width and `h` = viewport height, from sections 4 and 5.)

They differ in three ways, and the translation has to fix all three:

| | Window | Viewport |
|---|---|---|
| **Where zero is** | top-left corner | center |
| **Units** | whole pixels (0 … 1919) | world units (−w/2 … +w/2) |
| **Vertical direction** | y grows **downward** | v grows **upward** |

The translation happens in four steps (sections 7 to 10).

---

## 7. Step 1: aim at the pixel's center

A pixel isn't a dot. It's a little square. Pixel column `x` covers everything from `x` to `x + 1`:

```
    pixel 0     pixel 1     pixel 2
  |-----------|-----------|-----------|
  0     ↑     1     ↑     2     ↑     3
       0.5         1.5         2.5
     center      center      center
```

If you used `x` directly, every ray would pass through the pixel's **left edge**, and the whole image would be shifted by half a pixel. Adding `0.5` aims at the middle:

```
horizontal center of pixel x = x + 0.5
vertical   center of pixel y = y + 0.5
```

---

## 8. Step 2: normalize to a fraction

### What "normalize" means here

To **normalize** a position means to express it as a **fraction of the whole** instead of a raw count. You do it by dividing by the total.

Everyday example: page 150 of a 300-page book is 150 ÷ 300 = **0.5**, meaning "halfway through". The result is always between **0** (the start) and **1** (the end), whatever the size of the book.

> ⚠️ The word "normalize" appears again in step 4 with a **different** meaning. There it means "make an arrow's length equal to 1". Don't mix them up.

### Why it's needed

The window and the viewport are two rulers measuring the same span in different units:

```
Window:    0 ──────────────────── 960 ──────────────────── 1920   (pixels)
Viewport: −1 ───────────────────── 0 ───────────────────── +1     (world units, w = 2)
```

You can't copy 960 onto the viewport: that would be far outside a rectangle that's only 2 wide. But **fractions work the same on both rulers**: 0.5 is "halfway" on each of them. So the fraction is the shared language:

```
pixel number  ──(step 2: ÷ total)──►  fraction 0…1  ──(step 3)──►  viewport position
```

### The formulas

```
s = (x + 0.5) / X_MAX        "how far ACROSS, 0 = left,  1 = right"
t = (y + 0.5) / Y_MAX        "how far DOWN,   0 = top,   1 = bottom"
```

`s` and `t` are just names for these two fractions.

### Examples (X_MAX = 1920)

| column x | x + 0.5 | ÷ 1920 = s | meaning |
|---|---|---|---|
| 0 | 0.5 | 0.00026 | almost at the left edge |
| 959 | 959.5 | 0.49974 | just left of the middle |
| 960 | 960.5 | 0.50026 | just right of the middle |
| 1919 | 1919.5 | 0.99974 | almost at the right edge |

Since we aim at pixel centers, `s` is never exactly 0 or 1.

---

## 9. Step 3: from fractions to u and v

### What u and v are

**u** and **v** are the two coordinates of a point **on the viewport**, measured in world units **from the viewport's center**:

- **u**: how far **right** (+) or **left** (−) of the center.
- **v**: how far **up** (+) or **down** (−) of the center.

```
                    v
                    ▲
         +----------+----------+   ← top edge:    v = +h/2
         |          |          |
         |          |          |
    ─────+──────────O──────────+────► u
         |       (0, 0)        |
         |          |          |
         +----------+----------+   ← bottom edge: v = −h/2
         ↑                     ↑
    left edge:            right edge:
    u = −w/2              u = +w/2
```

**Why not call them x and y?** Those names are already used for the pixel's column and row. Graphics programmers conventionally use **u, v** for positions on a flat surface, so the two sets of numbers don't get confused.

### Building u: two operations

We have `s` (0 = left, 1 = right). We want `u` (−w/2 = left, +w/2 = right).

**Operation 1: stretch (multiply by w).** A fraction of the width, times the width, gives a distance:

```
s · w        → goes from 0 to w     (distance from the LEFT edge)
```

**Operation 2: shift (subtract w/2).** We want distances from the **center**, and the center is half a width from the left edge:

```
u = s · w − w/2      → goes from −w/2 to +w/2
```

```
 after ×w:    0 ─────────────── w/2 ─────────────── w
                                 │
 after −w/2: −w/2 ────────────── 0 ─────────────── +w/2
```

Check with w = 2 (using s = 0, 0.5 and 1 to show the edges and center clearly):

| s | s · 2 | − 1 | **u** | where |
|---|---|---|---|---|
| 0 | 0 | −1 | **−1** | left edge |
| 0.5 | 1 | −1 | **0** | center |
| 1 | 2 | −1 | **+1** | right edge |

### Building v: same idea, plus a flip

`t` goes from 0 at the **top** to 1 at the **bottom**. `v` must be **positive at the top** and negative at the bottom. The two point in opposite directions.

**Operation 1: stretch (multiply by h).**

```
t · h        → the distance DOWN from the top edge (0 to h)
```

**Operation 2: start at the top edge and go down.** The top edge is at `v = +h/2`. Going down makes the value smaller, so **subtract**:

```
v = h/2 − t · h
```

```
 pixel rows:   t = 0 (top) ────────────────────────► t = 1 (bottom)
 viewport:     v = +h/2    ────────── 0 ──────────►  v = −h/2
```

Check with h = 1.125:

| t | t · 1.125 | 0.5625 − that | **v** | where |
|---|---|---|---|---|
| 0 | 0 | 0.5625 − 0 | **+0.5625** | top edge |
| 0.5 | 0.5625 | 0.5625 − 0.5625 | **0** | center |
| 1 | 1.125 | 0.5625 − 1.125 | **−0.5625** | bottom edge |

Writing it as `h/2 − …` rather than `… − h/2` is what performs the flip.

---

## 10. Step 4: the 3D point and the ray direction

Step 4 has three parts:
- **(A)** place the 2D point (u, v) in the 3D world,
- **(B)** turn it into an arrow from the camera,
- **(C)** give that arrow a length of 1.

### A. From (u, v) to a 3D point P

A point in 3D needs **three** numbers `(x, y, z)`:

- **x**: right (+) / left (−)
- **y**: up (+) / down (−)
- **z**: forward (+) / backward (−)

> The x and y here are **world** coordinates, not pixel numbers. From this section on, "x, y, z" always means world coordinates.

**Simplifying assumption for these notes:** the camera sits at the origin `O = (0, 0, 0)` and looks straight along **+z**, with world +x to its right and world +y above it. (A tilted camera needs one more step, not covered here.)

The viewport is at distance `d = 1`, so it sits in the "wall" where `z = 1`:

```
  side view (looking from the camera's right)

        y ▲
          |            viewport
          |               |  ← top edge     (y = +h/2)
          |               |
  O ●─────┼───────────────●───────────────►  z
 camera   |               | (0, 0, 1) = viewport center
          |               |  ← bottom edge  (y = −h/2)
          |               |
          0               1
```

Because the viewport faces the camera squarely:
- the viewport's "right" is world **+x**, so `u` becomes the **x** coordinate,
- the viewport's "up" is world **+y**, so `v` becomes the **y** coordinate,
- every viewport point is 1 unit forward, so the **z** coordinate is `1`.

```
P = (u, v, 1)
```

### B. From the point P to an arrow

The ray equation needs **D**, a **direction**. A direction is an **arrow**, which mathematicians call a **vector**. It's also written as three numbers, but they mean "move this much in x, this much in y, this much in z".

The arrow that goes **from O to P** is P minus O, one coordinate at a time:

```
P − O = (u − 0,  v − 0,  1 − 0) = (u, v, 1)
```

```
          P = (u, v, 1)
          ●
         ↗
        /   the arrow P − O
       /
      ●
   O = (0,0,0)
```

With the camera at the origin, the subtraction changes nothing. (With a camera somewhere else, it would.)

### C. Normalizing the arrow (the second meaning)

Here, **normalize** means: *keep the arrow pointing the same way, but make its length exactly 1.* An arrow of length 1 is called a **unit vector**.

#### C1. How long is an arrow?

**In 2D**, the arrow `(3, 4)` is the slanted side of a right triangle whose other sides are 3 and 4:

```
              ● (3, 4)
            ╱ |
     len  ╱   |
        ╱     | 4
      ╱       |
    ●─────────┘
  (0,0)   3
```

**Pythagoras' theorem** says that in a right triangle, (slanted side)² = (side 1)² + (side 2)²:

```
len = √(3² + 4²) = √(9 + 16) = √25 = 5
```

**In 3D**, add one more term under the square root:

```
length of (a, b, c) = √(a² + b² + c²)
```

#### C2. Shrinking the arrow to length 1

Divide **each** of the three numbers by the length. All three shrink by the same factor, so the direction stays the same:

```
D = ( a / len,  b / len,  c / len )
```

2D check: `(3, 4) ÷ 5 = (0.6, 0.8)`, and `√(0.6² + 0.8²) = √(0.36 + 0.64) = √1 = 1` ✓

#### C3. Why bother?

With a length-1 direction, **`t` measures real distance**: `t = 10` means "10 world units from the camera". When a ray hits several objects, you keep the one with the smallest `t`, and that comparison is only a distance comparison if D has length 1. For a single flat disc it doesn't change which pixels are red, but it's standard practice and lighting will depend on it later.

### The complete pixel → ray chain

```
 pixel (x, y)
     │  step 1: + 0.5
     ▼
 (x + 0.5, y + 0.5)
     │  step 2: ÷ X_MAX, ÷ Y_MAX
     ▼
 fractions (s, t)          0 … 1
     │  step 3: u = s·w − w/2,   v = h/2 − t·h
     ▼
 viewport point (u, v)     world units, centered
     │  step 4A: put it at z = 1
     ▼
 P = (u, v, 1)
     │  step 4B: P − O
     ▼
 arrow (u, v, 1)
     │  step 4C: ÷ length
     ▼
 D  (length 1)  ──►  P(t) = O + t·D  ──►  sphere quadratic  ──►  red or black
```

---

## 11. Going backwards: world → pixel

Sometimes you want the opposite: **given a point somewhere in the 3D scene, which pixel does it appear on?** (Useful for checking your work, e.g. "the sphere's center should land in the middle of the screen".)

The idea: **undo each step, in reverse order.** It's like getting undressed: the last thing you put on comes off first.

### R1. From a 3D point Q to the viewport (undo step 4)

Take any world point `Q = (Qx, Qy, Qz)` in front of the camera. The ray from the camera to Q crosses the viewport (at z = 1) somewhere. Where?

Side view:

```
        y ▲
          |                           ● Q = (…, Qy, Qz)
          |                       ╱   |
          |                   ╱       |
          |          v ●  ╱           | Qy
          |          ╱|               |
          |      ╱    | v             |
  O ●─────────────────┼───────────────┼────► z
          0           1               Qz
```

The small triangle (sides 1 and v) and the big triangle (sides Qz and Qy) are **similar**: same angle at O, different size. In similar triangles the ratios of the sides are equal:

```
v / 1 = Qy / Qz        →      v = Qy / Qz
```

The top view gives the same thing for the horizontal direction:

```
u = Qx / Qz
```

In words: **divide by the depth**. A point twice as far away lands half as far from the center, which is why distant things look small.

> ⚠️ This only works if `Qz > 0` (the point is in front of the camera). If `Qz ≤ 0` the point is beside or behind the camera and doesn't appear on screen.

### R2. From (u, v) to fractions (undo step 3)

Step 3 did "stretch, then shift". Undo it as "un-shift, then un-stretch".

For **u**: step 3 was `u = s·w − w/2`.

```
add w/2 to both sides:        u + w/2 = s·w
divide both sides by w:       s = (u + w/2) / w
```

For **v**: step 3 was `v = h/2 − t·h`.

```
add t·h to both sides:        v + t·h = h/2
subtract v from both sides:   t·h = h/2 − v
divide both sides by h:       t = (h/2 − v) / h
```

> If `s` or `t` ends up **below 0 or above 1**, the point is outside the viewport, so it's off-screen.

### R3. From fractions to a pixel (undo steps 2 and 1)

Step 2 divided by the total, so multiply back:

```
s · X_MAX      and      t · Y_MAX
```

This gives a position **in pixel units**, usually with decimals, like `1302.857`. Which pixel is that? Pixel column `x` covers the span from `x` up to (but not including) `x + 1` (see section 7). So you **drop the decimals** (round down, called **floor**):

```
pixel column = floor(s · X_MAX)
pixel row    = floor(t · Y_MAX)
```

```
   ... | pixel 1302  | pixel 1303 | ...
       |-------------|------------|
     1302    ↑     1303         1304
          1302.857  → lands in pixel 1302
```

(Rounding down undoes the `+ 0.5` of step 1. Any position inside a pixel's square belongs to that pixel, not just its exact center.)

### The complete world → pixel chain

```
 world point Q = (Qx, Qy, Qz)
     │  R1: u = Qx / Qz,   v = Qy / Qz
     ▼
 viewport point (u, v)
     │  R2: s = (u + w/2) / w,   t = (h/2 − v) / h
     ▼
 fractions (s, t)          outside 0…1 → off-screen
     │  R3: × X_MAX, × Y_MAX, then round down
     ▼
 pixel (column, row)
```

---

## 12. Full worked example

Do this with pencil, paper and a calculator. Cover the answers and check yourself after each line.

### Setup

- Window: `X_MAX = 1920`, `Y_MAX = 1080`
- Camera: at `O = (0, 0, 0)`, looking along +z
- FOV: **90°**
- Pixel to convert: **column 1440, row 270** (three-quarters across, one-quarter down)

### Part 0: viewport size

```
FOV / 2         = 45°
tan(45°)        = 1
w = 2 · 1       = 2
h = 2 · 1080 / 1920 = 2 · 0.5625 = 1.125
w / 2 = 1       h / 2 = 0.5625
```

### Part 1: pixel → ray direction

**Step 1: center**

```
x + 0.5 = 1440.5
y + 0.5 =  270.5
```

**Step 2: fractions**

```
s = 1440.5 / 1920 = 0.750260
t =  270.5 / 1080 = 0.250463
```

Sanity check: s ≈ 0.75 (three-quarters across) ✓, t ≈ 0.25 (one-quarter down) ✓

**Step 3: viewport coordinates**

```
u = s · w − w/2 = 0.750260 · 2 − 1           = 1.500521 − 1      = 0.500521
v = h/2 − t · h = 0.5625 − 0.250463 · 1.125  = 0.5625 − 0.281771 = 0.280729
```

Sanity check: u > 0 means right of center ✓. v > 0 means above center ✓ (the pixel is in the top half).

**Step 4A/4B: point and arrow**

```
P = (0.500521, 0.280729, 1)
arrow = P − (0, 0, 0) = (0.500521, 0.280729, 1)
```

**Step 4C: length**

```
0.500521² = 0.250521
0.280729² = 0.078809
1²        = 1.000000
            ────────
sum       = 1.329330

length = √1.329330 = 1.152966
```

**Step 4C: divide**

```
D = (0.500521 / 1.152966,  0.280729 / 1.152966,  1 / 1.152966)
  = (0.434116,  0.243484,  0.867328)
```

**Verify the length is 1:**

```
0.434116² + 0.243484² + 0.867328²
= 0.188457 + 0.059284 + 0.752258
= 0.999999 ≈ 1  ✓
```

**Result:** the ray for pixel (1440, 270) travels in direction **D ≈ (0.4341, 0.2435, 0.8673)**: to the right, slightly up, and mostly forward.

### Part 2: world → pixel (the round trip)

Walk 10 units along that ray to get a point in the world:

```
Q = O + 10 · D = (4.34116, 2.43484, 8.67328)
```

Now forget where Q came from, and find its pixel. If everything is right, you should get back to **(1440, 270)**.

**R1: onto the viewport**

```
u = Qx / Qz = 4.34116 / 8.67328 = 0.500521
v = Qy / Qz = 2.43484 / 8.67328 = 0.280729
```

Same u and v as in Part 1 ✓. (Dividing by depth cancels out how far along the ray we walked.)

**R2: back to fractions**

```
s = (u + w/2) / w = (0.500521 + 1) / 2         = 1.500521 / 2     = 0.750260
t = (h/2 − v) / h = (0.5625 − 0.280729) / 1.125 = 0.281771 / 1.125 = 0.250463
```

Both are between 0 and 1, so the point is on screen ✓

**R3: back to pixels**

```
s · 1920 = 1440.5    → floor → 1440
t · 1080 =  270.5    → floor →  270
```

**Result: pixel (1440, 270).** The round trip closes ✓. Notice that `1440.5` is exactly the "pixel center" value from step 1. The reverse trip landed on the center, just as the forward trip started from it.

### Part 3: a quick bonus (sphere center)

A sphere centered at `Q = (0, 0, 10)`: which pixel shows its center?

```
R1:  u = 0 / 10 = 0          v = 0 / 10 = 0
R2:  s = (0 + 1) / 2 = 0.5   t = (0.5625 − 0) / 1.125 = 0.5
R3:  0.5 · 1920 = 960.0 → 960        0.5 · 1080 = 540.0 → 540
```

The center lands **exactly on the corner** shared by pixels 959/960 and 539/540. There's no single middle pixel, because 1920 and 1080 are even numbers. Rounding down assigns the corner to pixel (960, 540).

---

## 13. Practice exercise

Same setup as section 12 (1920 × 1080, camera at origin looking along +z, FOV 90°).

**Exercise A (pixel → ray):** find the direction D for pixel **(480, 810)**.

**Exercise B (world → pixel):** which pixel shows the world point **Q = (2.5, −1.3, 7)**?

<details>
<summary>Answers A</summary>

```
centers:  480.5,  810.5
s = 480.5 / 1920 = 0.250260
t = 810.5 / 1080 = 0.750463
u = 0.250260 · 2 − 1           = −0.499479
v = 0.5625 − 0.750463 · 1.125  = 0.5625 − 0.844271 = −0.281771
arrow = (−0.499479, −0.281771, 1)
length² = 0.249479 + 0.079395 + 1 = 1.328874
length  = 1.152768
D = (−0.433287, −0.244430, 0.867477)
check: 0.187738 + 0.059746 + 0.752516 ≈ 1 ✓
```

Left and down, which makes sense for a pixel in the bottom-left quarter. It's the mirror image of section 12's pixel.

</details>

<details>
<summary>Answers B</summary>

```
R1: u = 2.5 / 7  = 0.357143
    v = −1.3 / 7 = −0.185714
R2: s = (0.357143 + 1) / 2          = 0.678571
    t = (0.5625 − (−0.185714)) / 1.125
      = (0.5625 + 0.185714) / 1.125  = 0.748214 / 1.125 = 0.665079
R3: 0.678571 · 1920 = 1302.857 → 1302
    0.665079 · 1080 =  718.286 →  718
```

**Pixel (1302, 718)**: right of center and below center. Watch the sign in `h/2 − v`: v is negative, so subtracting it **adds**.

</details>

**Make your own:** pick any pixel, run Part 1, walk any distance along D, and run Part 2. If you don't land back on your pixel, one of the steps has a mistake. Also try the same pixel with FOV 60° (`tan(30°) ≈ 0.57735`) and see how D changes.

---

## 14. Self-check

<details>
<summary>1. Is the viewport's distance from the camera arbitrary?</summary>

Yes, the distance is. Moving the viewport while scaling its size by the same factor gives the same ray directions (similar triangles), so `d = 1` is chosen for simplicity. Once `d` is fixed, the width is **not** arbitrary: `w = 2 · tan(FOV / 2)`.

</details>

<details>
<summary>2. You lower the FOV from 70° to 30°. Does the disc get bigger or smaller on screen?</summary>

Bigger. The viewport shrinks (1.40 → 0.54 wide) while the sphere's outline on it stays the same size, so the same 1920 pixels are packed into a smaller area and more of them hit the sphere.

</details>

<details>
<summary>3. What happens if you set the viewport's height equal to its width?</summary>

Each pixel covers a patch taller than it is wide. The sphere spans fewer rows than columns, so it looks squashed into an ellipse.

</details>

<details>
<summary>4. Why is the "center" pixel (960, 540) at u ≈ +0.0005, v ≈ −0.0005 instead of exactly (0, 0)?</summary>

1920 and 1080 are even, so there's no single middle pixel. The true center is the corner between four pixels. Pixel (960, 540) is the one just right of and just below that corner, and its center is half a pixel away from it.

</details>

<details>
<summary>5. For that center pixel the arrow is about (0.0005, −0.0005, 1). What does normalizing do?</summary>

Almost nothing. Its length is `√(0.00000027 + 0.00000027 + 1) ≈ 1.00000027`, which is already about 1. Dividing by it barely changes the numbers. The rays near the center are almost exactly along +z. Normalization matters more toward the corners, where the length reaches about 1.52.

</details>

<details>
<summary>6. What are the two different meanings of "normalize" in these notes?</summary>

(1) In step 2: turn a count into a **fraction from 0 to 1** by dividing by the total. (2) In step 4C: turn an arrow into one of **length 1** by dividing each coordinate by its length.

</details>

---

## 15. Cheat sheet

```
VIEWPORT SIZE
  d = 1
  w = 2 · tan(FOV/2)            (FOV converted to radians)
  h = w · Y_MAX / X_MAX

PIXEL → RAY
  s = (x + 0.5) / X_MAX
  t = (y + 0.5) / Y_MAX
  u = s·w − w/2
  v = h/2 − t·h
  P = (u, v, 1)
  D = P / √(u² + v² + 1)

WORLD → PIXEL   (camera at origin looking along +z, and Qz > 0)
  u = Qx / Qz
  v = Qy / Qz
  s = (u + w/2) / w
  t = (h/2 − v) / h             (outside 0…1 → off-screen)
  column = floor(s · X_MAX)
  row    = floor(t · Y_MAX)
```
