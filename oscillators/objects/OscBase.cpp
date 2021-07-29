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
  int sample_pos = 2 * BLI_CROSSINGS * ((int)(position*((float)BLI_OVERSAMPLE)));
  if (idx_work >= BLI_CROSSINGS && idx_work + BLI_CROSSINGS <= CIRCULAR_BUFFER_LEN)
  {
    OscBase::scaleAdd(corrections + idx_work - BLI_CROSSINGS, value, &ljw::Bli::step_corrections[sample_pos], BLI_CROSSINGS*2);
  }
  else
  {
    for (int j=0; j<BLI_CROSSINGS*2; j++)
    {
      corrections[(idx_work - BLI_CROSSINGS + j) % CIRCULAR_BUFFER_LEN] += value*ljw::Bli::step_corrections[sample_pos];
      sample_pos++;
    }

  }
}

void OscBase::scaleAdd(float *__restrict__ target, float multiplier, float *__restrict__ source, unsigned int n)
{
  float32x4_t m = vdupq_n_f32(multiplier);
  float32x4_t x, y;

  for (unsigned int j=0; j<(n & ~3); j += 4)
  {
    x = vld1q_f32(source + j);
    y = vld1q_f32(target + j);
    y = vaddq_f32(y, vmulq_f32(m, x));
    vst1q_f32(target + j, y);
  }
}
