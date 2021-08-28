<<includeIntrinsic>>

#include <od/objects/Object.h>

#define FAUSTFLOAT float

#ifndef SWIGLUA
class dsp {};

struct Meta {
  virtual void declare(const char* key, const char* value) {};
};

struct Soundfile {
  FAUSTFLOAT** fBuffers;
  int* fLength;   // length of each part
  int* fSR;       // sample rate of each part
  int* fOffset;   // offset of each part in the global buffer
  int fChannels;  // max number of channels of all concatenated files
};

struct UI {
  // -- widget's layouts
  virtual void openTabBox(const char* label) {}
  virtual void openHorizontalBox(const char* label) {}
  virtual void openVerticalBox(const char* label) {}
  virtual void closeBox() {}

  // -- active widgets
  virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
  virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
  virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}

  // -- passive widgets
  virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}

  // -- soundfiles
  virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

  // -- metadata declarations
  virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
};

<<includeclass>>
#endif


class Saw : public od::Object
{
  public:
    Saw();
    ~Saw();

#ifndef SWIGLUA
    virtual void process();
    od::Outlet mOutput{"Out"};
    od::Inlet mFreq{"Freq"};

  private:
    mydsp DSP;

#endif

};
