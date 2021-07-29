#pragma once

#include "../lib/bli.h"

#define CIRCULAR_BUFFER_LEN 512

class OscBase
{
  public:
    OscBase();
    ~OscBase();

  protected:
    float naive_wave[CIRCULAR_BUFFER_LEN];
    float corrections[CIRCULAR_BUFFER_LEN];
    int idx_work, idx_play;

    void applyJump(float value, float position);

  private:
    void scaleAdd(float *__restrict__ target, float multiplier, float *__restrict__ source, unsigned int n);
};
