# MiniLibX: What Every Function Does

> These notes explain every function in MiniLibX (MLX), the small graphics library miniRT uses to open a window, show pixels, and react to the keyboard and mouse. They were written from the two copies shipped in this repo, `lib/mlx_linux` (X11) and `lib/mlx_macos` (OpenGL/AppKit), so every platform difference noted here comes from their actual source. No prior graphics knowledge is assumed. The notes describe how the library behaves; they don't write miniRT's code for you.

**Written:** 2026-09-17
**Sources:** `lib/mlx_linux/mlx.h`, `lib/mlx_macos/mlx.h` and their `.c` / `.m` files

---

## Table of contents

1. [The big picture](#1-the-big-picture)
2. [Three words you need first: pointer handles, colors, callbacks](#2-three-words-you-need-first)
3. [Starting up: `mlx_init`](#3-starting-up)
4. [Windows](#4-windows)
5. [Drawing pixels directly: `mlx_pixel_put`](#5-drawing-pixels-directly)
6. [Images: drawing off-screen](#6-images-drawing-off-screen)
7. [Inside an image: the pixel buffer](#7-inside-an-image-the-pixel-buffer)
8. [Colors: `mlx_get_color_value`](#8-colors)
9. [Loading pictures from files](#9-loading-pictures-from-files)
10. [Text](#10-text)
11. [Events: making the program react](#11-events-making-the-program-react)
12. [The loop](#12-the-loop)
13. [The mouse](#13-the-mouse)
14. [Keyboard auto-repeat and syncing](#14-keyboard-auto-repeat-and-syncing)
15. [Cleaning up](#15-cleaning-up)
16. [Linux vs macOS at a glance](#16-linux-vs-macos-at-a-glance)
17. [Self-check](#17-self-check)

---

## 1. The big picture

Your program can't draw on the screen by itself. The screen belongs to the operating system's **display system**:

- **Linux:** the **X server** (X11), a separate program your code talks to over a connection.
- **macOS:** **AppKit** (windows, keyboard, mouse) and **OpenGL** (drawing).

MiniLibX is a thin translator that hides these differences behind about 30 simple C functions:

```
   ┌──────────────┐     simple calls      ┌───────────┐    complex calls    ┌──────────────────┐
   │  miniRT      │ ────────────────────► │ MiniLibX  │ ──────────────────► │ X11  or  AppKit/ │
   │  (your code) │ ◄──────────────────── │           │ ◄────────────────── │ OpenGL           │
   └──────────────┘   "a key was pressed" └───────────┘    raw OS events    └──────────────────┘
```

Every MLX program follows the same life cycle:

```
  mlx_init ──► create window(s) and image(s) ──► fill the image with pixels
      │
      ▼
  put the image in the window ──► register event handlers (hooks) ──► mlx_loop
                                                                         │
                                            (program now waits for events)
                                                                         │
                                  ESC / red cross ──► destroy everything ──► exit
```

---

## 2. Three words you need first

### 2.1 Handles: `void *` pointers you never look inside

Most MLX functions return a `void *`, for example `mlx_init` and `mlx_new_window`. That pointer is a **handle**: a "ticket" identifying something MLX created. You don't read what it points to. You just keep it and give it back to MLX later.

```
   void *mlx  ───►  [ MLX's private connection data ]   (you never open this)
   void *win  ───►  [ MLX's private window data ]
   void *img  ───►  [ MLX's private image data ]
```

There are three kinds of handle:

| Handle | Created by | Means |
|---|---|---|
| `mlx_ptr` | `mlx_init` | "my connection to the display system" |
| `win_ptr` | `mlx_new_window` | "this particular window" |
| `img_ptr` | `mlx_new_image`, `mlx_xpm_*_to_image`, `mlx_png_file_to_image` | "this particular image" |

A function that creates a handle returns **`NULL`** (`(void *)0`) when it fails, so always check.

### 2.2 Colors as one integer: `0xTTRRGGBB`

MLX describes a color as a single `int` written in **hexadecimal** (base 16, prefix `0x`). Each pair of hex digits is one **byte** (a value from 0 to 255):

```
        0x  00   FF   80   00
            ──   ──   ──   ──
            T    R    G    B
            │    │    │    └── blue  = 0x00 =   0
            │    │    └─────── green = 0x80 = 128
            │    └──────────── red   = 0xFF = 255
            └───────────────── top byte (see below)
                               → an orange
```

The header says: *"color is 0x00RRGGBB"*. **Keep the top byte at `00`.** On macOS, MLX treats that byte as **transparency** in images: `00` = fully visible, `FF` = invisible. On Linux it's ignored.

| Color | Hex |
|---|---|
| black | `0x00000000` |
| white | `0x00FFFFFF` |
| pure red | `0x00FF0000` |
| the disc in the miniRT example | about `0x00C83219` |

### 2.3 Callbacks (hooks): "call me when…"

A **callback** is a function you write but **don't call yourself**. You give MLX its address, and MLX calls it later when something happens (a key press, a mouse click…). Registering a callback is called **hooking**.

```
   you:  "MLX, when a key is released, call my_key_function, and hand it THIS pointer."
                                       │                                    │
                                   function pointer                      void *param
   ...later, the user presses ESC...
   MLX:  my_key_function(65307, param);
```

The last argument of every hook function, `void *param`, is **any pointer you want**. MLX doesn't look at it; it just passes it back to your callback. This is how your callback reaches your program's data (the MLX handles, the scene…) without global variables, which the 42 Norm forbids.

---

## 3. Starting up

### `void *mlx_init();`

**What it does:** opens the connection to the display system and prepares MLX's internal bookkeeping.

- **Linux:** connects to the X server (the `DISPLAY` environment variable says which one).
- **macOS:** sets up the application object that receives keyboard and mouse events.

**Returns:** the `mlx_ptr` handle, or `NULL` if the connection failed (e.g. on Linux over SSH without a display).

**Rule:** call it **first**. Every other MLX function needs its result, directly or indirectly.

---

## 4. Windows

### `void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);`

**What it does:** creates a window `size_x` pixels wide and `size_y` pixels tall, with `title` in its title bar.

**Returns:** the `win_ptr` handle, or `NULL` on failure.

**Coordinates inside the window** (from the header comment):

```
   (0,0) ───────────────────► x
     │
     │        window
     │
     ▼
     y          (size_x − 1, size_y − 1) is the bottom-right pixel
```

The origin is the **top-left** corner, and **y grows downward**. This is the same convention as the pixel grid in `viewport-and-pixel-mapping.md`.

> **macOS:** the window **doesn't appear until `mlx_loop` starts**. That's normal (the header says so). Enter `mlx_loop` as soon as possible.

You can create several windows from the same `mlx_ptr`.

### `int mlx_clear_window(void *mlx_ptr, void *win_ptr);`

**What it does:** paints the whole window **black**, erasing everything drawn in it.

### `int mlx_destroy_window(void *mlx_ptr, void *win_ptr);`

**What it does:** closes the window and frees what MLX allocated for it. After this, `win_ptr` is invalid: don't use it again. See [section 15](#15-cleaning-up) for the full cleanup order.

---

## 5. Drawing pixels directly

### `int mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color);`

**What it does:** colors **one pixel** of the window at `(x, y)` with `color`, **immediately**.

**Why it's a trap for miniRT:** on Linux, every call is a separate request sent to the X server. A 1920 × 1080 window has **2,073,600** pixels, so that's two million round trips, and you'd watch the image paint itself line by line. It's also drawn straight onto the window, so if the window is covered and uncovered, the drawing is lost.

```
   mlx_pixel_put, 2 million times:
   program ──px──► X server
   program ──px──► X server          slow, and nothing is kept
   program ──px──► X server
   ...

   image approach (section 6):
   program: write 2 million pixels into memory  (fast, no messages)
   program ──whole image, once──► X server
```

That's why the next section exists.

---

## 6. Images: drawing off-screen

An **image** is a rectangle of pixels stored **in memory**, not on screen. You fill it at full speed, then show it in one go.

### `void *mlx_new_image(void *mlx_ptr, int width, int height);`

**What it does:** allocates a new image of `width × height` pixels. Its content starts undefined, so write every pixel before showing it.

**Returns:** the `img_ptr` handle, or `NULL` on failure.

An image isn't tied to a window: you create it from `mlx_ptr` only, so it doesn't matter whether you create the image or the window first.

### `char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);`

**What it does:** gives you the **memory address of the image's pixels**, so you can write into them directly. It also **fills in three variables** you pass by address, which describe how the pixels are laid out:

| Output | Meaning | Typical value |
|---|---|---|
| **return value** | address of the first byte of pixel (0, 0) | a `char *` |
| `*bits_per_pixel` | how many **bits** one pixel takes | 32 (so 4 bytes) |
| `*size_line` | how many **bytes** one row of pixels takes | 4 × width (macOS: exactly that) |
| `*endian` | in which **order** the bytes of a color are stored | 0 (see section 7) |

**Why pass pointers?** A C function can return only one value. To hand back four pieces of information, it returns one (the address) and writes the other three into variables **you** own, whose addresses you give it.

Section 7 explains how to use these values.

### `int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y);`

**What it does:** copies the image into the window, with the image's top-left corner at window position `(x, y)`. For a full-window image that's `(0, 0)`.

Writing into the image's memory does **not** update the screen. Nothing appears until you call this function. Call it again whenever the image changes, or when the window needs redrawing (see *expose* in section 11).

### `int mlx_destroy_image(void *mlx_ptr, void *img_ptr);`

**What it does:** frees the image and its pixel memory. The address from `mlx_get_data_addr` becomes invalid too.

---

## 7. Inside an image: the pixel buffer

This section explains the **concept** of the memory layout, which you need in order to understand the values from `mlx_get_data_addr`.

### 7.1 The image is one long line of bytes

Memory has no rows or columns: it's a single numbered line of bytes. The 2D image is stored **row after row**:

```
 image (width 3, height 2)          memory, starting at the returned address
 ┌─────┬─────┬─────┐
 │ p00 │ p10 │ p20 │  row 0  ──►   [ p00 | p10 | p20 | p01 | p11 | p21 ]
 ├─────┼─────┼─────┤                 └──── row 0 ────┘ └──── row 1 ────┘
 │ p01 │ p11 │ p21 │  row 1  ──►
 └─────┴─────┴─────┘                pXY = pixel at column X, row Y
```

And each pixel is itself **4 bytes** (32 bits ÷ 8):

```
   one pixel = [ byte | byte | byte | byte ]
```

### 7.2 Finding a pixel's first byte

To reach pixel `(x, y)`, count how many bytes come before it:

- **Full rows above it:** there are `y` of them, and each takes `size_line` bytes → `y × size_line` bytes.
- **Pixels to its left, in its own row:** there are `x` of them, each `bits_per_pixel / 8` bytes → `x × (bits_per_pixel / 8)` bytes.

```
offset of pixel (x, y) = y × size_line  +  x × (bits_per_pixel / 8)
```

**Worked example:** a 1920-wide image, `bits_per_pixel = 32`, `size_line = 7680`. Where does pixel (2, 1) start?

```
rows above:     1 × 7680        = 7680
pixels before:  2 × (32 / 8)    = 2 × 4 = 8
offset:                           7688
```

So pixel (2, 1) starts **7688 bytes** after the returned address.

> **Why use `size_line` and not `4 × width`?** A library is allowed to add unused **padding** bytes at the end of each row, for its own alignment reasons. `size_line` is the real row length, padding included. On macOS it's exactly `4 × width`; relying on the value MLX gives you is what keeps your code correct everywhere.

### 7.3 Which byte is which color: endianness

A color like `0x00FF8000` is 4 bytes. **Endianness** is the order those bytes are stored in memory:

- **Little endian** (`endian == 0`): the **least** significant byte comes **first**. This is what Intel/AMD and Apple Silicon processors use.
- **Big endian** (`endian == 1`): the **most** significant byte comes first.

```
 color 0x00FF8000  (T=00, R=FF, G=80, B=00)

 little endian (endian = 0):   [ 00 | 80 | FF | 00 ]
                                  B    G    R    T
 big endian    (endian = 1):   [ 00 | FF | 80 | 00 ]
                                  T    R    G    B
```

The macOS source hard-codes `*endian = 0` (little endian). If you write the **whole 4-byte color at once** as an integer at that address, the processor stores the bytes in its own order automatically, and on a little-endian machine that already matches MLX.

---

## 8. Colors

### `int mlx_get_color_value(void *mlx_ptr, int color);`
*(returns `unsigned int` on macOS)*

**What it does:** converts a `0x00RRGGBB` color into the exact bit pattern the display expects.

- **Linux:** if the display uses 24 bits or more per color (virtually every modern screen), it **returns the color unchanged**. It only rewrites it for old, low-color displays.
- **macOS:** also returns it effectively unchanged.

In practice you'll rarely need it on current machines, but it's the "official" way to make a color display-safe.

---

## 9. Loading pictures from files

miniRT computes every pixel itself, so these functions matter less than in so_long. They're listed for completeness.

### `void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height);`

**What it does:** reads an **XPM** file (a simple image format stored as text that looks like C source) and turns it into a new MLX image. It writes the picture's size into `*width` and `*height`.

**Returns:** an `img_ptr`, or `NULL` if the file can't be read. The header warns that XPM support is **incomplete**, so some files may fail.

### `void *mlx_xpm_to_image(void *mlx_ptr, char **xpm_data, int *width, int *height);`

**What it does:** same as above, but the XPM is already in your program as an array of strings (e.g. from `#include "picture.xpm"`) instead of a file on disk.

### `void *mlx_png_file_to_image(void *mlx_ptr, char *file, int *width, int *height);`

**macOS only.** Same idea for **PNG** files. Not available in `mlx_linux`.

---

## 10. Text

### `int mlx_string_put(void *mlx_ptr, void *win_ptr, int x, int y, int color, char *string);`

**What it does:** writes `string` in the window at `(x, y)` in `color`. Like `mlx_pixel_put`, it draws **directly on the window**, not in an image. So if you then call `mlx_put_image_to_window`, the image covers the text. Put the text **after** the image.

### `void mlx_set_font(void *mlx_ptr, void *win_ptr, char *name);`

**Linux only.** Picks the X11 font used by `mlx_string_put`, by its X font name.

---

## 11. Events: making the program react

An **event** is something the user or the system does to your window: pressing a key, clicking, moving the mouse, uncovering the window, clicking the red close button. Each hook function below says **which event** should trigger **which callback**.

### 11.1 The shortcut hooks

These three are convenience versions. Each one hooks one common event:

| Function | Callback is called when… | Callback receives |
|---|---|---|
| `mlx_key_hook(win_ptr, f, param)` | a key is **released** | `f(int keycode, void *param)` |
| `mlx_mouse_hook(win_ptr, f, param)` | a mouse button is **pressed** (or the wheel scrolls) | `f(int button, int x, int y, void *param)` |
| `mlx_expose_hook(win_ptr, f, param)` | the window must be **redrawn** | `f(void *param)` |

Details that surprise people:

- **`mlx_key_hook` fires on key *release*, not press.** Both copies in this repo do this: Linux registers X11's `KeyRelease`, and macOS registers event 3 (key up). To react the moment a key goes down, use `mlx_hook` with event 2 (below).
- **Keycodes differ by platform.** The same physical key has a different number:

  | Key | Linux (X11 keysym) | macOS |
  |---|---|---|
  | ESC | 65307 | 53 |
  | W / A / S / D | 119 / 97 / 115 / 100 | 13 / 0 / 1 / 2 |
  | ← / → | 65361 / 65363 | 123 / 124 |

- **Mouse button numbers differ too:**

  | Button | Linux | macOS |
  |---|---|---|
  | left | 1 | 1 |
  | middle | 2 | 3 |
  | right | 3 | 2 |
  | wheel up / down | 4 / 5 | 4 / 5 |

- **Expose** means "part of your window was hidden and is visible again, and its pixels were lost". A typical response is to put the image back in the window. (On macOS, the OpenGL backend keeps the content, so this matters mostly on Linux.)

### 11.2 The general hook

### `int mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct)(), void *param);`

**What it does:** hooks **any** event by its number. The three shortcuts above are just special cases of this one.

- `x_event`: **which** event (a number taken from X11's `X.h`).
- `x_mask`: on **Linux**, tells the X server "send me this kind of event". Without the right mask, X never delivers the event. **macOS ignores it.**
- `funct`, `param`: the callback and your pointer.

The events you're most likely to need:

| `x_event` | Name | Happens when | Linux mask | Callback receives |
|---|---|---|---|---|
| 2 | KeyPress | key pressed down | `1L<<0` | `(int keycode, void *param)` |
| 3 | KeyRelease | key released | `1L<<1` | `(int keycode, void *param)` |
| 4 | ButtonPress | mouse button down | `1L<<2` | `(int button, int x, int y, void *param)` |
| 5 | ButtonRelease | mouse button up | `1L<<3` | `(int button, int x, int y, void *param)` |
| 6 | MotionNotify | mouse moved | `1L<<6` | `(int x, int y, void *param)` |
| 12 | Expose | window must be redrawn | `1L<<15` | `(void *param)` |
| 17 | DestroyNotify | **red close button** clicked | `1L<<17` (often written `0`) | `(void *param)` |

`1L<<n` means "the number 1 shifted left by n bits", a number with only bit n set. Each kind of event has its own bit.

**Event 17 is the one the miniRT subject asks for:** clicking the window's red cross must quit cleanly. The Linux source handles the window manager's "close" message and calls whatever you hooked on event 17. macOS calls it from its close notification.

---

## 12. The loop

### `int mlx_loop(void *mlx_ptr);`

**What it does:** starts the **event loop**, an endless cycle of *"wait for an event → call the matching callback → repeat"*:

```
         ┌──────────────────────────────────────┐
         ▼                                      │
   wait for an event ──► find its window ──► call its hook (if any)
                                                │
                    no event pending? ──► call the loop hook (if any)
```

After you call `mlx_loop`, **your code only runs inside callbacks.** Anything written after `mlx_loop(...)` in your function isn't reached while the program is running.

When does it return?

- **Linux:** when **no window is left**, or after `mlx_loop_end` is called.
- **macOS:** **never**. It hands control to AppKit (`[NSApp run]`). To quit, a callback must end the program itself (with `exit`, which the miniRT subject allows).

### `int mlx_loop_hook(void *mlx_ptr, int (*funct)(), void *param);`

**What it does:** registers a callback that runs **over and over** whenever there's no event to handle, which is useful for animation. Note that it's attached to `mlx_ptr`, not to a window. A static image like miniRT's usually doesn't need it.

### `int mlx_loop_end(void *mlx_ptr);`

**Linux only.** Asks `mlx_loop` to stop. The loop finishes its current step and then **returns**, so the code after `mlx_loop(...)` runs. That's a clean place to free everything.

---

## 13. The mouse

These functions **query or control the pointer**. Their argument lists **differ between the two copies**:

| Function | Linux signature | macOS signature | What it does |
|---|---|---|---|
| get position | `mlx_mouse_get_pos(mlx, win, &x, &y)` | `mlx_mouse_get_pos(win, &x, &y)` | writes the pointer's position (relative to the window) into x and y |
| move | `mlx_mouse_move(mlx, win, x, y)` | `mlx_mouse_move(win, x, y)` | moves the pointer to (x, y) in the window |
| hide | `mlx_mouse_hide(mlx, win)` | `mlx_mouse_hide()` | makes the pointer invisible |
| show | `mlx_mouse_show(mlx, win)` | `mlx_mouse_show()` | makes it visible again |

Code that calls these with one platform's arguments won't compile on the other.

### `int mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey);`

**Linux only** in this repo. Writes the **screen's** resolution (not the window's) into `*sizex` and `*sizey`. That's handy for making sure a window isn't larger than the display.

---

## 14. Keyboard auto-repeat and syncing

### `int mlx_do_key_autorepeatoff(void *mlx_ptr);` / `mlx_do_key_autorepeaton(void *mlx_ptr);`

**What they do:** turn **auto-repeat** off or on. Auto-repeat is the behavior where holding a key sends the same key event again and again. With it off, holding a key produces only one press.

> **Linux caution:** this changes the setting for the **whole X session**, not just your window, and it stays that way after your program exits unless you turn it back on.

### `int mlx_do_sync(void *mlx_ptr);`

**What it does:** forces all pending drawing requests to be sent and processed **right now**. The display system normally batches them for speed. The macOS header mentions it as a (not great) way to make a window appear before `mlx_loop`.

---

## 15. Cleaning up

Everything MLX creates must be released, or valgrind and the evaluators will see leaks.

| Created with | Released with |
|---|---|
| `mlx_new_image`, `mlx_xpm_*_to_image`, `mlx_png_file_to_image` | `mlx_destroy_image` |
| `mlx_new_window` | `mlx_destroy_window` |
| `mlx_init` | `mlx_destroy_display` (**Linux only**), then `free` |

### `int mlx_destroy_display(void *mlx_ptr);`

**Linux only.** Closes the connection to the X server. It does **not** free the `mlx_ptr` memory itself: the Linux source only calls `XCloseDisplay`. You free that pointer yourself afterwards.

**This function doesn't exist in `lib/mlx_macos`.** Code that calls it fails to link on macOS. (This is the known macOS build failure noted in the project's `CLAUDE.md`.) On macOS there's no equivalent teardown for `mlx_ptr`.

### Order

Each step depends on the ones after it still being valid, so release in the **reverse order of creation**:

```
   created:    mlx_init ──► window / image
   destroyed:  image / window ──► display ──► free(mlx_ptr)
```

Using a handle after destroying what it points to is a **use-after-free**. For example, calling `mlx_destroy_window` after `mlx_destroy_display` on Linux talks to a connection that's already closed.

---

## 16. Linux vs macOS at a glance

| Topic | `lib/mlx_linux` | `lib/mlx_macos` |
|---|---|---|
| Link flags (from the Makefile) | `-lXext -lX11 -lm -lz` | `-framework OpenGL -framework AppKit` |
| `mlx_destroy_display` | ✅ | ❌ |
| `mlx_loop_end` | ✅ | ❌ |
| `mlx_get_screen_size` | ✅ | ❌ |
| `mlx_set_font` | ✅ | ❌ |
| `mlx_png_file_to_image` | ❌ | ✅ |
| Mouse get/move/hide/show | take `mlx_ptr` + `win_ptr` | shorter argument lists |
| `mlx_loop` returns? | yes (no windows left, or `mlx_loop_end`) | never |
| Window visible before `mlx_loop`? | yes | no |
| `x_mask` in `mlx_hook` | required | ignored |
| ESC keycode | 65307 | 53 |
| Right mouse button | 3 | 2 |
| Top color byte in images | ignored | transparency (00 = opaque) |

---

## 17. Self-check

<details>
<summary>1. Why is <code>mlx_pixel_put</code> a bad choice for rendering a full miniRT frame?</summary>

Each call is a separate request to the display system (on Linux, a message to the X server), and there are about 2 million pixels in a 1920 × 1080 window. It's slow, and the drawing isn't kept if the window gets covered. Writing into an image and calling `mlx_put_image_to_window` once avoids both problems.

</details>

<details>
<summary>2. You wrote pixels into the image's memory, but the window is still black. What's missing?</summary>

`mlx_put_image_to_window`. Writing into the buffer only changes memory. Nothing reaches the screen until the image is put into the window (and, on macOS, until `mlx_loop` is running).

</details>

<details>
<summary>3. <code>size_line</code> is 7680 and <code>bits_per_pixel</code> is 32. At what offset does pixel (10, 3) start?</summary>

3 × 7680 + 10 × 4 = 23040 + 40 = **23080** bytes after the returned address.

</details>

<details>
<summary>4. Your ESC handler registered with <code>mlx_key_hook</code> works on your Linux machine but does nothing on a Mac. Name two possible reasons.</summary>

(1) The keycode: ESC is 65307 on Linux and 53 on macOS. (2) If the code also calls a Linux-only function (like `mlx_destroy_display`), it won't even build on macOS.

</details>

<details>
<summary>5. Which event number must you hook to handle the red close button, and does the mask matter on macOS?</summary>

Event **17** (DestroyNotify). The mask is ignored on macOS. On Linux it's commonly passed as `1L<<17` (or `0`); the Linux loop checks for the close message and calls the event-17 hook.

</details>

<details>
<summary>6. What is <code>void *param</code> for in every hook function?</summary>

It's any pointer you choose. MLX passes it back unchanged to your callback, which is how the callback reaches your program's data (MLX handles, scene…) without global variables.

</details>

<details>
<summary>7. Code placed after <code>mlx_loop(mlx)</code> never runs on macOS. Why?</summary>

On macOS `mlx_loop` never returns: it hands control to AppKit's run loop. From then on, your code only runs inside callbacks, so quitting has to happen from a callback (e.g. with `exit`).

</details>
