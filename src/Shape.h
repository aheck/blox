/*
 *  Shape.h - Implements Shapes
 *
 * 
 *  This class implements most shape specific stuff:
 *
 *  - hold array of data for all shapes and their rotations
 *  - set current shape
 *  - rotate current shape
 *  - select random shape
 *  - get pointer to the data of the current shape (needed
 *    for collision detection in Game.cpp)
 *
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL.h>

#include "stuff.h"
#include "SdlDraw.h"

#ifndef _SHAPE_H
#define _SHAPE_H

#define SHAPE_COUNT 7

class Shape : public SdlDraw
{

  private:

    int shapes[SHAPE_COUNT][4][SHAPE_WH][SHAPE_WH];
    int shapeRotNums[SHAPE_COUNT];  // number of rotations for every shape
    SDL_Surface* blocks[SHAPE_COUNT];
    SDL_Surface* shape;
    Uint32 colorkey;
    int posx;
    int posy;
    int curShapeID;
    int curShapeRotID;
    int curShape[SHAPE_WH][SHAPE_WH];

    void createBlock(SDL_Surface *block, Uint8 r, Uint8 g, Uint8 b);
    void createShape(void);
    void setShapeData(int shape, int rot);

  public:

    Shape(SDL_Surface *screen);
    ~Shape();

    void setRandomShape(void);
    int  getShape(void);
    bool setShape(int shape, int rot);
    ShapeField* getShapeData(void);
    SDL_Surface* getBlock(unsigned int blockID);
    bool rotateLeft(void);
    bool rotateRight(void);
    void setPos(int x, int y);
    void draw(void);
};

#endif
