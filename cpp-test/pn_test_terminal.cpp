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
    GetConsoleScreenBufferInfo(GetStdHandle(STDOUT_HANDLE), &csbi);
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

int main(int argc, char const *argv[])
{
  // init
  SetConsoleSize();
  printf("The console size is: (%d, %d)\n", w_width, w_height);
  return 0;
}
