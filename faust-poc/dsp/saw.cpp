#include "saw.h"
#include <od/config.h>

Saw::Saw()
{
  addOutput(mOutput);
  addInput(mFreq);
  DSP = new mydsp();
  DSP->init(globalConfig.sampleRate);
}

Saw::~Saw()
{
  delete(DSP);
}

void Saw::process()
{
  FAUSTFLOAT* inputs[] = { mFreq.buffer() };
  FAUSTFLOAT* outputs[] = { mOutput.buffer() };
  DSP->compute(FRAMELENGTH, inputs, outputs);
}
