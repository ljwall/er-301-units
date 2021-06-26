#include <od/config.h>

#include "BLSquareOsc.h"
#include "../lib/bli.h"
#include "../lib/blit.h"

#define R 0.9997

BLSquareOsc::BLSquareOsc()
{
  blit = new ljw::Blit(ljw::Bli::bli, BLI_LEN, BLI_OVERSAMPLE);
  addOutput(mOutput);
}

BLSquareOsc::~BLSquareOsc()
{
}

void BLSquareOsc::process()
{
  float *out = mOutput.buffer();

  float step = 27*globalConfig.samplePeriod;
  float incSaw, nextSaw, x;
  float pw = 0.5;

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    incSaw = aliasSaw + step;
    nextSaw = incSaw;
    while (nextSaw >=1) nextSaw = nextSaw - 1;

    if (incSaw > pw && !high)
    {
      // Go high
      blit->addImpulse((incSaw - pw)/step, 1.0);
      high = true;
    }

    if (nextSaw < pw && high)
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
