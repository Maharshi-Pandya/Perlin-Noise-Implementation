// The test file for PerlinNoise 
#include <stdio.h>
#include <stdlib.h>
#include "PerlinNoise.h"

int main(int argc, char const *argv[])
{
  /* code */

  // activate!
  pn_init();

  // call the noise function
  float x = pn_noise(.5, .5, .5); 
  float y = pn_noise(.4, .4, .4);
  return 0;
}

