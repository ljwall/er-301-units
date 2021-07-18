#pragma once

#include <od/objects/Object.h>

#define DC_LOSS 0.9995

class DCBlock : public od::Object
{
  public:
    DCBlock();
    ~DCBlock();

#ifndef SWIGLUA
    // These declarations are omitted from the swig wrapper to reduce code size.
    virtual void process();
    od::Inlet mInput{"In"};
    od::Outlet mOutput{"Out"};
#endif

  protected:
    float lastX, lastY;
};
