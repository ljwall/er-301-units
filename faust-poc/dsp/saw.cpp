/* ------------------------------------------------------------
name: "saw"
Code generated with Faust 2.20.2 (https://faust.grame.fr)
Compilation options: -lang cpp -vec -lv 0 -vs 4 -ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include "saw.h"
#include <od/config.h>

#define FAUSTFLOAT float

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

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0_perm[4];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("filename", "saw.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "saw");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 0;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (1.0f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 4); l0 = (l0 + 1)) {
			fRec0_perm[l0] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("saw");
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* output0_ptr = outputs[0];
		float fZec0[4];
		float fZec1[4];
		float fZec2[4];
		int iZec3[4];
		float fRec0_tmp[8];
		float* fRec0 = &fRec0_tmp[4];
		float fRec1[4];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; (vindex <= (count - 4)); vindex = (vindex + 4)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = 4;
			/* Vectorizable loop 0 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = std::max<float>(1.00000001e-07f, std::fabs(float(input0[i])));
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (fRec0[(i - 1)] + (fConst1 * fZec0[i]));
				fZec2[i] = (fZec1[i] + -1.0f);
				iZec3[i] = (fZec2[i] < 0.0f);
				fRec0[i] = (iZec3[i] ? fZec1[i] : fZec2[i]);
				fRec1[i] = (iZec3[i] ? fZec1[i] : (fZec1[i] + ((1.0f - (fConst0 / fZec0[i])) * fZec2[i])));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec0_perm[j1] = fRec0_tmp[(vsize + j1)];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT(((2.0f * fRec1[i]) + -1.0f));
			}
		}
		/* Remaining frames */
		if (vindex < count) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = (count - vindex);
			/* Vectorizable loop 0 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = std::max<float>(1.00000001e-07f, std::fabs(float(input0[i])));
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (fRec0[(i - 1)] + (fConst1 * fZec0[i]));
				fZec2[i] = (fZec1[i] + -1.0f);
				iZec3[i] = (fZec2[i] < 0.0f);
				fRec0[i] = (iZec3[i] ? fZec1[i] : fZec2[i]);
				fRec1[i] = (iZec3[i] ? fZec1[i] : (fZec1[i] + ((1.0f - (fConst0 / fZec0[i])) * fZec2[i])));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec0_perm[j1] = fRec0_tmp[(vsize + j1)];
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT(((2.0f * fRec1[i]) + -1.0f));
			}
		}
	}

};

Saw::Saw()
{
  addOutput(mOutput);
  addInput(mFreq);
  DSP = new mydsp();
  ((mydsp*)DSP)->init(globalConfig.sampleRate);
}

Saw::~Saw()
{
  delete((mydsp*)DSP);
}

void Saw::process()
{
  FAUSTFLOAT* inputs[] = { mFreq.buffer() };
  FAUSTFLOAT* outputs[] = { mOutput.buffer() };
  ((mydsp*)DSP)->compute(FRAMELENGTH, inputs, outputs);
}

#endif
