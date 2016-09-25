#include "HighScore.h"

HighScore::HighScore(SDL_Surface *screen) : SdlDraw(screen)
{
  // initialize highscores
  for (int i = 0; i < HIGHSCORE_COUNT; i++)
  {
    this->hsEntries[i].highscore = 0;
    this->hsEntries[i].player = "";
  }

  this->readHighScores();
}

HighScore::~HighScore()
{
  if (this->ini != NULL)
    iniparser_freedict(this->ini);
}

bool HighScore::createHighScoreFile()
{
  FILE *fp;
  int numlines;
  int i;
  char *filecontent[] = {"[Highscores]"};

  this->highscoreFile += getenv("HOME");
  this->highscoreFile += "/";
  this->highscoreFile += ".blox/";
  this->highscoreFile += HIGHSCORE_FILE;

  if ((fp = fopen(this->highscoreFile.c_str(), "w")) == NULL)
  {
    fprintf(stderr, "Can't open file for writing: %s\n", this->highscoreFile.c_str());
    return false;
  }

  numlines = sizeof(filecontent) / sizeof (char*);

  for (i = 0; i < numlines; i++)
  {
    fwrite(filecontent[i], sizeof(char), strlen(filecontent[i]), fp);
  }

  fclose(fp);

  // encrypt new file
  util::encryptFile(this->highscoreFile.c_str(), ENC_KEY);

  return true;
}

// read all Highscores from the ini file and save them to
// the hsEntries array
void HighScore::readHighScores(void)
{
  int i;
  char *buffer;
  char inikey[80];     // ini key
  FILE *fp;

  if ((fp = fopen(this->highscoreFile.c_str(), "r")) == NULL)
  {
    this->createHighScoreFile();
  }
  else
  {
    fclose(fp);
  }

  util::decryptFile(this->highscoreFile.c_str(), ENC_KEY);

  char *filename = strdup(this->highscoreFile.c_str());

  if ((this->ini = iniparser_load(filename)) == NULL)
  {
    fprintf(stderr, "Can't parse highscore file: %s\n", this->highscoreFile.c_str());
    free(filename);
    return;
  }

  free(filename);

  for (i = 0; i < HIGHSCORE_COUNT; i++)
  {
    snprintf(inikey, 80, "Highscores:Highscore%d", i+1);
    this->hsEntries[i].highscore = iniparser_getint(this->ini, inikey, 0);

    snprintf(inikey, 80, "Highscores:Player%d", i+1);
    buffer = iniparser_getstring(this->ini, inikey, " ");
    this->hsEntries[i].player = buffer;
  }

  util::encryptFile(this->highscoreFile.c_str(), ENC_KEY);
}

int HighScore::isHighScore(int score)
{
  int i;

  for (i = 0; i < HIGHSCORE_COUNT; i++)
  {
    if (score >= this->hsEntries[i].highscore)
    {
      // sweet! this is a new highscore :)
      return i;
    }
  }

  // sorry dude! this is no new highscore :(
  return -1;
}

void HighScore::insertHighScore(int score, char *name)
{
  HsEntry buffer, buffer2;
  int pos, i;
  char inikey[80];
  char hsString[20];
  FILE *fp;

  // get position of the new highscore
  pos = this->isHighScore(score);

  buffer.highscore = this->hsEntries[pos].highscore;
  buffer.player = this->hsEntries[pos].player;

  // move other highscores down
  while (pos < (HIGHSCORE_COUNT - 1))
  {
    buffer2.highscore = this->hsEntries[pos+1].highscore;
    buffer2.player = this->hsEntries[pos+1].player;

    this->hsEntries[pos+1].highscore = buffer.highscore;
    this->hsEntries[pos+1].player = buffer.player;

    buffer.highscore = buffer2.highscore;
    buffer.player = buffer2.player;

    pos++;
  }

  // set the new highscore
  pos = this->isHighScore(score);

  this->hsEntries[pos].highscore = score;
  this->hsEntries[pos].player = name;

  // update the iniparser structure
  for (i = 0; i < HIGHSCORE_COUNT; i++)
  {
    snprintf(inikey, 80, "Highscores:Highscore%d", i+1);
    snprintf(hsString, 20, "%d", this->hsEntries[i].highscore);
    iniparser_setstr(this->ini, inikey, hsString);

    snprintf(inikey, 80, "Highscores:Player%d", i+1);
    iniparser_setstr(this->ini, inikey, (char*)this->hsEntries[i].player.c_str());
  }

  // open the highscore file for writing
  if ((fp = fopen(this->highscoreFile.c_str(), "w")) == NULL)
  {
    fprintf(stderr, "Can't open %s for writing\n", this->highscoreFile.c_str());
    return;
  }

  // dump all highscores to the highscore file
  iniparser_dump_ini(this->ini, fp);

  fclose(fp);

  util::encryptFile(this->highscoreFile.c_str(), ENC_KEY);

  this->readHighScores();
}

void HighScore::addHighScore(int score)
{
  char buffer[20] = "";
  char input;
  char cursor = '\0';
  int curpos = 0;
  int maxchars = 20;
  int i = 0;
  bool finished = false;
  SDLKey key;
  SDL_Event event;

  // do we really have a highscore?
  if (this->isHighScore(score) == -1)
     return;

  BFont_Info *headerFont = SetFontColor(getResFont(FONT_FONT1), 255, 0, 0);

  SDL_EnableUNICODE(1);

  while (!finished)
  {
    // draw screen
    SDL_FillRect(this->screen, NULL, SDL_MapRGB(this->screen->format, 0, 0, 0));

    // animate cursor
    if (i < 30)
      cursor = '|';
    else
      cursor = '\0';

    if (i == 60)
      i = 0;

    CenteredPrintStringFont(this->screen, getResFont(FONT_FONT1), 200, "You've got %d points", score);
    CenteredPutStringFont(this->screen, headerFont, 250, "Great score!");
    CenteredPutStringFont(this->screen, headerFont, 300, "Please enter your name:");
    CenteredPrintStringFont(this->screen, getResFont(FONT_FONT1), 360, "%s", buffer);
    PrintStringFont(this->screen, getResFont(FONT_FONT1), ((SCREEN_W - TextWidth(buffer)) / 2 + TextWidth(buffer) + 5), 360, "%c", cursor);

    SDL_Flip(this->screen);

    // get keyboard input
    while (SDL_PollEvent(&event))
    {
      key = event.key.keysym.sym;

      if (event.type == SDL_KEYDOWN)
      {
        key = event.key.keysym.sym;
    
        switch(key)
        {
          case SDLK_RETURN:
              // accept input only if there are more than zero characters
              if (curpos > 0)
                 finished = true;
              break;
          case SDLK_BACKSPACE:
            if (curpos >= 0)
            {
              if (curpos > 0)
                curpos--;
              buffer[curpos] = '\0';
            }
          default:
            if (curpos < (maxchars - 1))
            {
              input = event.key.keysym.unicode;
              if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z')
                   || input == ' ' || (input >= '1' && input <= '9'))
              {
                buffer[curpos] = input;
                buffer[curpos+1] = '\0';
                curpos++;
              }
            }
            break;
        }
      }
    }

    i++;
  }

  this->insertHighScore(score, buffer);

  SDL_EnableUNICODE(0);
  FreeFont(headerFont);
}

void HighScore::draw(void)
{
  int y = SCREEN_H/8;
  int i;
  BFont_Info *font = getResFont(FONT_FONT1);

  SetCurrentFont(font);
  PutStringFont(this->screen, getResFont(FONT_FONT2), (SCREEN_W-TextWidth("Highscores"))/2, 25, "Highscores");

  for (i = 0; i < HIGHSCORE_COUNT; i++)
  {
    if (this->hsEntries[i].highscore)
    {
      PrintString(this->screen, 150, y, "%2d.    %6d   %s", i+1, this->hsEntries[i].highscore, this->hsEntries[i].player.c_str());
    }
    else
    {
      PrintString(this->screen, 150, y, "%2d.    %s", i+1, "------   ----------------");
    }

    y += 20 + FontHeight(font);
  }

  return;
}
