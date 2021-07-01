/*
  Visualize the Perin Noise algorithm in the console using ascii
  (Windows and Linux only)
*/
// identify the platform
#if defined(__linux__)
  #include <sys/ioctl.h>  // for TIOCGWINSZ
  #include <unistd.h>     // for STDOUT_FILENO

#elif defined(__WIN32)
  #include <Windows.h>  // for CONSOLE_SCREEN_BUFFER_INFO
#endif

// standard include
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <csignal>  // signal handling
#include "pn_header/PerlinNoise.h"

// world width and height (to be initialised in SetConsoleSize)
int w_width = 0;
int w_height = 0;

// Get the terminal size depending on platform
void SetConsoleSize(void)
{
  #if defined(__WIN32)
    // used to get the screen info
    // implementation may vary. Feel free to edit, if this doesnot work
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    // set the world width and height
    w_width = (int)csbi.dwSize.X - 2;
    w_height = (int)csbi.dwSize.Y - 2;

  #elif defined(__linux__)
    // use the ioctl object
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    // set the world width and height (-2 for safety reasons)
    w_width = window.ws_col - 2;
    w_height = window.ws_row - 2;
  #endif
}

// random float between 0 and 1
float RandomFloat0and1(void)
{
  float lim = 1.0;
  float randval = ( ((float)rand() / (float)RAND_MAX) * lim );
  return randval;
} 


// we now have the width and height; can visualize noise
float zoff = 0.f;
void DrawPerlinConsole(void)
{
  // char to draw
  char charval = '.';
  // travel through the noise space in time
  float xoff, yoff = 0.f;
  float time_step = .001f;
  float incr = 0.1f;
  // for every point, calc the noise val
  for(int y=0; y<w_height; y++)
  {
    // increment yoff, and start xoff at 0 for every yoff
    yoff += incr;
    xoff = 0.f;
    for(int x=0; x<w_width; x++)
    {
      // calc noise value for xy, at time = zoff
      float noise_value = pn_noise(xoff, yoff, zoff);
      
      // // uncommment to generate random noise
      // float noise_value = RandomFloat0and1();

      // based on the noise value, print the char
      charval = noise_value < 0.5 ? '.' : '*';
      printf("%c", charval);
      xoff += incr;   // move along the noise space
    }
    printf("\n");
  }
  // increment zoff by the time step
  zoff += time_step;
  // move back up by w_height to render again
  printf("\033[%dA", w_height);
}

// when Ctrl-C is pressed
void ctrlCSignalHandler(int signum)
{
  // go down w_height lines
  printf("\033[%dB", w_height);
  printf("\nAight! Imma head out...\n\n");
  
  // close it
  exit(signum);
}
int main(int argc, char const *argv[])
{
  // register the signal handler
  signal(SIGINT, ctrlCSignalHandler);
  
  // init and activate the noise space
  SetConsoleSize();
  pn_init();

  // // uncomment to generate random noise
  // srand((unsigned int)time(NULL));

  // loop and render
  while(1){
    DrawPerlinConsole();
    
    // explicit sleep function to limit cpu usage
    #if defined(__WIN32)
      sleep(0.001);
    #elif defined(__linux__)
      usleep(1000);
    #endif
  }
  return 0;
}
