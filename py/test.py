#!/usr/bin/python3.9
# -*- coding: utf-8 -*-
import os
import sys
import PerlinNoise

# TODO: test in console using ascii
w_width = os.get_terminal_size().lines - 2
w_height = os.get_terminal_size().columns
# travel through the noise space in time
zoff: float = 0.0


def perlinDraw():
    global zoff
    # the xoff and yoff
    dr_char: str = "."
    xoff: float = 0.0
    yoff: float = 0.0
    inc: float = 0.1
    for x in range(w_width):
        xoff += inc
        yoff = 0.0  # for every xoff, yoff will start at 0
        for y in range(w_height):
            noise = PerlinNoise.PerlinNoise(xoff, yoff, zoff)
            dr_char = "#" if noise.val() < 0.5 else "."
            sys.stdout.write(dr_char)
            yoff += inc
        print()
    # increment the zoff by timestep
    zoff += 0.08
    # bring the cursor back at top for dynamic updating
    sys.stdout.write(f"\033[{w_width}A")


# loop (Ctrl-C to exit)
while 1:
    try:
        perlinDraw()
    except KeyboardInterrupt:
        print()
        # bring the cursor at bottom to stop updating
        sys.stdout.write(f"\033[{w_width}B")
        sys.stdout.write("\nAight. Imma head out...\n\n")
        sys.exit(1)

