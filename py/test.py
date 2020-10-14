#!/usr/bin/python3.9
# -*- coding: utf-8 -*-
import PerlinNoise
xoff = 0
for i in range(500):
    noise = PerlinNoise.OctavePerlin(xoff)
    nval = noise._calc()
    xoff += 0.1
    print(nval)
