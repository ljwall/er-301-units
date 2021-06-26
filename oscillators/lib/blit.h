#pragma once

#define BLIT_MAX_SIM 32

namespace ljw
{
  struct impulse
  {
    int position;
    float value;
  };

  // Band-limited impulse train
  class Blit
  {
    public:
      Blit(float *bli_, int bli_length_, int bli_oversample_);
      void addImpulse(float time, float value);
      float next();

      int pulses_count = 0;
      struct impulse pulses[BLIT_MAX_SIM];

    private:
      float *bli;
      int bli_length;
      int bli_oversample;
  };
}
