#pragma once

#include <od/objects/Object.h>
#include "../lib/blit.h"

class BLSquareOsc : public od::Object
{
  public:
    BLSquareOsc();
    ~BLSquareOsc();

#ifndef SWIGLUA
    // These declarations are omitted from the swig wrapper to reduce code size.
    virtual void process();
    od::Outlet mOutput{"Out"};
#endif
  protected:
    ljw::Blit* blit;
    float aliasSaw = 0, last = 0;
    bool high = false;
};
