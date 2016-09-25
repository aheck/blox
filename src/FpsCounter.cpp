#include "FpsCounter.h"

FpsCounter::FpsCounter()
{

}

FpsCounter::~FpsCounter()
{

}

void FpsCounter::calcFps(void)
{
  this->end_time = SDL_GetTicks();
  if (this->start_time == this->end_time) this->end_time++;

  this->seconds = (this->end_time - this->start_time) / 1000;

  if (this->seconds > 0)
    this->fps = this->frames_drawn / this->seconds;
  else
    this->fps = 0;
}

void FpsCounter::start()
{
  this->start_time = SDL_GetTicks();
  this->frames_drawn = 0;
  this->seconds = 0;
  this->fps = 0.0;
}

void FpsCounter::printResults()
{
  this->calcFps();

  /* Display the average framerate. */
  fprintf(stderr, "Drew %i frames in %i seconds, for a framerate of %0.1f fps.\n",
            this->frames_drawn,
            seconds,
            this->fps);
}

int FpsCounter::getSeconds()
{
  this->calcFps();

  return this->seconds;
}

float FpsCounter::getFps()
{
  this->calcFps();

  return this->fps;
}
