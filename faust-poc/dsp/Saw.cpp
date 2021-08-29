#include "Saw.h"
#include <od/config.h>
Saw::Saw() : DSP()
{
    addInput(mIn);
    addOutput(mOut);
}
Saw::~Saw()
{ }
void Saw::process()
{
    FAUSTFLOAT* inputs[] = { mIn.buffer() };
    FAUSTFLOAT* outputs[] = { mOut.buffer() };
    DSP.compute(FRAMELENGTH, inputs, outputs);
}

