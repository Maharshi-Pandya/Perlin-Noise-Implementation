#!/usr/bin/python3.9
# -*- coding: utf-8 -*-
import PerlinNoise

# the perlin noise instance
noise = PerlinNoise.PerlinNoise(3.14, 42.0 , 7.0)

# noise.val() returns the calculated noise value at the input point
print(noise.val())
