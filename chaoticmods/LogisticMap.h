#pragma once

#include <od/objects/Object.h>

class LogisticMap : public od::Object
{
public:
  LogisticMap();
  ~LogisticMap();

#ifndef SWIGLUA
  // These declarations are omitted from the swig wrapper to reduce code size.
  virtual void process();
  od::Inlet mTrigger{"Trigger"};
  od::Outlet mOutput{"Out"};
  od::Parameter mR{"R"};
#endif

protected:
  // Protected declarations are also omitted from the swig wrapper.
  float x = 0.44879895051; // Pi/7
};
