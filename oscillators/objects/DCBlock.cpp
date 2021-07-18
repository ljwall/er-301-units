#include <od/config.h>
#include "DCBlock.h"

DCBlock::DCBlock()
{
  addOutput(mOutput);
  addInput(mInput);
  lastX = 0.0f;
  lastY = 0.0f;
}

DCBlock::~DCBlock()
{
}

void DCBlock::process()
{
  float *out = mOutput.buffer(),
        *in = mInput.buffer();

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    out[i] = in[i] - lastX + DC_LOSS*lastY;
    lastY = out[i];
    lastX = in[i];
  }
}
