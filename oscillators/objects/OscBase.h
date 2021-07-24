#pragma once

#include "../lib/bli.h"

#define CIRCULAR_BUFFER_LEN  (BLI_LEN * 2)

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
};
