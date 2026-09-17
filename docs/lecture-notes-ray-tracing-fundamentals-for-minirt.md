# Ray Tracing Fundamentals for miniRT

> These notes explain the ideas you need in order to *read the miniRT subject and know what it is asking for*: how a ray finds an object, what "the inside of an object" means, why a cylinder is harder than a sphere, what a surface normal is, how the four lighting terms (spot brightness, diffuse, ambient, hard shadows) combine into a pixel colour, and what the camera's field of view controls. No prior graphics knowledge is assumed. Nothing here is code — it is the vocabulary and the geometry.

**Written:** 2026-09-06

---

## Table of contents

1. [Prerequisites](#prerequisites)
2. [TL;DR](#tldr)
3. [What a ray tracer actually does](#1-what-a-ray-tracer-actually-does)
4. [Ray–object intersection and the `t` rule](#2-rayobject-intersection-and-the-t-rule)
5. [The inside of an object](#3-the-inside-of-an-object)
6. [The cylinder: three surfaces in a trench coat](#4-the-cylinder-three-surfaces-in-a-trench-coat)
7. [Surface normals](#5-surface-normals)
8. [The scene file drives everything](#6-the-scene-file-drives-everything)
9. [Lighting, term by term](#7-lighting-term-by-term)
10. [The camera and field of view](#8-the-camera-and-field-of-view)
11. [How the pieces fit together](#how-the-pieces-fit-together)
12. [Common confusions](#common-confusions)
13. [Self-check](#self-check)
14. [Further reading](#further-reading)

---

## Prerequisites

Deliberately short. You need:

- **3D coordinates.** A point is three numbers `(x, y, z)`. A vector is also three numbers, but it means a *direction and a length* rather than a place.
- **The dot product.** For two vectors `A` and `B`, `A · B = Ax*Bx + Ay*By + Az*Bz`. The one fact that matters below: if both vectors have length 1, the dot product equals the cosine of the angle between them. So it is `1` when they point the same way, `0` when they are perpendicular, and negative when they point in opposing directions.
- **"Normalized"** means "scaled so its length is exactly 1". A normalized vector carries direction only, no magnitude.
- **Solving a quadratic.** `at² + bt + c = 0` has 0, 1 or 2 real solutions depending on the sign of the discriminant `b² - 4ac`.

That's it.

---

## TL;DR

- A **ray** is a half-line: a starting point plus a direction, parameterized by a single number `t`. Every candidate hit is just a value of `t`.
- The selection rule for which hit to draw is **"the smallest `t` that is still greater than zero"** — not simply "the closest", because negative `t` means *behind the camera*.
- The **inside** of an object is the case where the camera sits within it: one root goes negative, and the surviving root is the inner wall. Your code must render it rather than break.
- A **cylinder is three surfaces** (tube + two caps), giving up to four candidate hits, each with its own extra validity test.
- The **normal** is the arrow sticking straight out of a surface at a point. It is the single most-used quantity in lighting.
- A pixel's colour is `object colour × (ambient + diffuse)`, where **diffuse** depends on the angle between the normal and the light, **ambient** is a flat constant that stops shadows being pure black, and a **shadow ray** decides whether the diffuse part is allowed at all.
- Every number in the `.rt` file must genuinely drive the image. Hardcoding a radius or an axis is the failure this part of the subject is designed to catch.

---

## 1. What a ray tracer actually does

### Intuition

In the real world, light leaves a lamp, bounces around a room, and a tiny fraction of it happens to enter your eye. Simulating that forwards is hopeless — almost every ray you launch from the lamp misses the eye entirely.

So ray tracing runs the physics **backwards**. It starts at the eye and asks, for each pixel of the image: *if light arrived here, where did it come from?* You shoot one ray out through that pixel, find the first thing it hits, and work out how bright that spot is. Repeat for every pixel and you have an image.

```
   camera                image plane           the scene
                        (your window)
     o - - - - - - - - - - | - - - - - - ->  ( sphere )
     |                     |
     |                     | one ray per pixel
     o - - - - - - - - - - | - - - - - - ->  =============  plane
```

### Mechanics

A **ray** is written as:

```
    P(t) = O + t * D
```

- `O` is the **origin** — where the ray starts (the camera position).
- `D` is the **direction** — a normalized vector saying which way it travels.
- `t` is a single number, the **parameter**. Feed in a `t`, get back a point in space.

Because `D` has length 1, `t` is literally *distance travelled along the ray*.

```
    t = 0      t = 1      t = 2      t = 3
      O----------*----------*----------*------>  D
```

A ray is a **half-line**, not a full line: only `t > 0` is in front of you. `t = 0` is the camera itself, and `t < 0` is the region behind the camera — real points in space, but ones the camera cannot see.

> 🔑 **Key idea.** Everything in the intersection half of a ray tracer reduces to: *find the values of `t` where the ray touches a surface, then decide which one counts.*

---

## 2. Ray–object intersection and the `t` rule

### Intuition

A ray aimed at a ball does not touch it once. It punches in one side and out the other. Both of those are genuine intersections of the line with the sphere; only one of them is the surface you can see.

### Mechanics

Substituting the ray equation into a sphere's equation produces a **quadratic in `t`**, so it yields up to two roots, conventionally `t1 ≤ t2`:

- **discriminant < 0** → no real roots → the ray misses entirely.
- **discriminant = 0** → one root → the ray grazes the surface tangentially.
- **discriminant > 0** → two roots → entry and exit.

```
        camera
          o------------>  ray
                 t1   t2
                  *----*
                 (  sphere  )
              entry     exit
```

### The selection rule

Having candidates is not the same as having an answer. The rule is:

> 📌 **Discard every `t` that is ≤ 0, then keep the smallest of the survivors.**

The two halves of that sentence do different jobs:

- **Discard `t ≤ 0`** removes everything behind the camera. Without this, an object *behind* you would be drawn in front of you.
- **Smallest survivor** implements occlusion: the nearest visible surface hides everything further along the same ray.

This one rule must then be applied across *all objects in the scene*, not just within a single object — you gather every valid `t` from every sphere, plane and cylinder, and the global smallest one is the surface that pixel shows.

### Worked example

A ray with `O = (0,0,0)` and `D = (0,0,1)`. Three objects lie along it, producing candidate values `t = -4`, `t = 3`, and `t = 9`.

| candidate | in front? | verdict |
|---|---|---|
| `-4` | no — behind the camera | discarded immediately |
| `3` | yes | **chosen** — smallest survivor |
| `9` | yes | valid, but hidden behind the object at 3 |

The pixel shows the object at `t = 3`.

### Edge cases & gotchas

- **`t = 0` exactly** means the surface passes through the camera itself. Treat it as invalid rather than valid — it will otherwise produce a degenerate hit at the eye.
- **The tangent case** (one root) is real and must not crash your solver — it appears at the silhouette edge of every sphere.
- **Floating point never lands exactly on zero.** Comparisons against `0` in practice mean "compared against a very small positive epsilon".

---

## 3. The inside of an object

### Intuition

Put the camera inside a hollow ball. You are not blind — you see the ball's inner wall wrapped all around you. The subject line *"all possible intersections and the insides of the objects must be handled correctly"* is asking that this situation produce a sensible image instead of a black screen or a crash.

### Mechanics

Nothing new is needed. The same quadratic runs, but the sign pattern of its roots changes:

```
camera outside:                 camera inside:

  o------*--------*               *-----o--------*
  0     t1       t2              t1     0       t2
   (both roots positive)      (t1 negative, t2 positive)
        -> pick t1                     -> pick t2
```

When the origin is inside the sphere, the entry point lies *behind* the camera, so `t1` is negative and gets discarded. `t2` is not "the far hit chosen by exception" — after filtering, it is the **only** survivor, and therefore also the smallest survivor. The rule from section 2 is unchanged.

> ⚠️ **The trap.** A rule stated as "pick the closest intersection" gives the wrong answer here, because `t1` is closer in absolute distance. The rule is "smallest **positive** `t`". Sign first, magnitude second.

### Edge cases & gotchas

- **Lighting flips inside.** The outward-pointing normal now faces *away* from the camera, which makes the surface come out unlit even though you are staring right at it. The standard treatment is to detect that the normal points away from the ray and flip it before shading.
- **Which side is "in front" is per-object, not per-scene.** The camera may be inside one sphere and outside another in the same image; each object is tested independently.

---

## 4. The cylinder: three surfaces in a trench coat

### Intuition

A sphere is one seamless surface. A cylinder is not: it is a tube wall with a flat disc glued on each end. Those three pieces obey different equations, so "intersect a cylinder" is really three separate intersection tests whose results get pooled.

On top of that, the equation for a tube describes an *infinite* tube stretching forever along its axis. Your object has a finite height, so the algebra will happily hand you hits on parts of the tube that do not exist.

### Mechanics

The `.rt` file defines a cylinder by a **centre point**, an **axis vector** (which way the tube points), a **diameter**, and a **height**.

```
          ^ axis
          |
    +-----|-----+   <- top cap: a disc
    |     |     |
    |     |     |   height
    |     |     |
    +-----|-----+   <- bottom cap: a disc
          |
     .....|.....    <- the infinite tube continues, but is NOT your object
```

**Piece 1 — the tube wall.** Gives a quadratic, so up to two roots, exactly like the sphere. Each root then needs a **second test**: project the hit point onto the axis and check the result falls between the two ends. A root that lands outside the height range is thrown away just as firmly as a negative `t`.

```
    ray  o------*-------*------>
                t1      t2
    t1: on the real tube      -> keep
    t2: above the top rim     -> algebraically fine, geometrically nonexistent -> discard
```

**Pieces 2 and 3 — the caps.** Each cap is a flat disc, so it is a plane intersection followed by a **radius test**: the hit point is kept only if its distance from the cap's centre is at most the radius. Hits further out are on the infinite plane containing the cap, not on the cap.

So one cylinder produces **up to four candidate `t` values**. They all go into the same pool and the same rule applies: throw out `t ≤ 0`, throw out tube hits outside the height, throw out cap hits outside the radius, keep the smallest of whatever is left.

### Worked example

A cylinder of height 4 whose axis is vertical. A ray produces:

| candidate | source | test | verdict |
|---|---|---|---|
| `t = -1.2` | tube | `t ≤ 0` | discarded |
| `t = 2.6` | tube | projects to height 6.1 — above the top | discarded |
| `t = 5.0` | top cap | 0.8 from the cap centre, radius 1.5 | **kept** |
| `t = 8.4` | bottom cap | 2.9 from the centre, radius 1.5 | discarded |

One survivor, so the pixel shows the top cap at `t = 5.0`.

### Edge cases & gotchas

- **An uncapped cylinder looks hollow.** If you skip the cap tests, looking at the cylinder end-on shows straight through into the tube's interior. The subject's reference images show closed cylinders.
- **A ray parallel to the axis never hits the tube at all** — the quadratic degenerates. It can still hit the caps.
- **The rim is a discontinuity.** Tube and cap meet at a sharp circle with no smooth transition; both tests can succeed at nearly the same `t` there.

---

## 5. Surface normals

### Intuition

Lighting needs to know which way a surface is facing. Hold a sheet of paper up to a lamp: face it head-on and it is bright, tilt it and it dims. The mathematical stand-in for "which way it faces" is the **normal**.

### Mechanics

The **normal** at a point on a surface is the vector that points straight out of the surface there, perpendicular to it, normalized to length 1. Every surface type has its own rule for computing it.

**Plane.** Flat everywhere, so the normal is the *same at every point*. It is not computed at all — the `pl` line in the scene file supplies it directly.

```
    ^     ^     ^     ^     ^
    |     |     |     |     |
  ==============================
     every point: the same arrow
```

**Sphere.** Points radially outward from the centre. For a hit point `P` and centre `C`, the direction is `P - C`, normalized. Every point has a different normal, and that continuous variation is what makes a sphere shade smoothly.

```
              ^
          ^   |   ^
            \ | /
      <-----  C  ----->
            / | \
          v   |   v
              v
```

**Cylinder — three rules, one per surface.**

*Tube wall:* perpendicular to the axis, pointing away from it. Not away from the cylinder's centre point — away from the **nearest point on the axis line**. Picture a hairbrush roller: bristles stick out sideways, never tilting toward the ends.

*Top cap:* constant, along the axis direction.

*Bottom cap:* constant, along the negative axis direction.

```
          ^ ^ ^ ^ ^          <- top cap: +axis
        +---------+
      <-|         |->
      <-|         |->        <- tube: perpendicular to the axis,
      <-|         |->           anywhere along the height
        +---------+
          v v v v v          <- bottom cap: -axis
```

### Edge cases & gotchas

- **Normals must be normalized** before use. The dot-product-equals-cosine shortcut is only valid for unit-length vectors; an unnormalized normal silently scales your brightness.
- **The rim's abrupt change** from sideways to axial is what gives a cylinder its crisp edge, in contrast to the sphere's continuous gradient.
- **Inside an object, the outward normal is the wrong one** for shading, and needs flipping — see section 3.

---

## 6. The scene file drives everything

### Intuition

Two bullets in the subject say the program must be able to *resize* objects and to *apply translation and rotation transformations*. Read cold, these sound like interactive features — dragging a slider while the program runs. They are not. The mandatory part accepts exactly one input, the `.rt` file passed as the first argument, and requires exactly two events, `ESC` and the window's close button. There is no mechanism for runtime editing anywhere in the mandatory spec.

What the bullets actually demand is that **every number in the file genuinely reaches the maths**. Change a value, re-run, and the image must change accordingly.

### Mechanics — the two transformations

A **transformation** is an operation that moves every point of an object to a new position.

**Translation** is sliding. Every point moves by the same offset in the same direction. Orientation and shape are untouched. In the file, this is the object's coordinate triple.

```
   before                     after translating by (+4, +1)

   +---+                                 +---+
   | A |            =>                   | A |
   +---+                                 +---+
```

**Rotation** is turning about an axis. The centre stays put; the object pivots and changes *orientation*, not position or shape. In the file, this is the orientation vector — the cylinder's axis, the plane's normal, the camera's direction.

```
   before                     after rotating 90°

     ^ axis                        +-----+
     |                             |     |  ---> axis
   +-|-+                           +-----+
   | | |
   +---+
```

**Resizing** is the third: a sphere's diameter, a cylinder's diameter and height.

Spheres and lights are explicitly excluded from rotation, and the file format shows why — neither line carries an orientation vector. A sphere line has a centre and a diameter; a light line has a position, a brightness and a colour. A sphere looks identical however you spin it, and a point light radiates in every direction.

### The `.rt` elements referenced in these notes

| id | fields | example |
|---|---|---|
| `A` | ambient ratio `[0.0,1.0]`, R,G,B | `A 0.2 255,255,255` |
| `C` | position, normalized orientation vector, horizontal FOV `[0,180]` | `C -50,0,20 0,0,1 70` |
| `L` | position, brightness ratio `[0.0,1.0]`, R,G,B *(colour unused in mandatory)* | `L -40,50,0 0.6 10,0,255` |
| `cy` | centre, normalized axis vector, diameter, height, R,G,B | `cy 50,0,20.6 0,0,1 14.2 21.42 10,0,255` |

### Edge cases & gotchas

> ⚠️ **This is the hardcoding trap.** A sphere routine that assumes radius 1, or a cylinder that is always vertical because the axis vector is parsed but never used, can still produce a perfect-looking image for the one test scene you happened to write. It fails the moment someone edits the file — which is exactly what happens during evaluation.

- Test by **changing one number at a time** and confirming the picture responds in the expected direction.
- An orientation vector that arrives un-normalized will distort your maths; normalize on parse rather than trusting the file.

---

## 7. Lighting, term by term

The mandatory part names four things: spot brightness, hard shadows, ambient lighting, and diffuse lighting. They are not four independent features — they are the ingredients of a single colour calculation.

### 7.1 The spot and its brightness

**Spot** here means the light source — the `L` line. In miniRT it is a **point light**: an infinitely small dot at a coordinate, radiating equally in all directions. It has no size, no shape, no bulb.

```
              L
             *
          /  |  \
        /    |    \
      v      v      v      light leaves in every direction
```

**Brightness** is the ratio in `[0.0, 1.0]`. It is a dimmer knob: it does not change *where* light goes or *what* it reaches, only *how strong* it is on arrival. At `0.0` the lamp is off; at `0.6` every lit surface comes out at 60% of full strength.

### 7.2 Diffuse lighting

#### Intuition

Hold a sheet of paper facing a flashlight: bright. Tilt it: dimmer. Turn it edge-on: almost nothing. The flashlight never changed — only the **angle** did.

The reason is that a tilted surface spreads the same bundle of light over a larger patch, so each piece of surface receives less of it:

```
  light rays  |  |  |  |          light rays  \  \  \  \
              v  v  v  v                       \  \  \  \
           ===========             ===========================
           small patch,            same rays over a wide patch
           full strength           -> each bit is dimmer
```

#### Mechanics

The diffuse factor at a point is the dot product of two unit vectors:

- `N` — the surface **normal** at the hit point.
- `L_dir` — the normalized direction **from the hit point to the light**.

```
    diffuse factor = max(0, N · L_dir)
```

Because both are unit length, the dot product *is* the cosine of the angle between them. Facing the light gives `1`; perpendicular gives `0`; facing away gives a negative number, which is clamped to `0` because negative light does not exist.

**"Diffuse"** also means the surface scatters light equally in every direction, so the result does **not** depend on where the camera is. Only the light's angle matters. That is why miniRT surfaces look matte and chalky; a shiny highlight would require a specular term, which is not in the mandatory part.

#### Worked example

A unit sphere centred at the origin, a light far away straight up at `(0,100,0)`, brightness `0.6`, sphere colour white `(255,255,255)`.

Because the light is distant, `L_dir` is essentially `(0,1,0)` everywhere. And because the sphere is unit-radius at the origin, the normal at a hit point `P` is just `P`. So the diffuse factor collapses to *the y-component of the hit point*:

| hit point `P` | normal `N` | `N · L_dir` | result |
|---|---|---|---|
| `(0, 1, 0)` — top | `(0, 1, 0)` | `1.00` | full brightness |
| `(0.5, 0.866, 0)` | `(0.5, 0.866, 0)` | `0.87` | slightly dimmer |
| `(0.707, 0.707, 0)` | `(0.707, 0.707, 0)` | `0.71` | noticeably dimmer |
| `(0.866, 0.5, 0)` | `(0.866, 0.5, 0)` | `0.50` | half |
| `(1, 0, 0)` — side | `(1, 0, 0)` | `0.00` | black |
| `(0.707, -0.707, 0)` | `(0.707, -0.707, 0)` | `-0.71` → clamp `0` | black |

Turning the factor at `0.50` into a pixel means multiplying the three ingredients:

```
    255   ×   0.6   ×   0.50   =   76
     ^         ^         ^
   colour   light     angle
            brightness factor
```

That pixel is mid-grey `(76,76,76)`. The top of the sphere is `255 × 0.6 × 1.0 = 153`. The side is `0`.

```
        . : ▒ ▓ █ ▓ ▒ : .        <- facing the light: brightest
      : ▒ ▓ █ █ █ ▓ ▒ :
    . ▒ ▓ █ █ █ █ █ ▓ ▒ .
    : ▒ ▓ █ █ █ █ █ ▓ ▒ :
    . : ▒ ▓ █ █ █ ▓ ▒ : .
      . . : ▒ ▓ ▒ : . .
        .   .   .   .            <- turned away: black
```

That gradient is the entire reason a flat circle of pixels reads as a three-dimensional ball. The ring where the factor crosses zero is called the **terminator**.

### 7.3 Ambient lighting

#### Intuition

Look at the underside of a desk. No lamp shines on it directly, yet it is not black. Real light bounces: it hits the floor, scatters, and some of it reaches surfaces the lamp cannot see. Every surface in a room acts as a dim secondary light for every other surface.

```
        lamp *
             \
              \        the wall bounces light
               v       into the shadowed side
    ###########  ---->  ) sphere
    floor              (
                        ^ lit a little, though the
                          lamp cannot see it
```

A basic ray tracer does none of this. It traces one ray to a surface, asks whether the light can see that point, and stops. There is no second bounce, so anything the lamp cannot reach comes out **pure black** — which is why the sphere above lost its entire lower half.

Simulating real bounces is global illumination: expensive, and explicitly out of scope. So the cheap approximation is: **assume a constant amount of scattered light arrives everywhere, from every direction.** That constant is ambient lighting.

#### Mechanics

The `A` line gives a ratio in `[0.0, 1.0]` and a colour. Ambient ignores normals, ignores angles, ignores where the light is. Every point on every object receives it.

With `A 0.2` added to the sphere example:

| point | diffuse part | ambient part | total |
|---|---|---|---|
| top | `255 × 0.6 × 1.0 = 153` | `255 × 0.2 = 51` | `204` |
| factor `0.5` | `255 × 0.6 × 0.5 = 76` | `51` | `127` |
| side / bottom | `0` | `51` | `51` |

The dark half is now visible dark grey rather than a void. This is precisely what the subject means by *"objects are never completely in the dark"*.

#### Edge cases & gotchas

- Ambient sets the **floor brightness of the whole image**. Push it to `1.0` and every surface is fully lit everywhere, flattening the diffuse gradient until the ball looks like a disc again. Low ambient means high contrast; high ambient means washed out.
- Colour channels **clip at 255**. Ambient plus diffuse can overflow, and the result must be clamped or it wraps to nonsense.

### 7.4 Hard shadows

#### Intuition

Facing the light is not enough. Something might be standing in the way.

```
        L *
           \
            \
             ▓▓▓   <- an occluder blocks the path
              \
               \
   ------------ x ---------------   the point x is in shadow
```

The point `x` has a normal pointing up and a light above it, so the angle test says "bright" — but no light can physically reach it.

#### Mechanics

To catch that, fire a second ray **from the hit point toward the light** and ask whether it meets anything before arriving. That ray is a **shadow ray**; a blocking object is an **occluder**.

If the shadow ray is blocked, the **diffuse** contribution for that point is dropped. The **ambient** contribution is kept — ambient stands in for bounced light from all directions, and being in shadow does not stop that. This is exactly why ambient prevents shadows from becoming featureless black holes.

> 📌 A point ends up ambient-only for **two distinct reasons**, and both must be handled: it is turned away from the light (`N · L_dir ≤ 0`), or it is turned toward the light but blocked (shadow ray hits something).

#### Why "hard"?

The light is a single mathematical point with no size. From any location, that point is either visible or it is not — there is no in-between. So the shadow boundary is a perfect step: a fully lit pixel adjacent to a fully shadowed one.

```
   brightness across a shadow edge:

   hard (point light)        soft (light with real size)

   ████████                  ████████
           |                         \___
           |                             \___
           └────────                         ────────
   instant cliff              gradual falloff (penumbra)
```

Real lights have physical size — a bulb, a window, the sun's disc. At the edge of a real shadow part of the light is hidden and part is still visible, producing a gradual band called the **penumbra**. Reproducing it means sampling many points across the light's surface, i.e. many shadow rays per pixel. That is a bonus, not a defect of the mandatory part.

#### Edge cases & gotchas

- **Shadow acne.** A shadow ray starting exactly on the surface will, thanks to floating-point rounding, frequently re-intersect the very surface it started from at a `t` just above zero. The object then shadows itself and the render is speckled with black dots. The standard fix is to start the shadow ray a tiny epsilon away from the surface, along the normal.
- **Only occluders between the point and the light count.** An object *beyond* the light does not cast a shadow, so shadow-ray hits must be rejected past the light's distance.
- **Distance to the light matters for the test, not for the brightness.** miniRT's mandatory lighting has no distance falloff — the brightness ratio is the whole story.

---

## 8. The camera and field of view

### Intuition

A camera does not see everything around it. It sees a cone of directions pointing forward. **Field of view** is the angle of that cone, in degrees.

```
   FOV = 30°  (narrow)         FOV = 70°            FOV = 140°  (wide)

        \   /                    \       /        \               /
         \ /                      \     /          \             /
          \                        \   /            \           /
          C                         \ /              \         /
                                     C                \       /
                                                       \     /
                                                         C
```

**Horizontal** means it is measured left-to-right across the image. The vertical extent is not given in the file; it follows from the window's aspect ratio.

Everything inside the cone lands somewhere in the image. Everything outside it does not exist for this render.

### Mechanics — what changing it does

| FOV | behaviour | look |
|---|---|---|
| 20–40° | a narrow cone, like a telephoto lens | a small slice of the world fills the window: zoomed in, flattened perspective |
| ~70° | comfortable middle, close to natural human viewing | the subject's example value |
| 100–170° | a wide cone, like a fisheye | far more scene squeezed into the same pixels: everything smaller, edges visibly stretched |

> 🔑 **The camera never moves.** Changing FOV is not walking closer or further away. The camera stays exactly where the `C` line puts it; only the width of the cone changes. With a wide FOV the background rushes away while a nearby object stays large — a genuinely different effect from translating the camera.

### Edge cases & gotchas

Both ends of the `[0,180]` range are degenerate. At `0°` the cone has no width and you would see nothing. At `180°` the cone has flattened into a complete half-space and the projection onto a flat image breaks down — the image plane would need infinite width. Treat them as validation bounds your parser must accept and check, not as values that produce useful pictures.

---

## How the pieces fit together

```
   for every pixel:

     +---------------------------+
     |  build a ray through it   |   <- camera position, orientation, FOV
     +-------------+-------------+
                   |
                   v
     +---------------------------+
     |  intersect every object   |   <- sphere: 2 roots
     |  collect candidate t's    |      plane:  1 root
     +-------------+-------------+      cylinder: up to 4
                   |
                   v
     +---------------------------+
     |  discard t <= 0 and any   |   <- height range, cap radius
     |  that fail surface tests  |
     +-------------+-------------+
                   |
          no survivors? -> background colour
                   |
                   v
     +---------------------------+
     |  take the smallest t      |   = the visible surface
     +-------------+-------------+
                   |
                   v
     +---------------------------+
     |  compute the normal N     |   <- rule depends on the surface hit
     |  (flip it if inside)      |
     +-------------+-------------+
                   |
                   v
     +---------------------------+
     |  ambient = ratio          |   always applied
     +-------------+-------------+
                   |
                   v
     +---------------------------+
     |  shadow ray toward light  |
     +-------------+-------------+
             |             |
        blocked        clear
             |             |
             |             v
             |   +-----------------------+
             |   | diffuse = brightness  |
             |   |   * max(0, N · L_dir) |
             |   +-----------+-----------+
             |               |
             v               v
     +---------------------------+
     | colour = objColour        |
     |          * (ambient       |
     |             + diffuse)    |   -> clamp to 255
     +---------------------------+
```

---

## Common confusions

| X vs Y | The distinction |
|---|---|
| **"closest intersection"** vs **"smallest positive `t`"** | Only the second is correct. When the camera is inside an object the nearer root is negative — behind the camera — and must be discarded even though its absolute distance is smaller. |
| **facing away from the light** vs **in shadow** | Both end up ambient-only, but they are separate tests: one is a dot product on the normal, the other is a shadow ray. A point can pass the first and fail the second. |
| **translation** vs **rotation** | Translation slides an object without turning it (its coordinates change). Rotation turns it without moving its centre (its orientation vector changes). |
| **changing FOV** vs **moving the camera** | FOV widens or narrows the view cone from a fixed position. Moving the camera changes the position. They look different: FOV changes perspective exaggeration, translation does not. |
| **ambient** vs **diffuse** | Ambient is a flat constant applied everywhere regardless of geometry. Diffuse depends on the angle between the normal and the light, and is cancelled by shadows. |
| **hard** vs **soft** shadows | Hard shadows come from a point light: a binary visible/not-visible test and a razor-sharp edge. Soft shadows require a light with physical area and many samples — a bonus. |
| **the infinite tube** vs **the cylinder** | The tube equation has no ends. The height test is what turns its roots into hits on an object that actually exists. |
| **a hit on a cap's plane** vs **a hit on the cap** | The plane is infinite; the cap is the disc of radius `r` within it. The radius test separates them. |

---

## Self-check

1. A ray's intersection with a sphere yields `t1 = -3` and `t2 = 5`. Which value do you draw, and what does the sign of `t1` tell you about where the camera is?
2. Why can a cylinder produce four candidate intersections when a sphere produces at most two?
3. A point on a plane has its normal pointing straight at the light, and the dot product comes out as `0.9`. The rendered pixel is nevertheless as dark as the surrounding shadow. What single test explains this?
4. Two scenes are identical except that one has `A 0.1` and the other `A 0.9`. Describe how the two renders differ, and say why the second one makes a sphere look less three-dimensional.
5. You render a scene and it looks perfect. Someone changes the cylinder's axis vector from `0,0,1` to `0,1,0`, re-runs, and the image is unchanged. What has gone wrong, and which bullet of the subject does it violate?

<details>
<summary>Answers — attempt the questions first</summary>

**1.** You draw `t2 = 5`. Discard every `t ≤ 0` first, which removes `t1`, leaving `t2` as the only — and therefore smallest — survivor. A negative entry root means the ray was already inside the sphere when it started, so the camera is inside the sphere and you are looking at its inner wall.

**2.** Because a cylinder is three surfaces, not one. The tube wall is a quadratic and contributes up to two roots; each of the two end caps is a plane intersection contributing one more. Four candidates total, each with its own extra validity test — height range for the tube roots, radius for the cap roots.

**3.** The shadow-ray test. The angle test only asks which way the surface faces; it cannot know that another object stands between this point and the light. The shadow ray finds the occluder, the diffuse term is dropped, and only ambient remains — the same value as the surrounding shadow.

**4.** With `A 0.9` the whole image is much brighter, and shadows are pale grey instead of near-black. The sphere looks flatter because ambient is added equally at every point: it raises the floor brightness so high that the diffuse gradient between the lit and unlit sides becomes a small variation on top of a large constant. Low contrast between the lit cap and the dark side is exactly what destroys the perception of roundness.

**5.** The axis vector is being parsed but never used in the intersection maths — the cylinder's orientation is hardcoded, and it only looked correct because the original scene happened to match the hardcoded direction. This violates the requirement that the program apply translation and rotation transformations to objects, and it is the specific failure that requirement exists to catch.

</details>

---

## Further reading

- *Ray Tracing in One Weekend* — Peter Shirley. The canonical free introduction; covers ray/sphere intersection, normals and diffuse shading in the same order as these notes.
- *Scratchapixel* — the "Ray-Tracing: Rendering a Triangle / Geometry" lessons derive the ray–sphere and ray–plane equations in full algebraic detail.
- *Physically Based Rendering* (Pharr, Jakob, Humphreys) — far beyond miniRT's scope, but the reference for what ambient lighting is approximating and how real global illumination is computed.
- `man 3 math` — the C maths library, and a reminder to compile with `-lm`.
- The miniLibX documentation, for the image-buffer approach the subject strongly recommends over per-pixel drawing calls.
