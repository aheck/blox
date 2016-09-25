#include "Game.h"

Game::Game(SDL_Surface *screen, Shape *shape, Shape *shapeNext) : SdlDrawExt(screen)
{
  this->shape = shape;
  this->shapeNext = shapeNext;
  this->gameOverPic = getResPic(GPIC_GAMEOVER);
  this->pausedPic = getResPic(GPIC_PAUSED);
  this->soundChannel = 0;
  this->gameOver = true;
  this->shapeSettled = true;

  this->highscore = new HighScore(this->screen);

  this->setBlendingColor(0x99, 0x99, 0x99);
}

Game::~Game()
{

}

void Game::draw(void)
{
  int x, y;
  int blocknum;
  SDL_Rect dest;

  dest.w = BLOCK_PIX_WH;
  dest.h = BLOCK_PIX_WH;

  /* draw settled blocks */
  for (y = 0; y < FIELD_H; y++)
  {
    for (x = 0; x < FIELD_W; x++)
    {
      blocknum = this->field[y][x];
      if (blocknum != 0)
      {
        dest.x = FIELD_PIX_X + x * 30;
        dest.y = y * 30;
        SDL_BlitSurface(this->shape->getBlock(blocknum-1), NULL, this->screen, &dest);
      }
    }
  }

  /* print help texts */
  PutStringFont(this->screen, getResFont(FONT_FONT1), 30, 60, "Menu: ESC");
  PutStringFont(this->screen, getResFont(FONT_FONT1), 30, 100, "Pause: P");
  PutStringFont(this->screen, getResFont(FONT_FONT1), 30, 140, "Rotate L: CTRL");
  PutStringFont(this->screen, getResFont(FONT_FONT1), 30, 180, "Rotate R: ALT");
  PutStringFont(this->screen, getResFont(FONT_FONT1), 30, 220, "Move: Cursor Keys");

  /* print lines, score and level */
  PutStringFont(this->screen, getResFont(FONT_FONT1), 580, 60, "Next");
  PrintStringFont(this->screen, getResFont(FONT_FONT1), 580, 300, "Lines:         %d", this->lines);
  PrintStringFont(this->screen, getResFont(FONT_FONT1), 580, 340, "Score:         %d", this->score);
  PrintStringFont(this->screen, getResFont(FONT_FONT1), 580, 410, "Level:         %d", this->level);

  this->shape->draw();
  this->shapeNext->draw();

  if (this->gameOver || this->paused)
  {
    this->alphaBlending();
  }

  if (this->gameOver)
  {
    dest.x = (this->screen->w - this->gameOverPic->w) / 2;
    dest.y = 250;
    dest.w = this->gameOverPic->w;
    dest.h = this->gameOverPic->h;
    
    SDL_BlitSurface(this->gameOverPic, NULL, this->screen, &dest);
  }
  else if (this->paused)
  {
    dest.x = (this->screen->w - this->pausedPic->w) / 2;
    dest.y = 250;
    dest.w = this->pausedPic->w;
    dest.h = this->pausedPic->h;
    
    SDL_BlitSurface(this->pausedPic, NULL, this->screen, &dest);
  }
}

void Game::startNewGame(void)
{
  int x, y;

  this->score = 0;
  this->downscore = 0;
  this->lines = 0;
  this->level = 0;
  this->paused = false;
  this->gameOver = false;
  this->shapeSettled = false;
  this->initShapePos();
  this->timerInterval = DEFAULT_TINTERVAL;

  /* clear field */
  for (y = 0; y < FIELD_H; y++)
  {
    for (x = 0; x < FIELD_W; x++)
    {
      this->field[y][x] = 0;
    }
  }

  this->updateShapePos();
  this->shape->setRandomShape();

  this->setTimer();
}

void Game::initShapePos(void)
{
  this->posx = 4;
  this->posy = 0;
  this->updateShapePos();
}

void Game::setTimer(void)
{
  SDL_RemoveTimer(this->timerID);
  this->timerID = SDL_AddTimer(this->timerInterval, Game::fallCallback, (void*)this);
}

Uint32 Game::fallCallback(Uint32 interval, void *object)
{
  ((Game*)object)->moveShapeDown();

  return interval;
}

void Game::setScore(int score)
{
  this->score = score;
}

int  Game::getScore(void)
{
  return this->score;
}

void Game::setLines(int lines)
{
  this->lines = lines;
}

int  Game::getLines(void)
{
  return this->lines;
}

void Game::setLevel(int level)
{
  this->level = level;
}

int  Game::getLevel(void)
{
  return this->level;
}

void Game::togglePause(void)
{
  if (this->gameOver == false)
  {
    this->paused = !this->paused;

    if (!this->paused)
      this->setTimer();
  }
}

bool Game::moveShapeLeft(void)
{
  if (this->paused || this->gameOver)
    return false;

  if (this->collLeft() || this->gameOver)
    return false;
  
  this->posx--;
  this->updateShapePos();

  return true;
}

bool Game::moveShapeRight(void)
{
  if (this->paused || this->gameOver)
    return false;

  if (this->collRight() || this->gameOver)
    return false;

  this->posx++;
  this->updateShapePos();

  return true;
}

bool Game::moveShapeDown(void)
{
  if (this->paused || this->gameOver)
    return false;

  if (this->collBottom() || this->gameOver)
    return false;

  this->shapeSettled = false;

  this->posy++;
  this->downscore++;
  this->updateShapePos();
  this->setTimer();

  return true;
}

void Game::updateShapePos(void)
{
  this->shape->setPos(FIELD_PIX_X + BLOCK_PIX_WH * this->posx, FIELD_PIX_Y + BLOCK_PIX_WH * this->posy);
}

bool Game::collLeft(void)
{
  int x, y;
  ShapeField *shapeData;

  /* check if shape collides with other block */
  if (collBlock(this->posx - 1, this->posy))
  {
    return true;
  }

  if (this->posx > 0)
    return false;

  x = -this->posx;
  shapeData = this->shape->getShapeData();

  for (y = 0; y < SHAPE_WH; y++)
  {
    if ((*shapeData)[y][x] != 0)
      return true;
  }

  return false;
}

bool Game::collRight(void)
{
  int x, y;
  ShapeField *shapeData;

  /* check if shape collides with other block */
  if (collBlock(this->posx + 1, this->posy))
  {
    return true;
  }

  if (this->posx < (FIELD_W - SHAPE_WH))
    return false;

  x = FIELD_W - this->posx - 1;
  shapeData = this->shape->getShapeData();

  for (y = 0; y < SHAPE_WH; y++)
  {
    if ((*shapeData)[y][x] != 0)
      return true;
  }

  return false;
}

bool Game::shapeOverlapLeft(void)
{
  int x, y;
  ShapeField *shapeData;

  if (this->posx >= 0)
  {
    return false;
  }

  shapeData = this->shape->getShapeData();

  for (x = 0; x < (SHAPE_WH - (SHAPE_WH + posx)); x++)
  {
    for (y = 0; y < SHAPE_WH; y++)
    {
      if ((*shapeData)[y][x] != 0)
        return true;
    }
  }

  return false;
}


bool Game::shapeOverlapRight(void)
{
  int x, y;
  ShapeField *shapeData;

  if ((this->posx + 4) < FIELD_W)
  {
    return false;
  }

  x = FIELD_W - this->posx;
  shapeData = this->shape->getShapeData();

  for (; x < SHAPE_WH; x++)
  {
    for (y = 0; y < SHAPE_WH; y++)
    {
      if ((*shapeData)[y][x] != 0)
        return true;
    }
  }

  return false;
}

bool Game::collBottom(void)
{
  int x, y;
  ShapeField *shapeData;

  /* check if shape collides with other block */
  if (collBlock(this->posx, this->posy + 1))
  {
    this->settleShape();
    return true;
  }

  /* check if shape collides with bottom */
  if (this->posy < (FIELD_H - SHAPE_WH))
    return false;
  
  y = FIELD_H - this->posy -1;
  shapeData = this->shape->getShapeData();

  for (x = 0; x < SHAPE_WH; x++)
  {
    if ((*shapeData)[y][x] != 0)
    {
      this->settleShape();
      return true;
    }
  }

  return false;
}

/* check if shape collides with other block */
bool Game::collBlock(int posx, int posy)
{
  int x, y;
  ShapeField *shapeData = this->shape->getShapeData();
 
  for (y = 0; y < SHAPE_WH; y++)
  {
    for (x = 0; x < SHAPE_WH; x++)
    {
      /* check if x and y are inside the play field */
      if ((posy + y > FIELD_H - 1) || (posx + x > FIELD_W - 1))
        continue;

      if (((*shapeData)[y][x] != 0) && (this->field[posy+y][posx+x] != 0))
        return true;
    }
  }

  return false;
}

void Game::settleShape(void)
{
  int x, y, curfield;
  int lineCount;

  ShapeField *shapeData = this->shape->getShapeData();

  for (y = 0; y < SHAPE_WH; y++)
  {
    for (x = 0; x < SHAPE_WH; x++)
    {
      curfield = (*shapeData)[y][x];
      if (curfield != 0)
      {
        this->field[this->posy + y][this->posx + x] = curfield;
      }
    }
  }

  lineCount = this->detectLines();

  if (lineCount == 0)
  {
    util::playSound(SFX_FALL, -1);
  }
  
  this->addScore(lineCount);
  this->selectNextShape();

  SDL_EnableKeyRepeat(0, 0);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  this->shapeSettled = true;
}

void Game::addScore(int lineCount)
{
  this->score += (this->downscore + this->level) * (lineCount + 1);
  this->level = this->lines / 10;
  this->timerInterval = DEFAULT_TINTERVAL - (this->level * 60);

  this->downscore = 0;
}

void Game::selectNextShape(void)
{
  if (this->gameOver)
    return;

  /* make the next shape the current shape */
  this->shape->setShape(this->shapeNext->getShape(), 0);
  this->initShapePos();
  /* set the next shape */
  this->shapeNext->setRandomShape();

  /* check if game is over */
  if (this->collBlock(this->posx, this->posy))
  {
    this->gameOver = true;
    SDL_RemoveTimer(this->timerID);
    this->highscore->addHighScore(this->score);
  }
  else
  {
    this->setTimer();
  }
}

int Game::detectLines(void)
{
  int x, y;
  int lineCount = 0;
  int hit;

  for (y = 0; y < FIELD_H; y++)
  {
    hit = 1;

    for (x = 0; x < FIELD_W; x++)
    {
      if (this->field[y][x] == 0)
      {
        hit = 0;
        break;
      }
    }

    if (hit)
    {
      this->gravity(y, 1);
      this->lines++;
      lineCount++;
    }
  }

  util::playSound(SFX_DELETE, -1);

  return lineCount;
}

void Game::gravity(int posy, int lineCount)
{
  int x, y;

  for (y = posy; y >= 1; y--)
  {
    for (x = 0; x < FIELD_W; x++)
    {
      this->field[y][x] = this->field[y-lineCount][x];
    }
  }
}

bool Game::rotateShapeLeft(void)
{
  bool done;

  if (this->paused)
    return false;

  done = this->shape->rotateLeft();

  if (this->shapeOverlapRight() || this->shapeOverlapLeft() || collBlock(this->posx, this->posy))
  {
    this->shape->rotateRight();
    return false;
  }

  if (done)
  {
    this->soundChannel = util::playSound(SFX_ROTLEFT, this->soundChannel);
  }

  return true;
}

bool Game::rotateShapeRight(void)
{
  bool done;

  if (this->paused)
    return false;

  done = this->shape->rotateRight();

  if (this->shapeOverlapRight() || this->shapeOverlapLeft() || collBlock(this->posx, this->posy))
  {
    this->shape->rotateLeft();
    return false;
  }

  if (done)
  {
    this->soundChannel = util::playSound(SFX_ROTRIGHT, this->soundChannel);
  }

  return true;
}
