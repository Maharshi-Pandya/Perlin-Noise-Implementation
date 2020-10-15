#ifndef _PERLIN_NOISE_HEADER_
#define _PERLIN_NOISE_HEADER_

#define pn_noise _noise
#define pn_init _init_perm_table

// standard includes
#include <math.h>


// Permutation table to calculate the hashes for each corner
// of the unit cell. This contains ints from 0 to 255 inclusive.
// TODO: read the permutation table from file
int PERM_TABLE[256];
int P[512];
int perm_flag = 0;
void _init_perm_table(void)
{
  int tmp_num = -1;
  FILE *perm_file = fopen("PERMTABLE.txt", "r");
  if(perm_file == NULL)
  {
    exit(1);
  }

  // read the ints
  int idx = 0;
  while(!feof(perm_file))
  {
    fscanf(perm_file, "%d\n", &tmp_num);
    PERM_TABLE[idx] = tmp_num;
    idx++;
  }

  // double the permutation to avoid overflow
  for(int idx=0; idx<256; idx++)
  {
    P[256+idx] = P[idx] = PERM_TABLE[idx];
  }

  fclose(perm_file);
  // permtable created 
  perm_flag = 1;
}

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
  int *hash_arr = (int *)malloc(8*sizeof(int));
  // TODO: from the permutation table, create hash for every unit cell co-ord
  /*
    > There will be 8 hashes for each cell point
    
      hash_arr[0] : "aaa"
      hash_arr[1] : "baa"
      hash_arr[2] : "bba"
      hash_arr[3] : "aba"
      hash_arr[4] : "aab"
      hash_arr[5] : "bab"
      hash_arr[6] : "bbb"
      hash_arr[7] : "abb"
  */


  return hash_arr;
}

// Generate the noise value for the input co-ordinate
// if repeat is on, make sure the input co-ordinate
// map to their "local" co-ordinates i.e. make sure the co-ordinates
// wrap-around
float _noise(float inp_x, float inp_y, float inp_z)
{
  
  int rep_amt = -1;
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

  // // uncomment if you want the co-ordinates to overflow
  // // i.e. change them to their "local" co-ords.
  // rep_amt = <REPETITION_VALUE>
  
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

  u = _fadefunc(xf);
  v = _fadefunc(yf);

  // get the hashes of all the unit cell co-ords
  // int *hashes;
  int *hashes = _hash(xi, yi, zi);
  
  // free the memory allocated by the _hash function
  free(hashes);
}


#endif // _PERLIN_NOISE_HEADER_