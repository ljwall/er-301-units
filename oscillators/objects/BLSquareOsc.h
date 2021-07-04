#pragma once

#include <od/objects/Object.h>
#include "../lib/bli.h"

#define BLSQR_BUFF_LEN  (BLI_LEN * 2)

class BLSquareOsc : public od::Object
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
    float naive_sqr[BLSQR_BUFF_LEN];
    float corrections[BLSQR_BUFF_LEN];
    int idx_work, idx_play;
    bool high = false;
    float last_pw = 0.5f;
};
