#include <od/config.h>
#include <hal/ops.h>
#include <hal/log.h>
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
  float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *pw = mPulseWidth.buffer(),
        *fund = mFundamental.buffer();

  float step, last, incSaw;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(1, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    last = naive_saw;
    incSaw = last + step;
    naive_saw = incSaw;
    if (naive_saw >=1) naive_saw -= 1.0f;

    naive_wave[idx_play] = 0.0f;
    corrections[idx_play] = 0.0f;

    idx_work = (idx_work + 1) % CIRCULAR_BUFFER_LEN;
    idx_play = (idx_play + 1) % CIRCULAR_BUFFER_LEN;

    if (incSaw >= pw[i] && !high)
    {
      // Go high
      applyJump(1.0f, (incSaw - pw[i])/(step - pw[i] + last_pw));
      high = true;
    }

    if (naive_saw < incSaw && high)
    {
      // go low
      applyJump(-1.0f, naive_saw/step);
      high = false;
    }

    if (naive_saw >= pw[i] && !high)
    {
      // Go high again
      applyJump(1.0f, (naive_saw - pw[i])/(step - pw[i] + last_pw));
      high = true;
    }

    last_pw = pw[i];
    naive_wave[idx_work] = high ? 0.5f : -0.5f;
    out[i] = naive_wave[idx_play] + corrections[idx_play];
  }
}
