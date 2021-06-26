#include <od/config.h>
#include <hal/ops.h>
#include <math.h>

#include "BLSquareOsc.h"
#include "../lib/bli.h"
#include "../lib/blit.h"

#define R 0.9997

BLSquareOsc::BLSquareOsc()
{
  blit = new ljw::Blit(ljw::Bli::bli, BLI_LEN, BLI_OVERSAMPLE);
  addOutput(mOutput);
  addInput(mVoltPerOctave);
  addInput(mFundamental);
  addInput(mPulseWidth);
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
        *fund = mFundamental.buffer();

  float step;
  float incSaw, nextSaw, x;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    step = fund[i]*exp(CLAMP(-1.0, 1.0, vPerOct[i])*glog2)*globalConfig.samplePeriod;
    incSaw = aliasSaw + step;
    nextSaw = incSaw;
    while (nextSaw >=1) nextSaw = nextSaw - 1;

    if (incSaw > pw[i] && !high)
    {
      // Go high
      blit->addImpulse((incSaw - pw[i])/step, 1.0);
      high = true;
    }

    if (nextSaw < pw[i] && high)
    {
      // go low
      blit->addImpulse(nextSaw / step, -1.0);
      high = false;
    }

    x = blit->next();

    // Lossy running integration
    out[i] = x + R*(i > 0 ? out[i-1] : last);

    aliasSaw = nextSaw;
  }

  last = out[FRAMELENGTH - 1];
}
