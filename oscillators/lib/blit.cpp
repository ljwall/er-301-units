#include <iostream>
#include "blit.h"


namespace ljw
{
  Blit::Blit(float *bli_, int bli_length_, int bli_oversample_)
  {
    for (int i=0; i<BLIT_MAX_SIM; i++)
      pulses[i] = {0, 0.0f};

    bli = bli_;
    bli_length = bli_length_;
    bli_oversample = bli_oversample_;
  }

  void Blit::addImpulse(float time, float value)
  {
    for (int i=0; i<BLIT_MAX_SIM; i++)
    {
      if (pulses[i].value == 0)
      {
        pulses[i].position = (int)(time * bli_oversample);
        pulses[i].value = value;
        pulses_count++;
        break;
      }
    }
  }

  float Blit::next()
  {
    float res = 0.0f;

    for (int i=0; i<pulses_count; i++)
    {
      if (pulses[i].value == 0)
      {
        break;
      }

      while (true)
      {
        res += pulses[i].value * bli[pulses[i].position];
        pulses[i].position += bli_oversample;

        if (pulses[i].position < bli_length)
        {
          break;
        }
        else
        {
          pulses_count--;

          if (i == pulses_count)
          {
            pulses[i] = {0, 0.0f};
            break;
          }
          else
          {
            pulses[i] = pulses[pulses_count];
            pulses[pulses_count] = {0, 0.0f};
            continue;
          }
        }
      }
    }

    return res;
  }
}
