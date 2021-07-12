#include <od/config.h>
#include <hal/ops.h>
#include <hal/simd.h>
#include <math.h>

#include "BLSawOsc.h"
#include "../lib/bli.h"

BLSawOsc::BLSawOsc()
{
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mSync);

  idx_play = 0;
  idx_work = BLI_CROSSINGS;

  simd_set(naive_saw, BLS_BUFF_LEN, 0.0f);
  simd_set(corrections, BLS_BUFF_LEN, 0.0f);
}

BLSawOsc::~BLSawOsc()
{
}

void BLSawOsc::process()
{
  float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *sync = mSync.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *fund = mFundamental.buffer();

  float step, last, syncFraction, resetFraction;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(1, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    last = naive_saw[idx_work];

    naive_saw[idx_play] = 0.0f;
    corrections[idx_play] = 0.0f;

    idx_work = (idx_work + 1) % BLS_BUFF_LEN;
    idx_play = (idx_play + 1) % BLS_BUFF_LEN;

    naive_saw[idx_work] = last + step;
    resetFraction = (naive_saw[idx_work] - 0.5f) / step;

    if (sync[i] > 0 && lastSync <=0)
    {
      syncFraction = sync[i] / (sync[i] - lastSync);

      if (resetFraction > syncFraction)
      {
        // Do a reset and _then_ a sync
        applyJump(-1.0f, resetFraction);
        applyJump(-step*(resetFraction - syncFraction), syncFraction);
      }
      else
      {
        // Sync is comming first, reset (if it was expected) redundant
        applyJump(-(0.5f + last + step*(1.0f - syncFraction)), syncFraction);
      }

    }
    else if (resetFraction >= 0.0f)
    {
      applyJump(-1.0f, resetFraction);
    }

    lastSync = sync[i];
    out[i] = naive_saw[idx_play] + corrections[idx_play];
  }
}

void BLSawOsc::applyJump(float value, float position)
{
    int sample_pos = (int)(position*((float)BLI_OVERSAMPLE));
    for (int j=0; j<BLI_CROSSINGS*2; j++)
    {
      corrections[(idx_play + j) % BLS_BUFF_LEN] += value*ljw::Bli::step_corrections[sample_pos];
      sample_pos += BLI_OVERSAMPLE;
    }

    naive_saw[idx_work] += value;
}
