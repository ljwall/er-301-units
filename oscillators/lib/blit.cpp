#include "blit.h"
#include <hal/log.h>


namespace ljw
{
  Blit::Blit()
  {
    Bli::make();
    for (int i=0; i<BLIT_BUFFER_LEN; i++)
    {
      buffer[i] = 0;
    }
  }


  void Blit::addImpulse(float time, float value)
  {
    int j = buffer_pos;

    for (int i = time * BLI_OVERSAMPLE; i<BLI_LEN; i += BLI_OVERSAMPLE)
    {
      buffer[j] += value * Bli::bli[i];
      j = (j+1) % BLIT_BUFFER_LEN;
    }
  }

  float Blit::next()
  {
    float res = buffer[buffer_pos];
    buffer[buffer_pos] = 0;
    buffer_pos = (buffer_pos+1) % BLIT_BUFFER_LEN;

    return res;
  }
}
