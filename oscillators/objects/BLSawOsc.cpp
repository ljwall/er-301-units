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
  float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *sync = mSync.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *fund = mFundamental.buffer();

  float step, last, syncFraction, resetFraction;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(1, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    last = naive_wave[idx_work];

    naive_wave[idx_play] = 0.0f;
    corrections[idx_play] = 0.0f;

    idx_work = (idx_work + 1) % CIRCULAR_BUFFER_LEN;
    idx_play = (idx_play + 1) % CIRCULAR_BUFFER_LEN;

    naive_wave[idx_work] = last + step;
    resetFraction = (naive_wave[idx_work] - 0.5f) / step;

    if (sync[i] > 0 && lastSync <=0)
    {
      syncFraction = sync[i] / (sync[i] - lastSync);

      if (resetFraction > syncFraction)
      {
        // Do a reset and _then_ a sync
        applyJump(-1.0f, resetFraction);
        applyJump(-step*(resetFraction - syncFraction), syncFraction);
        naive_wave[idx_work] += -step*(resetFraction - syncFraction) - 1.0f;
      }
      else
      {
        // Sync is comming first, reset (if it was expected) redundant
        applyJump(-(0.5f + last + step*(1.0f - syncFraction)), syncFraction);
        naive_wave[idx_work] += -(0.5f + last + step*(1.0f - syncFraction));
      }

    }
    else if (resetFraction >= 0.0f)
    {
      applyJump(-1.0f, resetFraction);
      naive_wave[idx_work] += -1.0f;
    }

    lastSync = sync[i];
    out[i] = naive_wave[idx_play] + corrections[idx_play];
  }
}
