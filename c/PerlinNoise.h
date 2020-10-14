#ifndef _PERLIN_NOISE_HEADER_
#define _PERLIN_NOISE_HEADER_

#define pn_noise noise

// standard includes
#include <math.h>

// Generate the noise value for the input co-ordinate
// if repeat is on, make sure the input co-ordinate
// map to their "local" co-ordinates i.e. make sure the co-ordinates
// wrap-around
float noise(float inp_x, float inp_y, float inp_z, int rep_amt=-1)
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
}


#endif // _PERLIN_NOISE_HEADER_