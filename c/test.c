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
  for(int i=0; i<10000; i++)
  {
    float nval = pn_noise(i * 0.1f, 0, 0);
    printf("%f\n", nval);
  }
  return 0;
}

