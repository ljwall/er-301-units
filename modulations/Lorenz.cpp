#include "Lorenz.h"
#include <od/config.h>
#include <hal/ops.h>
#include <math.h>

Lorenz::Lorenz()
{
  addOutput(mOutput);
  addParameter(mRate);
  addParameter(mRho);
  addParameter(mSigma);
  addParameter(mBeta);
  addOption(mOutVar);
}

Lorenz::~Lorenz()
{
}

void Lorenz::process()
{
  float *out = mOutput.buffer();

  float rho = CLAMP(0, 100, mRho.target());
  float sigma = CLAMP(0, 100, mSigma.target());
  float beta = CLAMP(0, 40, mBeta.target());

  float dt = exp(mRate.target())*globalConfig.samplePeriod;

  float nextX, nextY, nextZ;

  int outVar = mOutVar.value();

  for (int i = 0; i < FRAMELENGTH; i++)
  {
    nextX = CLAMP(-200.0, 200.0, lastX + dt*sigma*(lastY - lastX)); // 78.5 45 3.7
    nextY = CLAMP(-200.0, 200.0, lastY + dt*(lastX*(rho - lastZ) - lastY));
    nextZ = CLAMP(-200.0, 200.0, lastZ + dt*(lastX*lastY - beta*lastZ));

    lastX = nextX;
    lastY = nextY;
    lastZ = nextZ;

    // Divide 50 to Normalise output to be (roughly) in [-1, 1]
    out[i] = (outVar == LORENZ_OUT_X ? nextX :
              outVar == LORENZ_OUT_Y ? nextY
                                     : nextZ) / 50.0;
  }
}
