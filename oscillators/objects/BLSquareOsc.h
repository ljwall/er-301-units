#pragma once

#include <od/objects/Object.h>
#include "OscBase.h"

class BLSquareOsc : public od::Object
                  , public OscBase
{
  public:
    BLSquareOsc();
    ~BLSquareOsc();

#ifndef SWIGLUA
    // These declarations are omitted from the swig wrapper to reduce code size.
    virtual void process();
    od::Outlet mOutput{"Out"};

    od::Inlet mVoltPerOctave{"V/Oct"};
    od::Inlet mFundamental{"Fundamental"};
    od::Inlet mPulseWidth{"PulseWidth"};
#endif

  protected:
    float naive_saw;
    bool high = false;
    float last_pw = 0.5f;
};
