/*
 *  SdlDraw.h - Base Classes for every Class that draws on the screen
 *
 *  SdlDraw:
 *  
 *    - protected SDL_Surface pointer to the screen
 *    - screen pointer is initialized with constructor
 *    - virtual draw method
 *
 *  SdlDrawExt:
 *  
 *    - support for alpha blending of the whole screen
 *      relevant methods: setBlendingColor, setBlendingAlpha, alphaBlending
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>


#ifndef _SDLDRAW_H

// base class for drawing onto the screen
class SdlDraw
{
  protected:

    SDL_Surface *screen;

  public:

    SdlDraw(SDL_Surface *screen);
    virtual ~SdlDraw();
    virtual void draw(void);
};

// extended base class for screen drawing (implements alpha blending)
class SdlDrawExt : public SdlDraw
{
  protected:

    SDL_Surface *blendingPic;

  public:

    SdlDrawExt(SDL_Surface *screen);
    ~SdlDrawExt();

    void setBlendingColor(Uint8 r, Uint8 g, Uint8 b);
    void setBlendingAlpha(Uint8 alpha);
    void alphaBlending(void);
};

#endif
#define _SDLDRAW_H
