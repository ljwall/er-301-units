#pragma once
#include "bli.h"

#define BLIT_BUFFER_LEN (BLI_CROSSINGS*2)

namespace ljw
{
  // Band-limited impulse train
  class Blit
  {
    public:
      Blit();
      void addImpulse(float time, float value);
      float next();

    private:
      float buffer[BLIT_BUFFER_LEN];
      int  buffer_pos = 0;
  };
}
