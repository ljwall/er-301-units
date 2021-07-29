#include <od/config.h>
#include <hal/ops.h>
#include <hal/simd.h>
#include <math.h>

#include "BLSawOsc.h"

BLSawOsc::BLSawOsc()
{
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mSync);
}

BLSawOsc::~BLSawOsc()
{
}

void BLSawOsc::process()
{
  //float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *sync = mSync.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *fund = mFundamental.buffer();

  float step[4], last, syncFraction, resetFraction;
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
      last = naive_wave[idx_work];

      idx_work = (idx_work + 1) % CIRCULAR_BUFFER_LEN;

      naive_wave[idx_work] = last + step[k];

      if (sync[i] > 0 && lastSync <=0)
      {
        syncFraction = sync[i] / (sync[i] - lastSync);
        resetFraction = (naive_wave[idx_work] - 0.5f) / step[k];

        if (resetFraction > syncFraction)
        {
          // Do a reset and _then_ a sync
          applyJump(-1.0f, resetFraction);
          applyJump(-step[k]*(resetFraction - syncFraction), syncFraction);
          naive_wave[idx_work] += -step[k]*(resetFraction - syncFraction) - 1.0f;
        }
        else
        {
          // Sync is comming first, reset (if it was expected) redundant
          applyJump(-(0.5f + last + step[k]*(1.0f - syncFraction)), syncFraction);
          naive_wave[idx_work] += -(0.5f + last + step[k]*(1.0f - syncFraction));
        }

      }
      else if (naive_wave[idx_work] >= 0.5f)
      {
        resetFraction = (naive_wave[idx_work] - 0.5f) / step[k];
        applyJump(-1.0f, resetFraction);
        naive_wave[idx_work] += -1.0f;
      }

      lastSync = sync[i];
    }

    float32x4_t n = vld1q_f32(naive_wave + idx_play);
    float32x4_t c = vld1q_f32(corrections + idx_play);
    vst1q_f32(out+j, n + c);
    vst1q_f32(corrections + idx_play, zero);
    idx_play = (idx_play + 4) % CIRCULAR_BUFFER_LEN;
  }
}
