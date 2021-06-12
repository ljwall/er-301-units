#pragma once

#include <od/objects/Object.h>

// Output variable choices
#define LORENZ_OUT_X 1
#define LORENZ_OUT_Y 2
#define LORENZ_OUT_Z 3

class Lorenz : public od::Object
{
public:
  Lorenz();
  ~Lorenz();

#ifndef SWIGLUA
  // These declarations are omitted from the swig wrapper to reduce code size.
  virtual void process();
  od::Outlet mOutput{"Out"};
  od::Parameter mRate{"Rate"};
  od::Parameter mRho{"Rho"};
  od::Parameter mSigma{"Sigma"};
  od::Parameter mBeta{"Beta"};
  od::Option mOutVar{"OutVar", LORENZ_OUT_X};
#endif

protected:
  float lastX = 1.0f;
  float lastY = 1.0f;
  float lastZ = 1.0f;
};
