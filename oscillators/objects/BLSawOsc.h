#pragma once

#include <od/objects/Object.h>

#include "OscBase.h"

//#define BLS_BUFF_LEN  (BLI_LEN * 2)

class BLSawOsc : public od::Object
               , public OscBase
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
    od::Inlet mSync{"Sync"};
#endif

  protected:
    float lastSync = 0.0f;
};
