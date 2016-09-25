/*
 *  FpsCounter.h - Configuration Class
 *
 * 
 *  This class implements a frame rate counter
 *
 */

#include <stdio.h>
#include <SDL.h>

#ifndef _FPSCOUNTER_H
#define _FPSCOUNTER_H

class FpsCounter
{
  private:

  /* frame rate counter variables */
  long start_time, end_time;
  int frames_drawn;
  float fps;
  int seconds;

  void calcFps(void);
    
  public:

    FpsCounter();
    ~FpsCounter();
    
    void start();
    void count() {this->frames_drawn++;}
    void printResults();
    int getFrames() {return this->frames_drawn;}
    int  getSeconds();
    float getFps();
};

#endif
