#include "main.h"

int main(int argc, char **argv)
{
  Uint32 bpp, flags;
  int oldstderrfd;
  FILE *oldstderr;

  gConf = new BloxConf();
  gConf->readConfig();

  // redirect stderr if a logfile name is given
  if (gConf->getLogfile().length() > 0)
  {
    // save stderr
    oldstderrfd = dup(fileno(stderr));
    oldstderr = fdopen(oldstderrfd,"w+");
    if (freopen(gConf->getLogfile().c_str(), "w+", stderr) == NULL)
    {
      // restore stderr
      *stderr = *oldstderr;
      fprintf(stderr, "Can't open logfile for writing: %s\n", gConf->getLogfile().c_str());
    }
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
  {
    fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  atexit(cleanUp);

  flags = SDL_HWSURFACE|SDL_DOUBLEBUF;
  if (gConf->getFullscreen())
  {
    flags |= SDL_FULLSCREEN;
  }

  bpp=SDL_VideoModeOK(SCREEN_W, SCREEN_H, 32, flags);

  if (!bpp)
  {
    bpp = 16;
  }

  gScreen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, bpp, flags);
  if (gScreen == NULL)
  {
    fprintf(stderr, "Unable to set video mode: %s", SDL_GetError());
    return 1;
  }

    /* init joystick subsystem */
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0)
    {
        printf("Joystick subsystem was intialized successfully\n");

        /* get number of available joysticks */
        if (SDL_NumJoysticks() != 1)
        {
            printf("%i joysticks were found.\n", SDL_NumJoysticks());
        }
        else
        {
            printf("%i joystick was found.\n", SDL_NumJoysticks());
        }

        /* open the first joystick if there are any joysticks connected */
        if (SDL_NumJoysticks() > 0)
        {
            printf("Using first Joystick: %s\n", SDL_JoystickName(0));
            SDL_JoystickEventState(SDL_ENABLE);
            gJoystick = SDL_JoystickOpen(0);
            printf("Joystick has:\n");
            printf("\t\t%d buttons\n", SDL_JoystickNumButtons(gJoystick));
            printf("\t\t%d axes\n", SDL_JoystickNumAxes(gJoystick));
            printf("\t\t%d balls\n", SDL_JoystickNumBalls(gJoystick));
            printf("\t\t%d hats\n", SDL_JoystickNumHats(gJoystick));
        }
    }
    else
    {
        fprintf(stderr, "Failed to initialized joystick subsystem: %s\n", SDL_GetError());
    }

  /* Hide the mouse pointer. */
  SDL_ShowCursor(0);

#ifndef NOSOUND
  initAudio();
#endif

  initGraphics();
  initMisc(); 

  gMenu->show();

  SDL_ShowCursor(1);
  delete gConf;

  return 0;
}

void drawBackground(void)
{
  int x, y;

  for (x = 0; x < SCREEN_W; x += gBgImage->w)
  {
    for (y = 0; y < SCREEN_H; y += gBgImage->h)
    {
      src.x = 0;
      src.y = 0;
      src.w = gIngameBg->w;
      src.h = gIngameBg->h;

      dest.x = x;
      dest.y = y;
      dest.w = x + gIngameBg->w;
      dest.h = y + gIngameBg->h;

      SDL_BlitSurface(gIngameBg, &src, gScreen, &dest);
    }
  }

}

void drawScreen(void)
{
  drawBackground();
  gGame->draw();
}

/* initialization functions */

#ifndef NOSOUND
void initAudio(void)
{
  fprintf(stderr, "Init audio system\n");

  /* set up the audio device */
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512))
  {
    fprintf(stderr, "Can't open audio device\n");
    // deactivate audio
    gConf->setMusicPlay(false);
    gConf->setSfxPlay(false);
    return;
  }
  
  if (gConf->getMusicPlay())
    resLoadMusic();

  if (gConf->getSfxPlay())
  {
    resLoadSfx();

    Mix_Volume(-1, gConf->getSfxVolume() * (MIX_MAX_VOLUME / 10));
  }

  if (gConf->getMusicPlay())
  {
    /* play music */
    Mix_FadeInMusic(getResMusic(MUSIC_TRACK02), -1, 4500);
    Mix_VolumeMusic(gConf->getMusicVolume() * (MIX_MAX_VOLUME / 10));
  }
}
#endif

void initGraphics(void)
{
  resLoadPics();

  gIngameBg = getResPic(GPIC_INGAME);
  gBgImage = getResPic(MPIC_BACKGROUND);
}

void initMisc(void)
{
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  /* init the game menu */
  gMenu = new GameMenu(gScreen, gBgImage);

  gMenu->addItem(&playGame, getResPic(MPIC_NEWGAME), getResPic(MPIC_ANEWGAME));
  gMenu->addItem(&showHighscores, getResPic(MPIC_HIGHSCORE), getResPic(MPIC_AHIGHSCORE));
  gMenu->addItem(&showCredits, getResPic(MPIC_CREDITS), getResPic(MPIC_ACREDITS));
  gMenu->addItem(&quitGame, getResPic(MPIC_QUIT), getResPic(MPIC_AQUIT));

  /* init the shapes object */
  gShape = new Shape(gScreen);
  gShapeNext = new Shape(gScreen);
  gShapeNext->setPos(620, 90);

  resLoadFonts();

  /* create game object */
  gGame = new Game(gScreen, gShape, gShapeNext);

  gHighscore = new HighScore(gScreen);
}

/* initialization functions */

#ifndef NOSOUND
void freeAudio(void)
{
  if (!(gConf->getMusicPlay() && gConf->getSfxPlay()))
    return;

  if (gConf->getMusicPlay())
  {
    Mix_HaltMusic();

    /* free all music tracks */
    resFreeMusic();
  }

  if (gConf->getSfxPlay())
  {
    /* free all sfx */
    resFreeSfx();
  }

  Mix_CloseAudio();
}
#endif

void freeGraphics(void)
{
  resFreePics();
}

void freeMisc(void)
{
  delete gMenu;
  delete gGame;
  delete gShape;
  delete gShapeNext;
  delete gHighscore;

  resFreeFonts();
}

void cleanUp(void)
{
#ifndef NOSOUND
  freeAudio();
#endif

  freeGraphics();
  freeMisc();
  SDL_Quit();
}

void handleInput(void)
{
    SDL_Event event;
    Uint32 ticks;
    static int axislr = 0;
    static int axisud = 0;
    static Uint32 lastjoyrepeat = 0;
    static Uint32 joyrepeatdelay = 0;

    ticks = SDL_GetTicks();

    if (gGame->isOver() || gGame->isSettled())
    {
        axislr = axisud = 0;
    }

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (gGameState == GAME_CREDITS)
                    {
                        gGameState = GAME_MENU;
                    }
                    else if (gGameState == GAME_HIGHSCORES)
                    {
                        gGameState = GAME_MENU;
                    }
                    else
                    {
                        gGameState = GAME_EXIT;
                    }
                }

                /* keypress takes you back to the menu if the game is over */
                if (gGameState == GAME_PLAY && gGame->isOver())
                {
                    gGameState = GAME_MENU;
                }

                else if (event.key.keysym.sym == SDLK_LEFT)
                    gGame->moveShapeLeft();
                else if (event.key.keysym.sym == SDLK_RIGHT)
                    gGame->moveShapeRight();
                else if (event.key.keysym.sym == SDLK_DOWN)
                    gGame->moveShapeDown();
                else if (event.key.keysym.sym == SDLK_LCTRL)
                    gGame->rotateShapeLeft();
                else if (event.key.keysym.sym == SDLK_LALT)
                    gGame->rotateShapeRight();
                else if (event.key.keysym.sym == SDLK_p)
                    gGame->togglePause();
                break;

            case SDL_JOYAXISMOTION:
                /* left-right movement */
                if (event.jaxis.axis == 0)
                {
                    axislr = event.jaxis.value;
                    /* left */
                    if (axislr < 0)
                    {
                        gGame->moveShapeLeft();
                    }
                    /* right */
                    else if (axislr > 0)
                    {
                        gGame->moveShapeRight();
                    }

                    joyrepeatdelay = SDL_DEFAULT_REPEAT_DELAY;
                    ticks = lastjoyrepeat = SDL_GetTicks();
                }
                else if (event.jaxis.axis == 1)
                {

                    axisud = event.jaxis.value;
                    /* down */
                    if (event.jaxis.value > 0)
                    {
                        gGame->moveShapeDown();
                        ticks = lastjoyrepeat = SDL_GetTicks();
                        axisud = -1;
                    }
                    else
                    {
                        axisud = 0;
                    }
                    joyrepeatdelay = SDL_DEFAULT_REPEAT_DELAY;
                    ticks = lastjoyrepeat = SDL_GetTicks();
                }

                break;
            case SDL_JOYBUTTONDOWN:
                if (gGameState == GAME_CREDITS)
                {
                    gGameState = GAME_MENU;
                }
                else if (gGameState == GAME_HIGHSCORES)
                {
                    gGameState = GAME_MENU;
                }
                else if (gGameState == GAME_PLAY && gGame->isOver())
                {
                    gGameState = GAME_MENU;
                }
                
                if (event.jbutton.button == 0)
                {
                    gGame->rotateShapeRight();
                }
                else if (event.jbutton.button == 1)
                {
                    gGame->rotateShapeLeft();
                }
                else if (event.jbutton.button == 2)
                {
                    gGame->togglePause();
                }

                break;
        }
    }

    /* Joystick repeat */
    if ((ticks - lastjoyrepeat) >= (joyrepeatdelay + SDL_DEFAULT_REPEAT_INTERVAL))
    {
        if (axislr != 0)
        {
            if (axislr < 0)
                gGame->moveShapeLeft();
            else
                gGame->moveShapeRight();

            joyrepeatdelay = 0;
        }
        if (axisud != 0)
        {
            gGame->moveShapeDown();
            joyrepeatdelay = 0;
        }
    }
}

/* action functions called by the menu class (with function pointer of course :) */
void playGame(void)
{
  int fps = 0;
  bool showfps = gConf->getShowFps();

#ifndef NOSOUND
  if (gConf->getMusicPlay())
    Mix_FadeInMusic(getResMusic(MUSIC_TRACK01), -1, 3500);
#endif

  gGameState = GAME_PLAY;
  gGame->startNewGame();
  gShapeNext->setRandomShape();
  FpsCounter fpscounter;

  /* start fps count */
  fpscounter.start();

  while(gGameState == GAME_PLAY)
  {
    drawScreen();

    /* show fps while playing? */
    if (showfps)
    {
      if (fpscounter.getFrames() % 10 == 0)
      {
        fps = (int)fpscounter.getFps();
      }

      if (fps != 0)
      {
        PrintStringFont(gScreen, getResFont(FONT_FONT1), 760, 560, "%d", fps);
      }
    }

    SDL_Flip(gScreen);
    /* count frame */
    handleInput();
    fpscounter.count();
  }

  /* show fps */
  fpscounter.printResults();
#ifndef NOSOUND
  if (gConf->getMusicPlay())
    Mix_FadeInMusic(getResMusic(MUSIC_TRACK02), -1, 6000);
#endif
}

void quitGame(void)
{
  exit(0);
}

void initCreditPos(int *positions, const char **credits, int strings, int fontHeight)
{
  int i;

  /* init the positions array */
  for (i = 0; i < strings; i++)
  {
    if (i == 0)
      positions[i] = 600 + fontHeight * i;
    else
      positions[i] = positions[i-1] + fontHeight;

    if ((i % 2 == 0) && (i > 1))
    {
      positions[i] += 70;
    }
  }
}

void showCredits(void)
{
  int i;
  int creditStrings;
  int fontHeight;
  BFont_Info *headerFont =  SetFontColor(getResFont(FONT_FONT1), 255, 10, 0);
  const char *credits[] = {"Programming & Graphics",
                           "Andreas Heck",
                           "Testing",
                           "Michael Flaig",
                           "Additional Testing",
                           "Tanja Heck",
                           "INI Parsing Library",
                           "Nicolas Devillard"};

  creditStrings = sizeof(credits) / sizeof (char*);
  int *positions = new int[creditStrings];

  fontHeight = FontHeight(getResFont(FONT_FONT1));

  initCreditPos(positions, credits, creditStrings, fontHeight);

  gGameState = GAME_CREDITS;
  gIngameBg = getResPic(MPIC_BACKGROUND);

  while (gGameState == GAME_CREDITS)
  {
    drawBackground();

    if ((positions[creditStrings-1] + fontHeight) < 0)
    {
      initCreditPos(positions, credits, creditStrings, fontHeight);
    }

    /* draw text and move positions */
    for (i = 0; i < creditStrings; i++)
    {
      if (i % 2)
        SetCurrentFont(getResFont(FONT_FONT2));
      else
        SetCurrentFont(headerFont);
    
      if (positions[i] <= 600 && positions[i] > (0 - fontHeight))
        CenteredPutString(gScreen,  positions[i], credits[i]);
    
      positions[i] -= 1;
    }
    
    SDL_Flip(gScreen);
    handleInput();
  }

  FreeFont(headerFont);
  delete [] positions;
  gIngameBg = getResPic(GPIC_INGAME);
}

void showHighscores(void)
{
  gGameState = GAME_HIGHSCORES;
  gIngameBg = getResPic(MPIC_BACKGROUND);
  gHighscore->readHighScores();

  while(gGameState == GAME_HIGHSCORES)
  {
    drawBackground();
    gHighscore->draw();

    SDL_Flip(gScreen);
    handleInput();
  }

  gIngameBg = getResPic(GPIC_INGAME);
}
