#include "SdlDraw.h"

// SdlDraw methods

SdlDraw::SdlDraw(SDL_Surface *screen)
{
  this->screen = screen;
}

SdlDraw::~SdlDraw()
{

}

void SdlDraw::draw(void)
{

}

// SdlDrawExt methods

SdlDrawExt::SdlDrawExt(SDL_Surface *screen) : SdlDraw(screen)
{
  // create new surface for alpha blending
  if ((this->blendingPic = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
                                                this->screen->w, this->screen->h,
                                                this->screen->format->BitsPerPixel,
                                                this->screen->format->Rmask,
                                                this->screen->format->Gmask,
                                                this->screen->format->Bmask,
                                                this->screen->format->Amask)) == NULL)
  {
    // handle error
    fprintf(stderr, "Failed to create SdlDraw::blendingPic SDL surface: %s\n", SDL_GetError());
  }
  else
  {
    // init everything needed for alpha blending with default values
    this->setBlendingColor(0, 0, 0);
    this->setBlendingAlpha(128);
  }
}

SdlDrawExt::~SdlDrawExt()
{
  if (this->blendingPic != NULL)
    SDL_FreeSurface(this->blendingPic);
}

void SdlDrawExt::setBlendingColor(Uint8 r, Uint8 g, Uint8 b)
{
  // fill it with black
  SDL_FillRect(this->blendingPic, NULL, SDL_MapRGB(this->screen->format, r, g, b));
}

void SdlDrawExt::setBlendingAlpha(Uint8 alpha)
{
  // set alpha transparency
  SDL_SetAlpha(this->blendingPic, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
}

void SdlDrawExt::alphaBlending(void)
{
  SDL_BlitSurface(this->blendingPic, NULL, this->screen, NULL);
}
