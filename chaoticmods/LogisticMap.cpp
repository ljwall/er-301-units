#include "LogisticMap.h"
#include <od/config.h>
#include <hal/ops.h>

LogisticMap::LogisticMap()
{
  addInput(mTrigger);
  addOutput(mOutput);
  addParameter(mR);
  mR.enableSerialization();
}

LogisticMap::~LogisticMap()
{
}

void LogisticMap::process()
{
  float r;
  float *trig = mTrigger.buffer();
  float *out = mOutput.buffer();

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    if (trig[i] > 0.0f)
    {
      r = CLAMP(0, 4, mR.target());
      x = CLAMP(0, 1, r * x * (1 - x));

      // prevent it locking up
      if (x == 0)
      {
        x = 0.00001f;
      }
    }

    out[i] = x;
  }
}
