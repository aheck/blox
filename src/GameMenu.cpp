#include "GameMenu.h"

GameMenu::GameMenu(SDL_Surface *screen, SDL_Surface *background) : SdlDraw(screen)
{
  int i;

  this->background = background;

  this->posx = 0;
  this->posy = 0;
  this->curItem = 0;
  this->itemCount = 0;
  this->errorMsg = NULL;
  this->soundChannel = 0;

  for (i = 0; i < MAXITEMS; i++)
  {
    this->menuItems[i] = NULL;
  }
}

GameMenu::~GameMenu()
{
  if (this->errorMsg != NULL)
    delete(this->errorMsg);
}

bool GameMenu::addItem(void (*action)(void), SDL_Surface *imageNormal, SDL_Surface *imageActive)
{
  /* there are no more items allowed */
  if (this->itemCount == MAXITEMS)
    return false;

  this->menuItems[this->itemCount] = new MenuItem(action, imageNormal, imageActive);
  this->itemCount++;


  return true;
}

void GameMenu::drawBackground(void)
{
  SDL_Rect src, dest;

  src.x = 0;
  src.y = 0;
  src.w = this->background->w;
  src.h = this->background->h;

  dest.x = 0;
  dest.y = 0;
  dest.w = this->background->w;
  dest.h = this->background->h;

  SDL_BlitSurface(this->background, &src, this->screen, &dest);
}

void GameMenu::drawMenu(void)
{
  SDL_Rect src, dest;
  SDL_Surface *curImage;
  int i;
  int y = 0;

  y = this->screen->h/6;

  for (i = 0; i < MAXITEMS; i++)
  {
    if (this->menuItems[i] != NULL)
    {
      if (i != this->curItem)
        curImage = this->menuItems[i]->imageNormal;
      else
        curImage = this->menuItems[i]->imageActive;

      src.x = 0;
      src.y = 0;
      src.w = curImage->w;
      src.h = curImage->h;

      dest.x = (this->screen->w - curImage->w) / 2;
      dest.y = y;
      dest.w = curImage->w;
      dest.h = curImage->h;

      SDL_BlitSurface(curImage, &src, this->screen, &dest);
      y += curImage->h + 10;
    }
  }

  /* Print version number */
  SetCurrentFont(getResFont(FONT_FONT1));
  PrintString(this->screen, (SCREEN_W - TextWidth(VERSION)) - 10, 570, "%s", VERSION);
}

void GameMenu::handleInput(void)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                exit(0);

                if (event.key.keysym.sym == SDLK_UP)
                    this->goUp();

                if (event.key.keysym.sym == SDLK_DOWN)
                    this->goDown();

                if (event.key.keysym.sym == SDLK_RETURN)
                    this->execItem();

                break;

            case SDL_JOYBUTTONDOWN:
                this->execItem();
                break;
            case SDL_JOYAXISMOTION:
                if (( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ))
                {
                    if (event.jaxis.axis == 1)
                    {
                        /* up */
                        if (event.jaxis.value < 0)
                            this->goUp();
                        /* down */
                        else if (event.jaxis.value > 0)
                            this->goDown();
                    }
                }

                break;
        }
    }
}

void GameMenu::goUp(void)
{
  if (this->curItem > 0)
    this->curItem--;

  this->soundChannel = util::playSound(SFX_CLICK, this->soundChannel);
}

void GameMenu::goDown(void)
{
  if (this->curItem < (this->itemCount - 1))
    this->curItem++;

  this->soundChannel = util::playSound(SFX_CLICK, this->soundChannel);
}

void GameMenu::execItem(void)
{
  if (this->menuItems[this->curItem]->action != NULL)
    this->menuItems[this->curItem]->action();
  else
    fprintf(stderr, "No callback function defined for menu item number %d.\n", this->curItem+1);
}

/* public methods */

void GameMenu::show(void)
{
  while (1)
  {
    this->handleInput();
  
    this->drawBackground();
    this->drawMenu();
  
    SDL_Flip(this->screen);
  }
}
