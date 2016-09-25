#include "Shape.h"

Shape::Shape(SDL_Surface *screen) : SdlDraw(screen)
{
  SDL_Surface *buffer;
  int i;
  int shape_rot_counts[SHAPE_COUNT] = {2, 4, 2, 1, 2, 4, 4};
  int shapes[SHAPE_COUNT][4][SHAPE_WH][SHAPE_WH] = {
                        {
                          {{1,1,1,1},
                           {0,0,0,0},
                           {0,0,0,0},
                           {0,0,0,0}},
                        
                          {{0,1,0,0},
                           {0,1,0,0},
                           {0,1,0,0},
                           {0,1,0,0}}

                        },

                        {
                          {{2,2,2,0},
                           {0,2,0,0},
                           {0,0,0,0},
                           {0,0,0,0}},

                          {{0,2,0,0},
                           {2,2,0,0},
                           {0,2,0,0},
                           {0,0,0,0}},


                          {{0,2,0,0},
                           {2,2,2,0},
                           {0,0,0,0},
                           {0,0,0,0}},

                          {{2,0,0,0},
                           {2,2,0,0},
                           {2,0,0,0},
                           {0,0,0,0}}

                        },

                        {
                          {{3,3,0,0},
                           {0,3,3,0},
                           {0,0,0,0},
                           {0,0,0,0}},
                           
                          {{0,0,3,0},
                           {0,3,3,0},
                           {0,3,0,0},
                           {0,0,0,0}}

                        },

                        {
                          {{4,4,0,0},
                           {4,4,0,0},
                           {0,0,0,0},
                           {0,0,0,0}}
                        },

                        {
                          {{0,5,5,0},
                           {5,5,0,0},
                           {0,0,0,0},
                           {0,0,0,0}},

                          {{0,5,0,0},
                           {0,5,5,0},
                           {0,0,5,0},
                           {0,0,0,0}}
                        },

                        {
                          {{6,6,6,0},
                           {0,0,6,0},
                           {0,0,0,0},
                           {0,0,0,0}},

                           {{0,6,0,0},
                            {0,6,0,0},
                            {6,6,0,0},
                            {0,0,0,0}},

                           {{6,0,0,0},
                            {6,6,6,0},
                            {0,0,0,0},
                            {0,0,0,0}},

                           {{6,6,0,0},
                            {6,0,0,0},
                            {6,0,0,0},
                            {0,0,0,0}}
                            
                        },

                        {
                          {{7,7,7,0},
                           {7,0,0,0},
                           {0,0,0,0},
                           {0,0,0,0}},

                           {{7,7,0,0},
                            {0,7,0,0},
                            {0,7,0,0},
                            {0,0,0,0}},

                           {{0,0,7,0},
                            {7,7,7,0},
                            {0,0,0,0},
                            {0,0,0,0}},
                           
                           {{7,0,0,0},
                            {7,0,0,0},
                            {7,7,0,0},
                            {0,0,0,0}}
                        }

                       };

  SDL_Color blockcolors[SHAPE_COUNT] = {
                              {0xff, 0x00, 0x00},
                              {0xd3, 0xd8, 0x2d},
                              {0x4b, 0x37, 0xd3},   // blue
                              {0xd6, 0x9c, 0x31},
                              {0x7e, 0x1c, 0xce},   // green
                              {0x55, 0xe8, 0x20},
                              {0x93, 0x2a, 0x99}
                              };

  srand(time(NULL));

  memcpy(this->shapes, shapes, sizeof(shapes));
  memcpy(this->shapeRotNums, shape_rot_counts, sizeof(shape_rot_counts));

  /* init block surfaces */
  for (i = 0; i < SHAPE_COUNT; i++)
  {
    this->blocks[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, BLOCK_PIX_WH, BLOCK_PIX_WH,
                                       this->screen->format->BitsPerPixel, 0x00, 0x00, 0x00, 0);
    buffer = this->blocks[i];
    this->blocks[i] = SDL_DisplayFormat(this->blocks[i]);
    SDL_FreeSurface(buffer);

    this->createBlock(this->blocks[i], blockcolors[i].r, blockcolors[i].g, blockcolors[i].b);
  }

  /* set position */
  this->setPos(0, 0);

  /* init this->shape */
  this->shape = SDL_CreateRGBSurface(SDL_SRCALPHA, BLOCK_PIX_WH * SHAPE_WH, BLOCK_PIX_WH * SHAPE_WH,
                                     this->screen->format->BitsPerPixel, 0x00, 0x00, 0x00, 255);
  this->colorkey = SDL_MapRGB(this->shape->format, COLOR_KEY_RED, COLOR_KEY_GREEN, COLOR_KEY_BLUE);
  SDL_SetColorKey(this->shape, SDL_SRCCOLORKEY, this->colorkey);
  buffer = this->shape;
  this->shape = SDL_DisplayFormat(this->shape);
  SDL_FreeSurface(buffer);

  this->setRandomShape();
}

Shape::~Shape()
{
  int i;

  if (this->shape != NULL)
    SDL_FreeSurface(this->shape);

  for (i = 0; i < SHAPE_COUNT; i++)
  {
    if (blocks[i] != NULL)
      SDL_FreeSurface(this->blocks[i]);
  }
}

void Shape::createBlock(SDL_Surface *block, Uint8 r, Uint8 g, Uint8 b)
{
  Uint32 bordercolor, color;
  int bordersize;
  SDL_Rect dest;

  bordercolor = SDL_MapRGB(this->screen->format, 0x00, 0x00, 0x00);
  color = SDL_MapRGB(this->screen->format, r, g, b);

  dest.x = 0;
  dest.y = 0;
  dest.w = BLOCK_PIX_WH;
  dest.h = BLOCK_PIX_WH;

  SDL_FillRect(block, &dest, bordercolor);

  bordersize = (int)((float)BLOCK_PIX_WH / 8.0);

  dest.x = bordersize;
  dest.y = bordersize;
  dest.w = BLOCK_PIX_WH - bordersize * 2;
  dest.h = BLOCK_PIX_WH - bordersize * 2;

  SDL_FillRect(block, &dest, color);
}

void Shape::createShape(void)
{
  int i, j;
  SDL_Rect dest;

  /* redraw shape surface */

  dest.x = 0; 
  dest.y = 0;
  dest.w = this->shape->w;
  dest.h = this->shape->h;

  SDL_FillRect(this->shape, &dest, this->colorkey);

  /* draw shape */
  dest.w = BLOCK_PIX_WH;
  dest.h = BLOCK_PIX_WH;

  for (i = 0; i < SHAPE_WH; i++)
  {
    for (j = 0; j < SHAPE_WH; j++)
    {
      if (this->curShape[i][j] == 0)
        continue;

      /* set destination coordinates for the blit */
      dest.x = BLOCK_PIX_WH * j;
      dest.y = BLOCK_PIX_WH * i;

      SDL_BlitSurface(this->blocks[this->curShapeID], NULL, this->shape, &dest);
    }
  }
}

void Shape::setRandomShape(void)
{
  this->setShape((rand() % SHAPE_COUNT), 0);
  this->createShape();
}

int Shape::getShape(void)
{
  return this->curShapeID;
}

bool Shape::setShape(int shape, int rot)
{
  if (shape >= 0 && shape < SHAPE_COUNT)
  {
    this->curShapeRotID = rot;
    this->curShapeID = shape;
    this->setShapeData(shape, rot);
    this->createShape();

    return true;
  }

  return false;
}

void Shape::setShapeData(int shape, int rot)
{
  memcpy(this->curShape, this->shapes[shape][rot], sizeof(this->curShape));
}

ShapeField* Shape::getShapeData(void)
{
  return (&this->curShape);
}

SDL_Surface* Shape::getBlock(unsigned int blockID)
{
  if (blockID < (sizeof(this->blocks) / sizeof(SDL_Surface*)) )
  {
    return this->blocks[blockID];
  }

  return NULL;
}

bool Shape::rotateLeft(void)
{
  int rot;

  if (this->shapeRotNums[this->curShapeID] == 1)
    return false;

  if (this->curShapeRotID > 0)
    rot = this->curShapeRotID - 1;
  else
    rot = this->shapeRotNums[this->curShapeID] - 1;

  this->setShape(this->curShapeID, rot);

  return true;
}

bool Shape::rotateRight(void)
{
  int rot;

  if (this->shapeRotNums[this->curShapeID] == 1)
    return false;

  if (this->curShapeRotID < (this->shapeRotNums[this->curShapeID] - 1))
    rot = this->curShapeRotID + 1;
  else
    rot = 0;
  this->setShape(this->curShapeID, rot);

  return true;
}

void Shape::setPos(int x, int y)
{
  this->posx = x;
  this->posy = y;
}

void Shape::draw(void)
{
  SDL_Rect dest;

  /* draw onto screen */
  dest.x = this->posx;
  dest.y = this->posy;
  dest.w = this->shape->w;
  dest.h = this->shape->h;

  SDL_BlitSurface(this->shape, NULL, this->screen, &dest);
}
