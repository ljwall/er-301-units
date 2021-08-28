#include "saw.h"
#include <od/config.h>

Saw::Saw() : DSP()
{
  addOutput(mOutput);
  addInput(mFreq);
  DSP.init(globalConfig.sampleRate);
}

Saw::~Saw()
{
}

void Saw::process()
{
  FAUSTFLOAT* inputs[] = { mFreq.buffer() };
  FAUSTFLOAT* outputs[] = { mOutput.buffer() };
  DSP.compute(FRAMELENGTH, inputs, outputs);
}
