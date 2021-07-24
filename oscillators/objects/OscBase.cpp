#include <hal/simd.h>
#include "OscBase.h"

OscBase::OscBase()
{
  idx_play = 0;
  idx_work = BLI_CROSSINGS;

  simd_set(corrections, CIRCULAR_BUFFER_LEN, 0.0f);
  simd_set(naive_wave, CIRCULAR_BUFFER_LEN, 0.0f);
}

OscBase::~OscBase()
{
}

void OscBase::applyJump(float value, float position)
{
    int sample_pos = (int)(position*((float)BLI_OVERSAMPLE));
    for (int j=0; j<BLI_CROSSINGS*2; j++)
    {
      corrections[(idx_play + j) % CIRCULAR_BUFFER_LEN] += value*ljw::Bli::step_corrections[sample_pos];
      sample_pos += BLI_OVERSAMPLE;
    }
}
