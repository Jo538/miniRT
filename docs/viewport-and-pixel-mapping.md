# From Pixels to Rays: the Viewport, Step by Step

> These notes explain how a ray tracer decides **which direction to shoot a ray for each pixel** of the window. They cover where the "viewport" goes relative to the camera, how big it is, and how to turn a pixel number into a 3D direction, then back again. The camera is the one a `.rt` file actually gives you: it sits **anywhere** in the scene and looks **any way**, from the very first section. Every step is shown, and nothing is assumed beyond basic arithmetic and square roots. Nothing here is code: it's the geometry you need before writing any.

**Written:** 2026-09-17 · **Rewritten:** 2026-09-22 (general camera throughout)
**Builds on:** `lecture-notes-ray-tracing-fundamentals-for-minirt.md` (ray equation, sphere intersection)

---

## Table of contents

1. [The goal: a flat red disc](#1-the-goal-a-flat-red-disc)
2. [The camera the scene file gives you](#2-the-camera-the-scene-file-gives-you)
3. [Where the viewport goes](#3-where-the-viewport-goes)
4. [Why the viewport's distance doesn't matter](#4-why-the-viewports-distance-doesnt-matter)
5. [The viewport's width comes from the FOV](#5-the-viewports-width-comes-from-the-fov)
6. [The viewport's height comes from the window's shape](#6-the-viewports-height-comes-from-the-windows-shape)
7. [The camera's three directions: forward, right and up](#7-the-cameras-three-directions-forward-right-and-up)
8. [Two coordinate systems that must be translated](#8-two-coordinate-systems-that-must-be-translated)
9. [Step 1: aim at the pixel's center](#9-step-1-aim-at-the-pixels-center)
10. [Step 2: normalize to a fraction](#10-step-2-normalize-to-a-fraction)
11. [Step 3: from fractions to u and v](#11-step-3-from-fractions-to-u-and-v)
12. [Step 4: from (u, v) to the ray direction D](#12-step-4-from-u-v-to-the-ray-direction-d)
13. [The complete pixel → ray chain](#13-the-complete-pixel--ray-chain)
14. [Going backwards: world → pixel](#14-going-backwards-world--pixel)
15. [Full worked example (do it yourself)](#15-full-worked-example)
16. [Practice exercise with answers](#16-practice-exercise)
17. [Self-check](#17-self-check)
18. [Cheat sheet](#18-cheat-sheet)

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

1. Work out **where the ray starts** and **which direction it travels**. *(These notes.)*
2. Put those into the ray equation `P(t) = O + t·D`, substitute it into the sphere equation, and solve the quadratic for `t`. *(Already known.)*
3. If there's a solution, paint the pixel **red**. Otherwise paint it **black**.

The only missing piece is step 1: **how do you get `O` and `D` from a pixel?** `O` turns out to be free (it's the camera's position, straight from the file). Everything else is what the **viewport** is for.

---

## 2. The camera the scene file gives you

A `.rt` file describes the camera on one line:

```
C   10,0,5   0.6,0,0.8   90
    ───────  ──────────  ──
    position orientation FOV
```

| part | meaning | name used here |
|---|---|---|
| `10,0,5` | where the camera is | **O** |
| `0.6,0,0.8` | which way it looks, as an arrow of length 1 | **forward** |
| `90` | field of view, in degrees | **FOV** |

None of these is fixed in advance: the camera can be anywhere and look any way. So every formula in these notes is built from them.

### Points and arrows

Two kinds of things get written as three numbers, and they mean different things:

- A **point** is a **location**: `(10, 0, 5)` means "10 along x, 0 along y, 5 along z, starting from the origin".
- An **arrow** (mathematicians say **vector**) is a **movement**: `(0.6, 0, 0.8)` means "move 0.6 along x, 0 along y, 0.8 along z". A movement doesn't belong to any particular place; you can make it starting from anywhere.

```
 z
 ▲
 3 ┤                    ●  end (5, 3)
 2 ┤            ●───────┘  start (3, 2)
 1 ┤        ●  end (2, 1)
 0 ┤●───────┘  start (0, 0)
   └┬───┬───┬───┬───┬───┬──► x
    0   1   2   3   4   5
```

Both paths above are the same movement, `(2, 1)`. Only the starting point differs.

To find the movement that takes you **from** one point **to** another, subtract the start from the end, one number at a time:

```
movement = end − start        e.g. (5, 3) − (3, 2) = (2, 1)
```

In the ray equation `P(t) = O + t·D`, **O is a point** (where the ray starts) and **D is an arrow** (the step it repeats).

### A compass for naming directions

To talk about directions without confusing them with directions *in a drawing*, imagine floating above the scene and looking down at the ground:

```
                north (+z)
                    ▲
                    │
   west (−x) ◄──────┼──────► east (+x)
                    │
                    ▼
                south (−z)

   +y points up toward the sky (straight at you, as you look down)
```

Every "seen from above" diagram below uses this compass. Positions and movements seen from above are written as **(x, z)**, leaving out y.

So the camera of the example above sits at (10, 0, 5) and looks in the direction "0.6 east and 0.8 north".

---

## 3. Where the viewport goes

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

### Its placement rules

The viewport is:

- **perpendicular** to `forward`: it faces the camera squarely, not at an angle,
- **centered** on `forward`: the camera's line of sight hits the viewport's exact middle,
- at some **distance `d`** in front of the camera,
- **square with the camera**: its horizontal sides run along the camera's right-hand side, its vertical sides along the camera's top.

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

The middle line is `forward`, the camera's line of sight. The two slanted lines are the left and right edges of what the camera can see. The angle between them is the **field of view (FOV)**.

### The viewport travels with the camera

Nothing above says *where in the world* the viewport is. It is defined **relative to the camera**: always `d` in front, always facing the same way as the camera. Move the camera and the viewport moves with it; turn the camera and it turns with it.

```
        camera at (0, 0)                  same camera moved to (−5, 2)

 z                                 z
 ▲                                 ▲
 1 ┤   ●────────·────────●         3 ┤   ●────────·────────●
   │                                 │
 0 ┤            ●                  2 ┤            ●
   └───┬────────┬────────┬─► x       └───┬────────┬────────┬─► x
      −1        0        1              −6       −5       −4
```

That's why the camera's **position never appears** in the pixel-to-direction steps: those steps measure everything from the camera, and the camera and its viewport always keep the same arrangement. The position appears once, at the very end, as the point where the ray starts.

That leaves three questions: **what is `d`?**, **how big is the viewport?** and **which world directions are the camera's right and up?**

---

## 4. Why the viewport's distance doesn't matter

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

## 5. The viewport's width comes from the FOV

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

## 6. The viewport's height comes from the window's shape

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

## 7. The camera's three directions: forward, right and up

The viewport is a rectangle attached to the camera, so to place a spot on it you need to know which way its sides point **in the world**. That's what these three arrows are for. Each is a movement of length 1:

| name | meaning | where it comes from |
|---|---|---|
| **forward** | where the camera looks | the orientation vector, straight from the file |
| **right** | toward the camera's right-hand side | computed below |
| **up** | toward the camera's top | computed below |

```
      up
       ▲   forward
       │  ↗
       │ ╱
       ●───────► right
     camera
```

They sit at right angles to each other, exactly like the three edges meeting at the corner of a box.

(If the file's orientation vector isn't exactly length 1, divide its three numbers by `√(fx² + fy² + fz²)` before doing anything else.)

### 7.1 The file doesn't fully determine the camera

Point a camera at something and keep it pointing there. You can still **spin it around its own line of sight**. `forward` doesn't change, but the picture comes out rotated. That spin is called **roll**:

```
   no roll                       rolled

  ┌───────────────┐            ┌───────────────┐
  │        ☁      │            │          ☁    │
  │               │            │        ╱      │
  │───────────────│            │      ╱        │
  │    ▲▲▲▲▲      │            │    ╱  ▲▲      │
  └───────────────┘            └───────────────┘
   horizon is level             horizon is slanted
```

"Where the camera looks" and "how the camera is rolled" are two **separate** pieces of information, and the `C` line only gives the first.

Look at all the arrows that stand at a right angle to `forward`. They don't form one direction: they form a whole **disc**, like the spokes of a bicycle wheel whose axle is `forward`:

```
     the disc of arrows perpendicular to forward
          (forward points straight out at you)

                    ↑ A
              ╭─────┼─────╮
              │     │     │
         D ←──┼─────●─────┼──► B          the ● in the middle is the camera,
              │     │     │               forward comes straight toward you
              ╰─────┼─────╯
                    ↓ C

   A, B, C, D and everything in between are all perpendicular to forward.
   Any one of them could be the camera's "up". forward cannot tell you which.
```

Since the file doesn't choose, the renderer must.

### 7.2 The choice: the camera never rolls

Out of that whole disc, we pick the arrangement where the camera's right arm is **horizontal**, i.e. flat on the ground, the way a person holds a camera normally. The horizon then comes out level in the image.

> **"Horizontal" means y = 0**, not "along an axis". `(0.6, 0, −0.8)` is horizontal: it has no sky component even though its x and z are both non-zero. There is a whole flat plane of horizontal arrows to choose from.

Here is a camera **tilted downward**, drawn in a vertical slice of the world. Two symbols are used for arrows that leave the slice:

```
   ⊙ = pointing out of the page, toward you
   ⊗ = pointing into the page, away from you
```

```
   side view: a vertical slice containing north and the sky

      sky ▲
          │
          │   up = (0, 0.8, 0.6)
          │  ↖
          │    ╲
   camera ●─────╲─────────────────────────► north
         ⊙│      ╲
          │       ↘ forward = (0, −0.6, 0.8)
          │
   ground ─────────────────────────────────────

   right = east = ⊙ straight out of this slice, toward you
```

- `forward` and `up` both live **inside** the vertical slice: `up` is the spoke that leans the same way the camera leans.
- `right` sticks out sideways and lies flat: no sky component at all.

Tilting the camera makes the disc lean, but a leaning disc still crosses the horizontal plane, and it crosses it along a line. That line is the one horizontal spoke, and it is `right`:

```
   side view

       sky ▲
           │        ↗ forward, tilted up out of the ground
           │      ╱
           │    ╱
           │  ╱
    camera ●──────────────────────────► (the ground)
          ⊙ ╲
            ╲  the disc, seen edge-on (it leans, because forward leans)
             ╲

   ⊙ = the one spoke with no sky component = right
```

This works for every `forward` except straight up or straight down (section 7.5).

### 7.3 The cross product

Finding an arrow perpendicular to two given arrows is a standard operation: the **cross product**. Given two arrows **A** and **B**, `A × B` is a new arrow **perpendicular to both**:

```
          A × B
            ▲
            │
            │
            ●──────► B
           ╱
         ╱
       A
```

```
A × B = ( Ay·Bz − Az·By ,
          Az·Bx − Ax·Bz ,
          Ax·By − Ay·Bx )
```

Each line uses the *other two* axes, crosswise. A check with the compass (east = (1,0,0), sky = (0,1,0), north = (0,0,1)):

```
east × sky = ( 0·0 − 0·1 ,  0·0 − 1·0 ,  1·1 − 0·0 ) = (0, 0, 1) = north  ✓
```

North really is perpendicular to both east and sky.

> ⚠️ **Order matters.** `B × A` is the same arrow pointing the opposite way: `sky × east = south`. Swapping the operands in the formulas below would give a mirrored image.

### 7.4 The two formulas

The world's up is the reference that breaks the roll tie:

```
worldup = (0, 1, 0)

right = normalize( worldup × forward )
up    = forward × right
```

**Why `right` is always horizontal.** Work out `worldup × forward` for a completely general `forward = (fx, fy, fz)`:

```
worldup × forward = ( 1·fz − 0·fy ,  0·fx − 0·fz ,  0·fy − 1·fx )
                  = ( fz ,  0 ,  −fx )
```

The middle slot is **0 whatever `fy` is**, so the result has no sky component: horizontal by construction. And `fy` doesn't appear at all, which says something useful:

> **`right` only depends on the camera's shadow on the ground.** Tilt the camera up or down as much as you like; its right arm doesn't move.

```
   seen from above

                  ↗ forward's shadow on the ground
                ╱
      camera  ●╱
               ╲
                ╲
                 ↘ right   — the shadow, turned a quarter turn clockwise
```

**Why `right` needs normalizing.** `(fz, 0, −fx)` has length `√(fx² + fz²)`, which is less than 1 when the camera is tilted (the shadow is shorter than the arrow). Dividing by that length fixes it.

**Why `up` doesn't.** `forward` and `right` are both length 1 and perpendicular to each other, so `forward × right` already has length 1.

#### Check 1: a level camera

With `fy = 0`, `right = (fz, 0, −fx)` and its length is already 1, so normalizing changes nothing:

| forward | in words | right | in words |
|---|---|---|---|
| (0, 0, 1) | 1 north | (1, 0, 0) | 1 east ✓ |
| (1, 0, 0) | 1 east | (0, 0, −1) | 1 south ✓ |
| (0.6, 0, 0.8) | mostly north | (0.8, 0, −0.6) | mostly east ✓ |
| (0.8, 0, 0.6) | mostly east | (0.6, 0, −0.8) | mostly south ✓ |

Each line matches the everyday rule "face north, your right arm points east; face east, it points south", i.e. the **next compass direction clockwise**.

Then:

```
up = forward × right = ( 0·(−fx) − fz·0 ,  fz·fz − fx·(−fx) ,  fx·0 − 0·fz )
                     = ( 0,  fz² + fx²,  0 )  =  (0, 1, 0)   ✓
```

A level camera's top points straight at the sky, as it should.

> ⚠️ **Don't compute `right` by just flipping the sign of z.** `(0.6, 0, −0.8)` from `(0.6, 0, 0.8)` is forward reflected in a mirror, not turned. Test that rule on a camera facing north, `(0, 0, 1)`: it gives `(0, 0, −1)`, which points **behind** the camera instead of to its right.

#### Check 2: a tilted camera

`forward = (0, −0.6, 0.8)`: looking north and downward.

```
right = worldup × forward = ( 0.8, 0, 0 )
        length 0.8  →  normalize  →  (1, 0, 0) = east      ✓ still horizontal

up = forward × right = ( (−0.6)·0 − 0.8·0 ,  0.8·1 − 0·0 ,  0·0 − (−0.6)·1 )
                     = ( 0, 0.8, 0.6 )
        length √(0.64 + 0.36) = 1  ✓
```

The camera looks down toward the north, so its top leans backward, to the south-and-up side ✓

#### Check 3: tilted and turned

`forward = (0.8, 0.3, 0.6)` (not quite length 1, but it shows the shape of the computation):

```
right = (fz, 0, −fx) = (0.6, 0, −0.8)          length √(0.36 + 0.64) = 1

forward · right = 0.8·0.6 + 0.3·0 + 0.6·(−0.8) = 0  ✓ perpendicular

up = forward × right = ( 0.3·(−0.8) − 0.6·0 ,  0.6·0.6 − 0.8·(−0.8) ,  0.8·0 − 0.3·0.6 )
                     = ( −0.24, 1.00, −0.18 )
```

`right` has a non-zero x **and** z, lies along no axis, and is still horizontal because its y is 0. `up` is mostly sky, leaning slightly west and south: the camera looks up toward the east-north, so its top tips backward ✓

### 7.5 The one case that breaks

If `forward` is **exactly** straight up or straight down, `(0, 1, 0)` or `(0, −1, 0)`:

```
worldup × forward = (0, 0, 0)
```

Two parallel arrows have no perpendicular direction to single out, so the cross product collapses and normalizing would divide by zero. The camera's roll genuinely is undefined there: looking straight down, "which way is the top of the image?" has no answer from `forward` alone. Detect this case and fall back on a different reference arrow, e.g. use `(0, 0, 1)` in place of `worldup`.

---

## 8. Two coordinate systems that must be translated

A **coordinate system** is a way of naming positions with numbers. A pixel position and a viewport position use two different ones:

```
  WINDOW (pixels)                         VIEWPORT (world units)

  (0,0)                                              +v (toward the camera's up)
    +---------------------------+ → x         +------+------+
    |                           |             |      |      |
    |                           |             |------O------|──► +u (toward the camera's right)
    |                           |             |      |      |
    +---------------------------+             +------+------+
    ↓ y                  (1919,1079)
                                          u goes from −w/2 (left) to +w/2 (right)
  x goes from 0 to 1919                   v goes from +h/2 (top) to −h/2 (bottom)
  y goes from 0 to 1079                   (0, 0) is the CENTER
  (0, 0) is the TOP-LEFT corner
```

(`w` = viewport width and `h` = viewport height, from sections 5 and 6.)

They differ in three ways, and the translation has to fix all three:

| | Window | Viewport |
|---|---|---|
| **Where zero is** | top-left corner | center |
| **Units** | whole pixels (0 … 1919) | world units (−w/2 … +w/2) |
| **Vertical direction** | y grows **downward** | v grows **upward** |

The translation happens in four steps (sections 9 to 12). The first three never involve the camera; only the fourth does.

---

## 9. Step 1: aim at the pixel's center

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

## 10. Step 2: normalize to a fraction

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

## 11. Step 3: from fractions to u and v

### What u and v are

**u** and **v** are the two coordinates of a spot **on the viewport**, measured in world units **from the viewport's center**:

- **u**: how far toward the camera's **right** (+) or **left** (−) of the center.
- **v**: how far toward the camera's **up** (+) or **down** (−) of the center.

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

They are **distances along the camera's own axes**, not world coordinates. "u = 0.5" means "half a unit toward wherever the camera's right happens to point". Turning that into world coordinates is step 4's job.

**Why not call them x and y?** Those names are already used for the pixel's column and row, and for world coordinates. Graphics programmers conventionally use **u, v** for positions on a flat surface.

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

## 12. Step 4: from (u, v) to the ray direction D

This is the only step where the camera's directions come in. It has two parts:
- **(A)** build the arrow that goes from the camera to the pixel's spot,
- **(B)** give that arrow a length of 1.

### A. Building the arrow

Reading `u` and `v` out loud gives a set of walking instructions:

> Start at the camera. Take **1 step forward** (that lands on the viewport's center). Then take **u steps right**, then **v steps up**.

Two operations on arrows turn that into numbers.

**Repeating a movement: multiply each number.**

```
2    · (0.8, 0, −0.6) = (1.6,  0, −1.2)     twice as far
0.5  · (0.8, 0, −0.6) = (0.4,  0, −0.3)     half as far
−1   · (0.8, 0, −0.6) = (−0.8, 0,  0.6)     a negative amount means the opposite way
```

**Chaining movements: add them number by number.** Doing one movement and then another is the same as one movement whose numbers are the sums:

```
(2, 0, 0) + (0, 3, 0) = (2, 3, 0)
```

So the three instructions become:

```
arrow = 1·forward + u·right + v·up
```

And since the arrow starts at the camera, the pixel's spot as a world point is `P = O + arrow`. We don't actually need P: `arrow` **is** `P − O`, which is exactly what the ray equation wants.

**Check it against the simplest camera.** A camera at the origin looking north has forward = (0, 0, 1), right = (1, 0, 0), up = (0, 1, 0):

```
1·(0, 0, 1) + u·(1, 0, 0) + v·(0, 1, 0) = (u, v, 1)
```

The spot's world coordinates are then just `u`, `v` and the distance 1, which is what you'd draw by hand:

```
  side view of that camera (looking from its right)

        y ▲
          |            viewport
          |               |  ← top edge     (v = +h/2)
          |               |
  O ●─────┼───────────────●───────────────►  z
 camera   |               | (0, 0, 1) = viewport center
          |               |  ← bottom edge  (v = −h/2)
          |               |
          0               1
```

For any other camera, the same three instructions still hold; only the three arrows differ.

### B. Normalizing the arrow (the second meaning)

Here, **normalize** means: *keep the arrow pointing the same way, but make its length exactly 1.* An arrow of length 1 is called a **unit vector**.

#### B1. How long is an arrow?

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

#### B2. Shrinking the arrow to length 1

Divide **each** of the three numbers by the length. All three shrink by the same factor, so the direction stays the same:

```
D = ( a / len,  b / len,  c / len )
```

2D check: `(3, 4) ÷ 5 = (0.6, 0.8)`, and `√(0.6² + 0.8²) = √(0.36 + 0.64) = √1 = 1` ✓

#### B3. Why bother?

With a length-1 direction, **`t` measures real distance**: `t = 10` means "10 world units from the camera". When a ray hits several objects, you keep the one with the smallest `t`, and that comparison is only a distance comparison if D has length 1. For a single flat disc it doesn't change which pixels are red, but it's standard practice and lighting will depend on it later.

> Turning the camera rotates the arrow but never stretches it. Two cameras that differ only in orientation give the same arrow **length** for the same pixel, so the same pixel keeps the same shape of calculation wherever the camera looks.

---

## 13. The complete pixel → ray chain

```
 C line:  O (position),  forward (orientation),  FOV
     │
     │  w = 2·tan(FOV/2),  h = w · Y_MAX / X_MAX        (sections 5–6)
     │  right = normalize(worldup × forward),  up = forward × right   (section 7)
     ▼
 pixel (x, y)
     │  step 1: + 0.5                                    (section 9)
     ▼
 (x + 0.5, y + 0.5)
     │  step 2: ÷ X_MAX, ÷ Y_MAX                         (section 10)
     ▼
 fractions (s, t)          0 … 1
     │  step 3: u = s·w − w/2,   v = h/2 − t·h           (section 11)
     ▼
 viewport coordinates (u, v)
     │  step 4A: arrow = forward + u·right + v·up        (section 12)
     ▼
 arrow  (= P − O)
     │  step 4B: ÷ its length
     ▼
 D  (length 1)  ──►  P(t) = O + t·D  ──►  sphere quadratic  ──►  red or black
                          ▲
                          └── the camera's position, used here and nowhere else
```

Two things worth noting:

- **Steps 1 to 3 never touch the camera.** They only depend on the window size and the viewport size. Move or turn the camera and they give exactly the same `u` and `v`.
- **The position enters only at the end.** Moving the camera changes where rays start, not where they point.

---

## 14. Going backwards: world → pixel

Sometimes you want the opposite: **given a point somewhere in the 3D scene, which pixel does it appear on?** (Useful for checking your work, e.g. "the sphere's center should land in the middle of the screen".)

The idea: **undo each step, in reverse order.** It's like getting undressed: the last thing you put on comes off first.

### R1. Measure the point along the camera's three directions (undo step 4)

Take any world point `Q`. First get the movement from the camera to it:

```
M = Q − O
```

Now you need three numbers: how much of `M` goes **forward**, how much goes **right**, and how much goes **up**. That question has a standard tool.

#### The dot product: how much of a movement goes in a given direction

For a direction `n` of **length 1**, the amount of `M` that goes along `n` is:

```
M · n = Mx·nx + My·ny + Mz·nz
```

Multiply the matching numbers, then add. Geometrically it's the length of `M`'s shadow on the line of `n`:

```
              ● M
             ╱|
           ╱  |
         ╱    |
   cam ●──────┴──────────► n
       |──────|
       M · n  (the part of M that goes along n)
```

Checks that make it believable:

| M | n | M · n | why it's right |
|---|---|---|---|
| (3, 0, 4) | (0, 0, 1) = north | 4 | the north part of M is 4 ✓ |
| (3, 0, 4) | (1, 0, 0) = east | 3 | the east part of M is 3 ✓ |
| (0.8, 0, −0.6) | (0.8, 0, −0.6) | 0.64 + 0.36 = 1 | a movement of 1 straight along n ✓ |
| (0.6, 0, 0.8) | (0.8, 0, −0.6) | 0.48 − 0.48 = 0 | M is at a right angle to n: none of it goes that way ✓ |

#### The three measurements

```
depth    = M · forward        how far in front of the camera Q is
sideways = M · right          how far to the camera's right
vertical = M · up             how far above the camera's line of sight
```

> ⚠️ This only works if `depth > 0`. If `depth ≤ 0`, the point is beside or behind the camera and doesn't appear on screen.

#### Dividing by depth

The viewport sits at depth 1. The ray from the camera to Q crosses it somewhere, and the two triangles below are **similar**: same angle at the camera, different size. In similar triangles the ratios of the sides are equal.

```
   up ▲
      |                                ● Q
      |                           ╱    |
      |        v ●          ╱          | vertical
      |          |     ╱               |
      |      ╱   |                     |
  cam ●─────┴────┴─────────────────────┴────► forward
      0          1                    depth
```

```
v / 1 = vertical / depth        →     v = vertical / depth
u / 1 = sideways / depth        →     u = sideways / depth
```

In words: **divide by the depth**. A point twice as far away lands half as far from the center, which is why distant things look small.

**Check against the simplest camera** (at the origin, looking north): `M = Q`, `depth = Qz`, `sideways = Qx`, `vertical = Qy`, so `u = Qx / Qz` and `v = Qy / Qz`, the familiar "divide by z".

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

This gives a position **in pixel units**, usually with decimals, like `1302.857`. Which pixel is that? Pixel column `x` covers the span from `x` up to (but not including) `x + 1` (see section 9). So you **drop the decimals** (round down, called **floor**):

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
 world point Q
     │  R1: M = Q − O
     │      depth = M·forward     (≤ 0 → behind the camera)
     │      u = (M·right) / depth,   v = (M·up) / depth
     ▼
 viewport coordinates (u, v)
     │  R2: s = (u + w/2) / w,   t = (h/2 − v) / h
     ▼
 fractions (s, t)          outside 0…1 → off-screen
     │  R3: × X_MAX, × Y_MAX, then round down
     ▼
 pixel (column, row)
```

---

## 15. Full worked example

Do this with pencil, paper and a calculator. Cover the answers and check yourself after each line.

### Setup

- Window: `X_MAX = 1920`, `Y_MAX = 1080`
- Scene file line: `C 10,0,5  0.6,0,0.8  90`
  - `O = (10, 0, 5)`
  - `forward = (0.6, 0, 0.8)` — 0.6 east and 0.8 north (this one happens to be level, which keeps the arithmetic readable; nothing below depends on that)
  - FOV = 90°
- Pixel to convert: **column 1440, row 270** (three-quarters across, one-quarter down)

**First, check that forward really has length 1** (the file is supposed to guarantee it):

```
√(0.6² + 0² + 0.8²) = √(0.36 + 0 + 0.64) = √1 = 1  ✓
```

### Part 0: viewport size and camera directions

```
FOV / 2             = 45°
tan(45°)            = 1
w = 2 · 1           = 2                    w/2 = 1
h = 2 · 1080 / 1920 = 1.125                h/2 = 0.5625

forward = (0.6, 0,  0.8)                   from the file

right   = normalize( worldup × forward )
        = normalize( (1·0.8 − 0·0,  0·0.6 − 0·0.8,  0·0 − 1·0.6) )
        = normalize( (0.8, 0, −0.6) )      length √(0.64 + 0.36) = 1, nothing to divide
        = (0.8, 0, −0.6)                   0.8 east, 0.6 south

up      = forward × right
        = (0·(−0.6) − 0.8·0,  0.8·0.8 − 0.6·(−0.6),  0.6·0 − 0·0.8)
        = (0, 1, 0)                        straight at the sky (the camera is level)
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

**Step 4A: build the arrow**

```
1 · forward = 1        · (0.6, 0,  0.8)  = (0.600000, 0.000000,  0.800000)
u · right   = 0.500521 · (0.8, 0, −0.6)  = (0.400417, 0.000000, −0.300313)
v · up      = 0.280729 · (0,   1,  0  )  = (0.000000, 0.280729,  0.000000)
                                            ────────────────────────────── add each column
arrow                                    = (1.000417, 0.280729,  0.499687)
```

Sanity check: compared with forward `(0.6, 0, 0.8)`, the arrow has more east and less north, so it leans toward the camera's right ✓, and its y is positive, so it also leans toward the sky ✓. That matches a pixel right of and above the center.

**Step 4B: length**

```
1.000417² = 1.000834
0.280729² = 0.078809
0.499687² = 0.249687
            ────────
sum       = 1.329330

length = √1.329330 = 1.152966
```

**Step 4B: divide**

```
D = (1.000417 / 1.152966,  0.280729 / 1.152966,  0.499687 / 1.152966)
  = (0.867690,  0.243484,  0.433393)
```

**Verify the length is 1:**

```
0.867690² + 0.243484² + 0.433393²
= 0.752886 + 0.059284 + 0.187830
= 1.000000  ✓
```

**Result:** the ray for pixel (1440, 270) is

```
P(t) = (10, 0, 5) + t · (0.867690, 0.243484, 0.433393)
```

### Part 2: world → pixel (the round trip)

Walk 10 units along that ray to get a point in the world:

```
Q = O + 10 · D = (10 + 8.676900,  0 + 2.434840,  5 + 4.333930)
               = (18.676900, 2.434840, 9.333930)
```

Now forget where Q came from, and find its pixel. If everything is right, you should get back to **(1440, 270)**.

**R1: measure Q along the camera's directions**

```
M = Q − O = (8.676900, 2.434840, 4.333930)

depth    = M · forward = 8.676900·0.6 + 2.434840·0 + 4.333930·0.8
                       = 5.206140 + 0 + 3.467144 = 8.673284      (> 0 ✓ in front)
sideways = M · right   = 8.676900·0.8 + 2.434840·0 + 4.333930·(−0.6)
                       = 6.941520 + 0 − 2.600358 = 4.341162
vertical = M · up      = 8.676900·0 + 2.434840·1 + 4.333930·0
                       = 2.434840

u = sideways / depth = 4.341162 / 8.673284 = 0.500521
v = vertical / depth = 2.434840 / 8.673284 = 0.280729
```

Same u and v as in Part 1 ✓. (Dividing by depth cancels out how far along the ray we walked: walking twice as far doubles all three measurements.)

**R2: back to fractions**

```
s = (u + w/2) / w = (0.500521 + 1) / 2          = 1.500521 / 2     = 0.750260
t = (h/2 − v) / h = (0.5625 − 0.280729) / 1.125 = 0.281771 / 1.125 = 0.250463
```

Both are between 0 and 1, so the point is on screen ✓

**R3: back to pixels**

```
s · 1920 = 1440.5    → floor → 1440
t · 1080 =  270.5    → floor →  270
```

**Result: pixel (1440, 270).** The round trip closes ✓. Notice that `1440.5` is exactly the "pixel center" value from step 1.

### Part 3: a quick bonus (what the camera stares at)

Which pixel shows the point exactly 10 units straight ahead of the camera, `Q = O + 10·forward = (16, 0, 13)`?

```
M = (6, 0, 8)
depth    = 6·0.6 + 8·0.8   = 3.6 + 6.4 = 10
sideways = 6·0.8 + 8·(−0.6) = 4.8 − 4.8 = 0
vertical = 0
u = 0 / 10 = 0        v = 0 / 10 = 0
s = (0 + 1) / 2 = 0.5          t = (0.5625 − 0) / 1.125 = 0.5
0.5 · 1920 = 960.0 → 960       0.5 · 1080 = 540.0 → 540
```

It lands **exactly on the corner** shared by pixels 959/960 and 539/540. There's no single middle pixel, because 1920 and 1080 are even numbers. Rounding down assigns the corner to pixel (960, 540).

---

## 16. Practice exercise

Same setup as section 15: 1920 × 1080, `C 10,0,5  0.6,0,0.8  90`.

**Exercise A (pixel → ray):** find the direction D for pixel **(480, 810)**.

**Exercise B (world → pixel):** which pixel shows the world point **Q = (13, 1, 12)**?

<details>
<summary>Answers A</summary>

```
centers:  480.5,  810.5
s = 480.5 / 1920 = 0.250260
t = 810.5 / 1080 = 0.750463
u = 0.250260 · 2 − 1           = −0.499479
v = 0.5625 − 0.750463 · 1.125  = 0.5625 − 0.844271 = −0.281771

1·forward =             (0.600000,  0.000000, 0.800000)
u·right   = −0.499479 · (0.8, 0, −0.6) = (−0.399583, 0.000000, 0.299687)
v·up      = −0.281771 · (0, 1, 0)      = ( 0.000000, −0.281771, 0.000000)
arrow                                  = ( 0.200417, −0.281771, 1.099687)

length² = 0.040167 + 0.079395 + 1.209311 = 1.328873
length  = 1.152768
D = (0.173857, −0.244430, 0.953953)
check: 0.030226 + 0.059746 + 0.910027 = 0.999999 ✓
```

The pixel is in the bottom-left quarter, so the ray leans toward the camera's **left** (less east, more north than forward) and **downward** (negative y) ✓

</details>

<details>
<summary>Answers B</summary>

```
M = Q − O = (13 − 10,  1 − 0,  12 − 5) = (3, 1, 7)

depth    = 3·0.6 + 1·0 + 7·0.8    = 1.8 + 5.6 = 7.4     (> 0 ✓)
sideways = 3·0.8 + 1·0 + 7·(−0.6) = 2.4 − 4.2 = −1.8
vertical = 3·0   + 1·1 + 7·0      = 1

u = −1.8 / 7.4 = −0.243243
v =    1 / 7.4 =  0.135135

s = (−0.243243 + 1) / 2          = 0.756757 / 2     = 0.378378
t = (0.5625 − 0.135135) / 1.125  = 0.427365 / 1.125 = 0.379880

0.378378 · 1920 = 726.486 → 726
0.379880 · 1080 = 410.270 → 410
```

**Pixel (726, 410)**: left of center (u < 0) and above center (v > 0) ✓

</details>

**Make your own:** pick any pixel, run Part 1, walk any distance along D, and run Part 2. If you don't land back on your pixel, one of the steps has a mistake. Also try the same pixel with FOV 60° (`tan(30°) ≈ 0.57735`), or with the camera turned to `forward = (0.8, 0, 0.6)`, and watch which numbers change.

---

## 17. Self-check

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
<summary>4. You move the camera without turning it. Which of s, t, u, v, D and O change?</summary>

Only **O**. The viewport travels with the camera, so both the camera and the pixel's spot shift by the same amount, and the movement between them (the arrow, and therefore D) is unchanged. Steps 1 to 3 never involve the camera at all.

</details>

<details>
<summary>5. A camera has forward = (0, 0, −1). What are its right and up?</summary>

`worldup × forward = (fz, 0, −fx) = (−1, 0, 0)`, already length 1, so **right = (−1, 0, 0)**: 1 west. That matches the everyday check — face south and your right arm points west ✓. Then `up = forward × right = (0·0 − (−1)·0, (−1)·(−1) − 0·0, 0·0 − 0·(−1)) = (0, 1, 0)`, straight at the sky, since this camera isn't tilted.

</details>

<details>
<summary>6. Why is `right` always horizontal, even for a camera tilted steeply up or down?</summary>

Because `worldup × forward` works out to `(fz, 0, −fx)` for any forward: the middle slot is 0 by construction, and `fy` doesn't appear at all. Geometrically, `right` depends only on the camera's shadow on the ground, so tilting the camera doesn't move it. "Horizontal" means y = 0, not "along an axis": `(0.6, 0, −0.8)` is horizontal too.

</details>

<details>
<summary>7. Where does the camera's roll come from, given that the C line never mentions it?</summary>

From a convention. Every arrow perpendicular to `forward` is a candidate for `up` — a whole disc of them — and the file doesn't choose. We choose the one that keeps `right` flat on the ground, which is what `worldup` is doing in the formula. The choice breaks down only when `forward` is exactly straight up or down, where every candidate is equally horizontal.

</details>

<details>
<summary>8. Why is the "center" pixel (960, 540) at u ≈ +0.0005, v ≈ −0.0005 instead of exactly (0, 0)?</summary>

1920 and 1080 are even, so there's no single middle pixel. The true center is the corner between four pixels. Pixel (960, 540) is the one just right of and just below that corner, and its center is half a pixel away from it.

</details>

<details>
<summary>9. For that center pixel, what does step 4 produce, and what does normalizing change?</summary>

`arrow = forward + 0.0005·right − 0.0005·up`, which is forward with two tiny nudges, so it's almost exactly `forward` itself. Its length is about 1.0000003, so dividing by it barely changes anything. Normalization matters more toward the corners, where the arrow's length reaches about 1.52.

</details>

<details>
<summary>10. What are the two different meanings of "normalize" in these notes?</summary>

(1) In step 2: turn a count into a **fraction from 0 to 1** by dividing by the total. (2) In step 4B: turn an arrow into one of **length 1** by dividing each coordinate by its length.

</details>

---

## 18. Cheat sheet

```
FROM THE C LINE     C  px,py,pz  fx,fy,fz  FOV
  O       = (px, py, pz)                 ray start
  forward = (fx, fy, fz)                 already length 1
  worldup = (0, 1, 0)                    the no-roll convention
  right   = normalize(worldup × forward) = normalize((fz, 0, −fx))
  up      = forward × right              (already length 1)
  guard: forward = (0, ±1, 0) → worldup × forward = (0,0,0);
         use (0, 0, 1) as the reference instead

VIEWPORT SIZE
  d = 1
  w = 2 · tan(FOV/2)            (FOV converted to radians)
  h = w · Y_MAX / X_MAX

PIXEL → RAY
  s     = (x + 0.5) / X_MAX
  t     = (y + 0.5) / Y_MAX
  u     = s·w − w/2
  v     = h/2 − t·h
  arrow = forward + u·right + v·up
  D     = arrow / length(arrow)          length(a,b,c) = √(a² + b² + c²)
  ray   = P(t) = O + t·D

WORLD → PIXEL
  M        = Q − O
  depth    = M · forward                 (≤ 0 → behind the camera)
  u        = (M · right) / depth
  v        = (M · up)    / depth
  s        = (u + w/2) / w
  t        = (h/2 − v) / h               (outside 0…1 → off-screen)
  column   = floor(s · X_MAX)
  row      = floor(t · Y_MAX)

  dot product:    A · B = Ax·Bx + Ay·By + Az·Bz          (0 → perpendicular)
  cross product:  A × B = (Ay·Bz − Az·By,                 (perpendicular to both;
                           Az·Bx − Ax·Bz,                  order matters:
                           Ax·By − Ay·Bx)                  B × A points the other way)
```
