#include <od/config.h>
#include <hal/ops.h>
#include <math.h>

#include "BLSquareOsc.h"
#include "../lib/bli.h"
#include "../lib/blit.h"

#define R 0.9997f
#define PULSE_SIZE 0.75f

BLSquareOsc::BLSquareOsc()
{
  blit = new ljw::Blit(ljw::Bli::bli, BLI_LEN, BLI_OVERSAMPLE);
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mPulseWidth);
  addInput(mHardSync);
}

BLSquareOsc::~BLSquareOsc()
{
  delete blit;
}

void BLSquareOsc::process()
{
  float glog2 = FULLSCALE_IN_VOLTS * logf(2.0f);
  float *out = mOutput.buffer(),
        *vPerOct = mVoltPerOctave.buffer(),
        *pw = mPulseWidth.buffer(),
        *fund = mFundamental.buffer(),
        *sync = mHardSync.buffer();

  float step;
  float incSaw, nextSaw, x;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = CLAMP(0, 20000, fund[i]*exp(vPerOct[i]*glog2))*globalConfig.samplePeriod;
    incSaw = aliasSaw + step;
    if (sync[i] == 0.0f)
    {
      nextSaw = incSaw;
      while (nextSaw >=1) nextSaw = nextSaw - 1;
    }
    else
    {
      nextSaw = 0;
    }

    if (incSaw > pw[i] && !high)
    {
      // Go high
      blit->addImpulse((incSaw - pw[i])/step, PULSE_SIZE);
      high = true;
    }

    if (nextSaw < incSaw && high)
    {
      // go low
      blit->addImpulse(nextSaw / step, -PULSE_SIZE);
      high = false;
    }

    if (nextSaw > pw[i] && !high)
    {
      // Go high again
      blit->addImpulse((nextSaw - pw[i])/step, PULSE_SIZE);
      high = true;
    }

    x = blit->next();

    // Lossy running integration
    last = out[i] = x + R*last;

    aliasSaw = nextSaw;
  }

  //last = out[FRAMELENGTH - 1];
}