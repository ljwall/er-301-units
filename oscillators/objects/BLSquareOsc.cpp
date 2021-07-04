#include <od/config.h>
#include <hal/ops.h>
#include <hal/log.h>
#include <math.h>

#include "BLSquareOsc.h"
#include "../lib/bli.h"

BLSquareOsc::BLSquareOsc()
{
  ljw::Bli::make();
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mPulseWidth);

  idx_play = 0;
  idx_work = BLI_CROSSINGS;
  naive_saw = 0.0f;

  for (int i=0; i<BLSQR_BUFF_LEN; i++)
  {
    naive_sqr[i] = -0.5f;
    corrections[i] = 0.0f;
  }
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
  int j, sample_pos;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(1, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    last = naive_saw;
    incSaw = last + step;
    naive_saw = incSaw;
    if (naive_saw >=1) naive_saw -= 1.0f;

    naive_sqr[idx_play] = 0.0f;
    corrections[idx_play] = 0.0f;

    idx_work = (idx_work + 1) % BLSQR_BUFF_LEN;
    idx_play = (idx_play + 1) % BLSQR_BUFF_LEN;

    if (incSaw >= pw[i] && !high)
    {
      // Go high
      sample_pos = (int)((incSaw - pw[i])*((float)BLI_OVERSAMPLE)/(step - pw[i] + last_pw));
      for (j=0; j<BLI_CROSSINGS*2; j++)
      {
        corrections[(idx_play + j) % BLSQR_BUFF_LEN] += ljw::Bli::step_corrections[sample_pos];
        sample_pos += BLI_OVERSAMPLE;
      }
      high = true;
    }

    if (naive_saw < incSaw && high)
    {
      // go low
      sample_pos = (int)(naive_saw*((float)BLI_OVERSAMPLE)/step);
      for (j=0; j<BLI_CROSSINGS*2; j++)
      {
        corrections[(idx_play + j) % BLSQR_BUFF_LEN] -= ljw::Bli::step_corrections[sample_pos];
        sample_pos += BLI_OVERSAMPLE;
      }
      high = false;
    }

    if (naive_saw >= pw[i] && !high)
    {
      // Go high again
      sample_pos = (int)((naive_saw - pw[i])*((float)BLI_OVERSAMPLE)/(step - pw[i] + last_pw));
      for (j=0; j<BLI_CROSSINGS*2; j++)
      {
        corrections[(idx_play + j) % BLSQR_BUFF_LEN] += ljw::Bli::step_corrections[sample_pos];
        sample_pos += BLI_OVERSAMPLE;
      }
      high = true;
    }

    last_pw = pw[i];
    naive_sqr[idx_work] = high ? 0.5f : -0.5f;
    out[i] = naive_sqr[idx_play] + corrections[idx_play];
  }
}
