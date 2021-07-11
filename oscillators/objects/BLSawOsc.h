#pragma once

#include <od/objects/Object.h>
#include "../lib/bli.h"

#define BLS_BUFF_LEN  (BLI_LEN * 2)

class BLSawOsc : public od::Object
{
  public:
    BLSawOsc();
    ~BLSawOsc();

#ifndef SWIGLUA
    // These declarations are omitted from the swig wrapper to reduce code size.
    virtual void process();
    od::Outlet mOutput{"Out"};

    od::Inlet mVoltPerOctave{"V/Oct"};
    od::Inlet mFundamental{"Fundamental"};
#endif

  protected:
    float naive_saw[BLS_BUFF_LEN];
    float corrections[BLS_BUFF_LEN];
    int idx_work, idx_play;

    void applyStep(float value, float position);
};
