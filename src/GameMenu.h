#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifndef _GAMEMENU_H
#define _GAMEMENU_H


#include "SdlDraw.h"
#include "util.h"
#include "resources.h"
#include "stuff.h"

#define MAXITEMS 10


struct MenuItem
{
  void (*action)(void);
  SDL_Surface *imageNormal;
  SDL_Surface *imageActive;

  MenuItem(void (*action)(void), SDL_Surface *imageNormal, SDL_Surface *imageActive)
  {
    this->action = action;
    this->imageNormal = imageNormal;
    this->imageActive = imageActive;
  }
};

class GameMenu : public SdlDraw
{
  private:

    int posx, posy;
    int curItem;
    int itemCount;
    char *errorMsg;
    int soundChannel;
    SDL_Surface *background;
    MenuItem *menuItems[MAXITEMS];

    void drawBackground(void);
    void drawMenu(void);
    void handleInput(void);

    void goUp(void);
    void goDown(void);
    void execItem(void);

  public:

    GameMenu(SDL_Surface *screen, SDL_Surface *background);

    ~GameMenu();

    char *getErrorMsg(void);
    bool addItem(void (*action)(void), SDL_Surface *imageNormal, SDL_Surface *imageActive);

    void show(void);
};

#endif
