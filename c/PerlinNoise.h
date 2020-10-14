#ifndef _PERLIN_NOISE_HEADER_
#define _PERLIN_NOISE_HEADER_

#define pn_noise _noise

// standard includes
#include <math.h>

// linearly interpolate between the lo and hi values
// priority given by the param t
float _lerp(float lo, float hi, float t)
{
  return lo + t * (hi -lo);
}

// calculate the dot product between the random chosen gradient vector
// and the distance vector
float _grad(int hash, float x_comp, float y_comp, float z_comp)
{
  // ...
}

// fade function which has slope of zero as it reaches the extremes
// 0 or 1. 
// this is for the smoothness in the noise value while interpolating
float _fadefunc(float tf)
{
  return tf*tf*tf*(tf*(6*tf - 15) + 10); 
}

// calculates the hashes of all the unit cell co-ords
// stores them into the hash_arr and returns a pointer to the array
int* _hash(int xi, int yi, int zi)
{ 
  static int hash_arr[8];

  // calculate all the hashes here....

  return hash_arr;
}

// Generate the noise value for the input co-ordinate
// if repeat is on, make sure the input co-ordinate
// map to their "local" co-ordinates i.e. make sure the co-ordinates
// wrap-around
float _noise(float inp_x, float inp_y, float inp_z, int rep_amt=-1)
{
  float x = inp_x;
  float y = inp_y;
  float z = inp_z;
  // the *i represent the co-ordinates of the unit cell in which 
  // our input point is located.
  // the *f represent the relative co-ordinates of input point 
  // relative to the unit cell i.e. (0.5, 0.5, 0.5) will be at the center
  // of the unit cell
  int xi, yi, zi;
  float xf, yf, zf;
  float u, v;     // for fading the *f values

  // when the repeatition is on
  if(rep_amt>0)
  {
    x = remainder(x, (float)rep_amt);
    y = remainder(y, (float)rep_amt);
    z = remainder(z, (float)rep_amt);
  }
  // init the *i and *f
  // *i are bound to 255 to avoid overflow while creating the hashes i.e.
  // accessing the P[] array
  xi = (int)x & 255;
  yi = (int)y & 255;
  zi = (int)z & 255;

  xf = x - (int)x;
  yf = y - (int)y;
  zf = z - (int)z;

  u = _fadefunc(xf);
  v = _fadefunc(yf);
}


#endif // _PERLIN_NOISE_HEADER_