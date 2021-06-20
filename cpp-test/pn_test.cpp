/*
 ____           _ _         _   _       _            __  __             
|  _ \ ___ _ __| (_)_ __   | \ | | ___ (_)___  ___  |  \/  | __ _ _ __  
| |_) / _ \ '__| | | '_ \  |  \| |/ _ \| / __|/ _ \ | |\/| |/ _` | '_ \ 
|  __/  __/ |  | | | | | | | |\  | (_) | \__ \  __/ | |  | | (_| | |_) |
|_|   \___|_|  |_|_|_| |_| |_| \_|\___/|_|___/\___| |_|  |_|\__,_| .__/ 
                                                                 |_|  
*/

#include <iostream>
#include <SFML/Graphics.hpp>
// for perlin noise generation
#include "pn_header/PerlinNoise.h"

int main(int argc, char const *argv[])
{
  // init window
  sf::RenderWindow window(sf::VideoMode(640, 360), "Perlin Noise Test");
  // limit the framerate; gives a decent performance of 37 fps
  window.setFramerateLimit(60);

  // texture and sprite for pixels
  int tWidth, tHeight;
  tWidth = window.getSize().x;
  tHeight = window.getSize().y;

  // framerate calc
  sf::Font fFont;
  sf::Text fText;
  float fRate = 0.f;

  // termfont to be kept in the bin folder
  if(!fFont.loadFromFile("termfont.ttf"))
  {
    // unable to load the font
    return -1;
  }

  // easy
  fText.setFont(fFont);
  fText.setCharacterSize(24);
  fText.setFillColor(sf::Color::Green);

  // travel through the noise space in time
  float noiseTime = 0;
  float timeInc = 0.1;
  float xoff = 0, yoff = 0;

  sf::Texture noiseTexture;
  if(!noiseTexture.create(tWidth, tHeight))
  {
    // error creating the texture
    return -1;
  }

  sf::Sprite noiseSprite(noiseTexture);

  // activate the noise space
  pn_init();
  int framesDone = 0;

  sf::Clock cloc;
  // the draw loop
  while (window.isOpen())
  {
    sf::Time elapsed = cloc.restart();
    fRate = 1.f/elapsed.asSeconds();

    sf::Event event;
    // poll the various events from the last iteration
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
      {
        // "close requested" event
        window.close();
      }
    }
    // avoid overkilling and every 3 frames, calc new noise values
    // for good performance
    if(framesDone % 3 == 0)
    {
      // update the pixel values each cycle
      // each pixel has rgba i.e. 4 channels
      sf::Uint8 *pixels = new sf::Uint8[tWidth*tHeight*4];

      // travel through the noise space in small increments
      // in the x and the y directions
      xoff = 0;
      for(int x=0; x<tWidth; x++)
      {
        xoff+=0.01f;
        // for each xoff start the yoff at zero.
        yoff = 0;
        for(int y=0; y<tHeight; y++)
        {
          int idx = (x + y*tWidth) * 4;
          // use octaves to generate noise valued color
          int col = (int)(pn_noise(xoff, yoff, noiseTime) * 255);
          pixels[idx] = col;    // red
          pixels[idx+1] = col;  // green
          pixels[idx+2] = col;  // blue
          pixels[idx+3] = 255;  // alpha

          yoff+=0.01f;
        }
      }
      // travel through noise space (3rd dimension) in time
      noiseTime+=timeInc;
      // update the texture and free the pixels
      noiseTexture.update(pixels);
      delete[] pixels;
    }
    fText.setString(std::to_string(fRate));

    // draw to window
    window.draw(noiseSprite);
    window.draw(fText);
    window.display();
    framesDone++;
  }
  return 0;
}
