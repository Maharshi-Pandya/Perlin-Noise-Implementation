## Perlin Noise Generation 

<hr>

- Important thing to note here is that, if a noise function `PerlinNoise`  exists for 2 dimensions such that

  ```python
  PerlinNoise(x, y)
  ```

  then, 

  ```python
  # the noise val for point x=3, y=0 in 2-d
  noise_val = PerlinNoise(3, 0)
  # the noise val for point x=3 in 1-d
  noise_val2 = PerlinNoise(3)
  
  print(noise_val == noise_val2)
  ```

  the output of the above snippet will be `true`.

### Generation Logic

<hr>
Perlin Noise (for 2-D and 1-D) can be generated using vector operations on a 2-D grid.

Firstly to find the Perlin Noise value for a co-ordinate in space, we locate the co-ordinate inside of a unit square.

>For eg.
>
>if the co-ordinate is (x, y) we calculate its location inside a unit square by (x, y) % 1.0

<img src="./assets/logic01.gif" alt="unit_square" />

Above image shows the point inside of the unit square.

Each unit square co-ordinate, we generate what is called a pseudorandom gradient vector. By pseudorandom, it means that for any set of integers inputted in the gradient vector equation, the same gradient vector will be generated, i.e. its not entirely random. So, every integral co-ordinate has its own gradient vector which doesnot change if the gradient vector function doesnot change.

<img src="./assets/logic02.gif">



Example gradient vectors.

>Note
>
>In Ken Perlin's Improved algorithm, the gradient vectors are not completely random. Instead, they are chosen from vectors, that point from the unit square co-ordinate to the center of the unit square.
>
>i.e. A random vector is chosen from 
>
>{
>
>​	(1, 1), (-1, 1)
>
>​	(1, -1), (-1, -1)
>
>}
>
>In this implementation, will be doing the same.

Next, we calculate the distance vectors from the unit square point to our input point as shown below.

<img src="./assets/logic03.gif">

We, then need to compute the dot product between the distance vector and the gradient vector to get values at the four corners so we can "fade" the resulting values thus getting a smoothed out curve.

```python
# Below are the 4 values from the dot product
# d1 | d2
# --------
# d3 | d4

# u and v are the "faded" co-ordinates of our point in the unit square
# (xf, yf) = location of our point inside the unit square
u, v = fade(xf), fade(yf)

x1 = lerp(d1, d2, u)
x2 = lerp(d3, d4, u)

avg = lerp(x1, x2, v)
```

The "fade" function will be applied to `u` and `v` as this makes changes more gradual as one approaches integral co-ords.



Logically, thats all we want.