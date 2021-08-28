#pragma once

#include <od/objects/Object.h>

class Saw : public od::Object
{
  public:
    Saw();
    ~Saw();

#ifndef SWIGLUA
    virtual void process();
    od::Outlet mOutput{"Out"};
    od::Inlet mFreq{"Freq"};
#endif

  private:
    void* DSP;
};
