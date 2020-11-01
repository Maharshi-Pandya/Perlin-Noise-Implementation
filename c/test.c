// The test file for PerlinNoise 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>   // ctrl-c handling
#include "PerlinNoise.h"

// hard-coding rn
int w_rows=24, w_cols=80;

// TODO: use the console and ascii to test the header
float zoff = 0.f;   // travel through noise space in time
void perlinDrawConsole(void)
{
  // xoff and yoff for 2-D generation
  float xoff = 0.f, yoff = 0.f;
  float inc = 0.1f;

  for(int x=0; x<w_rows; x++)
  {
    xoff += inc;
    yoff = 0;   // for every xoff, yoff starts at zero
    for(int y=0; y<w_cols; y++)
    {
      float noiseval = pn_noise(xoff, yoff, zoff);
      noiseval < 0.5 ? printf(".") : printf("#");
      yoff += inc;
    }
    printf("\n");
  }
  // increment the time step
  zoff += 0.001f;
  // go back height lines
  printf("\033[24A");
}
void ctrlcHandler(int signum)
{
  // go down w_rows lines
  printf("\033[%dB", w_rows);
  printf("\nAight. Imma head out...\n\n");
  exit(signum);
}

int main(int argc, char const *argv[])
{
  signal(SIGINT, ctrlcHandler);

  // activate the noise space!
  pn_init();
  while(1)
  {
    perlinDrawConsole();
  }
  return 0;
}

