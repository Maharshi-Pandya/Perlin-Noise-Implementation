#ifndef _PERLIN_NOISE_HEADER_
#define _PERLIN_NOISE_HEADER_

#define pn_noise _noise
#define pn_init _init_perm_table
#define pn_octave_noise _octave_noise

// standard includes
#include <stdio.h>
#include <math.h>

// // uncomment if you want the repeatition on
// // i.e. want to map the input point co-ordinates to their "local"
// // co-ords 
// int rep_amt = <REPEATITION_VALUE_HERE>
int rep_amt = -1;

// ✨ Permutation table to calculate the hashes for each corner
// of the unit cell. This contains ints from 0 to 255 inclusive.
// TODO: read the permutation table from file
int PERM_TABLE[256] = {151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};
int P[512];
int perm_flag = 0;
void _init_perm_table(void)
{
  // double the permutation to avoid overflow
  for(int idx=0; idx<256; idx++)
  {
    P[256+idx] = P[idx] = PERM_TABLE[idx];
  }
  // permtable created 
  perm_flag = 1;
}

// ✨ linearly interpolate between the lo and hi values
// priority given by the param t
float _lerp(float lo, float hi, float t)
{
  return lo + t * (hi -lo);
}

// ✨ calculate the dot product between the random chosen gradient vector
// and the distance vector
float _grad(int hash, float x_comp, float y_comp, float z_comp)
{
  // use the first 4 bits of the hash to generate 12 random vectors
  // and "dot" them with (x_comp, y_comp, z_comp)

  int h = hash & 0xF;
  float w = h < 8 /* 0b1000 */ ? x_comp : y_comp;

  float t = h < 4 /* 0b100 */ ? y_comp : (h == 12 || h == 14 ? x_comp : z_comp);

  // from the first two bits decide if w or t are positive or negative
  return ((h&1) == 0 ? w : -w) + ((h&2) == 0? t: -t);
}

// ✨ fade function which has slope of zero as it reaches the extremes
// 0 or 1. 
// this is for the smoothness in the noise value while interpolating
float _fadefunc(float tf)
{
  return tf*tf*tf*(tf*(6*tf - 15) + 10); 
}

// increment the num and wrap around the rep_amt value 
int _inc(int num)
{
  num++;
  num = rep_amt > 0 ? num%rep_amt : num;
  return num;
}

// ✨ calculates the hashes of all the unit cell co-ords
// stores them into the hash_arr and returns a pointer to the array
int* _hash(int xi, int yi, int zi)
{ 
  int *hash_arr = (int *)malloc(8*sizeof(int));
  // TODO: from the permutation table, create hash for every unit cell co-ord
  /*
    > There will be 8 hashes for each cell point.
      Here's the mapping:

      [0] : "aaa"
      [1] : "baa"
      [2] : "bba"
      [3] : "aba"
      [4] : "aab"
      [5] : "bab"
      [6] : "bbb"
      [7] : "abb"
  */
  hash_arr[0] /*aaa*/ = P[P[P[xi] + yi] + zi];
  hash_arr[1] /*baa*/ = P[P[P[_inc(xi)] + yi] + zi];
  hash_arr[2] /*bba*/ = P[P[P[_inc(xi)] + _inc(yi)] + zi];
  hash_arr[3] /*aba*/ = P[P[P[xi] + _inc(yi)] + zi];
  hash_arr[4] /*aab*/ = P[P[P[xi] + yi] + _inc(zi)];
  hash_arr[5] /*bab*/ = P[P[P[_inc(xi)] + yi] + _inc(zi)];
  hash_arr[6] /*bbb*/ = P[P[P[_inc(xi)] + _inc(yi)] + _inc(zi)];
  hash_arr[7] /*abb*/ = P[P[P[xi] + _inc(yi)] + _inc(zi)];

  return hash_arr;
}

// ✨ Generate the noise value for the input co-ordinate
// if repeat is on, make sure the input co-ordinate
// map to their "local" co-ordinates i.e. make sure the co-ordinates
// wrap-around
float _noise(float inp_x, float inp_y, float inp_z)
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
  float u, v, w;     // for fading the *f values

  if(rep_amt>0)
  {
    x = remainderf(x, (float)rep_amt);
    y = remainderf(y, (float)rep_amt);
    z = remainderf(z, (float)rep_amt);
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

  // fade the *f for smoother interpolation
  u = _fadefunc(xf);
  v = _fadefunc(yf);
  w = _fadefunc(zf);

  // get the hashes of all the unit cell co-ords
  // int *hashes;
  int *hashes = _hash(xi, yi, zi);
  
  // calculate the dot product between the gradient vectors 
  // and the distance vectors and linearly interpolate between them
  // ...

  float x1 = _lerp(_grad(hashes[0], xf, yf, zf), 
  _grad(hashes[1], xf-1, yf, zf), u);

  float x2 = _lerp(_grad(hashes[3], xf, yf-1, zf), 
  _grad(hashes[2], xf-1, yf-1, zf), u);

  float y1 = _lerp(x1, x2, v);  // 1

  // no need to redefine can overwrite the previously 
  // "lerp-ed" values safely
  x1 = _lerp(_grad(hashes[4], xf, yf, zf-1), 
  _grad(hashes[5], xf-1, yf, zf-1), u);

  x2 = _lerp(_grad(hashes[7], xf, yf-1, zf-1), 
  _grad(hashes[6], xf-1, yf-1, zf-1), u);

  float y2 = _lerp(x1, x2, v);  // 2

  // free the memory allocated by the _hash function
  free(hashes);
  
  // lerp the two y values and map em in the range [0, 1]
  return (_lerp(y1, y2, w) + 1) / 2;
}

// Generate more "noisy" noise using octaves
// this is done by adding contributions of the noise function
// iteratively and changing the amplitude and frequency of inputs
float _octave_noise(float inp_x, float inp_y, float inp_z)
{
  // The octave count and the persistance of each octave
  int octaveCount = 8;
  float mulFreq = 2.f;
  float persistance = .5f;
  // "summed" noise, frequency and the max amplitude
  float noiseSum = 0.f;
  float currFreq = 1.f;
  float maxAmp = 0.f;
  float currAmp = 1.0f;

  // iterate through the octaves
  for(int i=0; i<octaveCount; i++)
  {
    float currNoise = _noise(inp_x*currFreq, inp_y*currFreq, inp_z*currFreq) * currAmp;
    noiseSum += currNoise;
    // resultant value will be in range [0, 1]
    maxAmp += currAmp;
    // increase the freq and decrease the amplitude
    currFreq *= mulFreq;
    currAmp *= persistance;
  }
  // map value in range [0, 1]
  return (noiseSum / maxAmp);
}

// That's all folks!

#endif // _PERLIN_NOISE_HEADER_
