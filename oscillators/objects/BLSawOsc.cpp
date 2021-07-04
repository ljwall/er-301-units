#include <od/config.h>
#include <hal/ops.h>
#include <math.h>

#include "BLSawOsc.h"
#include "../lib/bli.h"

BLSawOsc::BLSawOsc()
{
  ljw::Bli::make();
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  idx_play = 0;
  idx_work = BLI_CROSSINGS;

  for (int i=0; i<BLS_BUFF_LEN; i++)
  {
    naive_saw[i] = corrections[i] = 0.0f;
  }
}

BLSawOsc::~BLSawOsc()
{
}

void BLSawOsc::process()
{
  float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *fund = mFundamental.buffer();

  float step, last;
  int j, sample_pos;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(0, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    last = naive_saw[idx_work];

    naive_saw[idx_play] = 0.0f;
    corrections[idx_play] = 0.0f;

    idx_work = (idx_work + 1) % BLS_BUFF_LEN;
    idx_play = (idx_play + 1) % BLS_BUFF_LEN;

    naive_saw[idx_work] = last + step;

    if (naive_saw[idx_work] >= 0.5f)
    {
      sample_pos = (int)((naive_saw[idx_work] - 0.5f)*((float)BLI_OVERSAMPLE)/step);
      for (j=0; j<BLI_CROSSINGS*2; j++)
      {
        corrections[(idx_play + j) % BLS_BUFF_LEN] -= ljw::Bli::step_corrections[sample_pos];
        sample_pos += BLI_OVERSAMPLE;
      }

      naive_saw[idx_work] -= 1;
    }

    out[i] = naive_saw[idx_play] + corrections[idx_play];
  }
}
