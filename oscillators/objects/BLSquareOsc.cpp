#include <od/config.h>
#include <hal/log.h>
#include <hal/ops.h>
#include <hal/simd.h>
#include <math.h>

#include "BLSquareOsc.h"

BLSquareOsc::BLSquareOsc()
{
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mPulseWidth);

  naive_saw = 0.0f;
}

BLSquareOsc::~BLSquareOsc()
{
}

void BLSquareOsc::process()
{
  float *out = mOutput.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *pw = mPulseWidth.buffer(),
        *fund = mFundamental.buffer();

  float step[4], last, incSaw;
  int i, j, k;

  float32x4_t glog2 = vdupq_n_f32(FULLSCALE_IN_VOLTS * logf(2.0f));
  float32x4_t zero = vdupq_n_f32(0.0f);
  float32x4_t one = vdupq_n_f32(1.0f);
  float32x4_t negOne = vdupq_n_f32(-1.0f);
  float32x4_t sr = vdupq_n_f32(globalConfig.samplePeriod);

  for (j = 0; j < FRAMELENGTH; j += 4)
  {
    // Load the V/oct input into a SIMD vector.
    float32x4_t q = vld1q_f32(vPerOct + j);
    // Load the linear freq (f0) input into a SIMD vector.
    float32x4_t dP = sr * vld1q_f32(fund + j);
    // Clamp the V/oct input to [-1,1] (i.e. -10V to 10V)
    q = vminq_f32(one, q);
    q = vmaxq_f32(negOne, q);
    // Scale the linear frequency by the V/oct input
    q = dP * simd_exp(q * glog2);

    vst1q_f32(step, q);

    for (k = 0; k < 4; k++)
    {
      i = j + k;
      last = naive_saw;
      incSaw = last + step[k];
      naive_saw = incSaw;
      if (naive_saw >=1) naive_saw -= 1.0f;

      idx_work = (idx_work + 1) % CIRCULAR_BUFFER_LEN;

      if (incSaw >= pw[i] && !high)
      {
        // Go high
        applyJump(1.0f, (incSaw - pw[i])/(step[k] - pw[i] + last_pw));
        high = true;
      }

      if (naive_saw < incSaw && high)
      {
        // go low
        applyJump(-1.0f, naive_saw/step[k]);
        high = false;
      }

      if (naive_saw >= pw[i] && !high)
      {
        // Go high again
        applyJump(1.0f, (naive_saw - pw[i])/(step[k] - pw[i] + last_pw));
        high = true;
      }

      last_pw = pw[i];
      naive_wave[idx_work] = high ? 0.5f : -0.5f;
    }

    float32x4_t n = vld1q_f32(naive_wave + idx_play);
    float32x4_t c = vld1q_f32(corrections + idx_play);
    vst1q_f32(out+j, n + c);
    vst1q_f32(corrections + idx_play, zero);
    idx_play = (idx_play + 4) % CIRCULAR_BUFFER_LEN;
  }
}
