/* ------------------------------------------------------------
name: "Saw"
Code generated with Faust 2.34.4 (https://faust.grame.fr)
Compilation options: -a /tmp/faust.KQPfXX/Saw/Saw.h -lang cpp -es 1 -vec -lv 0 -vs 4 -single -ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__


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

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	float fRec10_perm[4];
	float fYec0[256];
	int fYec0_idx;
	int fYec0_idx_save;
	float fRec8_perm[4];
	float fYec1[128];
	int fYec1_idx;
	int fYec1_idx_save;
	float fRec6_perm[4];
	float fYec2[512];
	int fYec2_idx;
	int fYec2_idx_save;
	float fRec4_perm[4];
	float fYec3[512];
	int fYec3_idx;
	int fYec3_idx_save;
	float fRec2_perm[4];
	float fYec4[1024];
	int fYec4_idx;
	int fYec4_idx_save;
	float fRec14_perm[4];
	float fYec5[8192];
	int fYec5_idx;
	int fYec5_idx_save;
	float fRec13_perm[4];
	float fYec6[4096];
	int fYec6_idx;
	int fYec6_idx_save;
	float fRec11_perm[4];
	float fYec7[4096];
	int fYec7_idx;
	int fYec7_idx_save;
	float fRec0_perm[4];
	float fYec8[1024];
	int fYec8_idx;
	int fYec8_idx_save;
	float fRec19_perm[4];
	float fYec9[8192];
	int fYec9_idx;
	int fYec9_idx_save;
	float fRec18_perm[4];
	float fYec10[2048];
	int fYec10_idx;
	int fYec10_idx_save;
	float fRec16_perm[4];
	float fYec11[2048];
	int fYec11_idx;
	int fYec11_idx_save;
	float fRec1_perm[4];
	int fSampleRate;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.2");
		m->declare("compile_options", "-a /tmp/faust.KQPfXX/Saw/Saw.h -lang cpp -es 1 -vec -lv 0 -vs 4 -single -ftz 0 -mcd 16");
		m->declare("er301_input", "In");
		m->declare("er301_output", "Out");
		m->declare("filename", "Saw.dsp");
		m->declare("name", "Saw");
		m->declare("reverbs.lib/dattorro_rev:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev:license", "MIT-style STK-4.3 license");
		m->declare("reverbs.lib/dattorro_rev_default:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev_default:license", "MIT-style STK-4.3 license");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
	}
	
	virtual void instanceResetUserInterface() {
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 4); l0 = (l0 + 1)) {
			fRec10_perm[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 256); l1 = (l1 + 1)) {
			fYec0[l1] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		for (int l2 = 0; (l2 < 4); l2 = (l2 + 1)) {
			fRec8_perm[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 128); l3 = (l3 + 1)) {
			fYec1[l3] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		for (int l4 = 0; (l4 < 4); l4 = (l4 + 1)) {
			fRec6_perm[l4] = 0.0f;
		}
		for (int l5 = 0; (l5 < 512); l5 = (l5 + 1)) {
			fYec2[l5] = 0.0f;
		}
		fYec2_idx = 0;
		fYec2_idx_save = 0;
		for (int l6 = 0; (l6 < 4); l6 = (l6 + 1)) {
			fRec4_perm[l6] = 0.0f;
		}
		for (int l7 = 0; (l7 < 512); l7 = (l7 + 1)) {
			fYec3[l7] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		for (int l8 = 0; (l8 < 4); l8 = (l8 + 1)) {
			fRec2_perm[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 1024); l9 = (l9 + 1)) {
			fYec4[l9] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		for (int l10 = 0; (l10 < 4); l10 = (l10 + 1)) {
			fRec14_perm[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 8192); l11 = (l11 + 1)) {
			fYec5[l11] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l12 = 0; (l12 < 4); l12 = (l12 + 1)) {
			fRec13_perm[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 4096); l13 = (l13 + 1)) {
			fYec6[l13] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		for (int l14 = 0; (l14 < 4); l14 = (l14 + 1)) {
			fRec11_perm[l14] = 0.0f;
		}
		for (int l15 = 0; (l15 < 4096); l15 = (l15 + 1)) {
			fYec7[l15] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		for (int l16 = 0; (l16 < 4); l16 = (l16 + 1)) {
			fRec0_perm[l16] = 0.0f;
		}
		for (int l17 = 0; (l17 < 1024); l17 = (l17 + 1)) {
			fYec8[l17] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
		for (int l18 = 0; (l18 < 4); l18 = (l18 + 1)) {
			fRec19_perm[l18] = 0.0f;
		}
		for (int l19 = 0; (l19 < 8192); l19 = (l19 + 1)) {
			fYec9[l19] = 0.0f;
		}
		fYec9_idx = 0;
		fYec9_idx_save = 0;
		for (int l20 = 0; (l20 < 4); l20 = (l20 + 1)) {
			fRec18_perm[l20] = 0.0f;
		}
		for (int l21 = 0; (l21 < 2048); l21 = (l21 + 1)) {
			fYec10[l21] = 0.0f;
		}
		fYec10_idx = 0;
		fYec10_idx_save = 0;
		for (int l22 = 0; (l22 < 4); l22 = (l22 + 1)) {
			fRec16_perm[l22] = 0.0f;
		}
		for (int l23 = 0; (l23 < 2048); l23 = (l23 + 1)) {
			fYec11[l23] = 0.0f;
		}
		fYec11_idx = 0;
		fYec11_idx_save = 0;
		for (int l24 = 0; (l24 < 4); l24 = (l24 + 1)) {
			fRec1_perm[l24] = 0.0f;
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
		ui_interface->openVerticalBox("Saw");
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* output0_ptr = outputs[0];
		float fRec10_tmp[8];
		float* fRec10 = &fRec10_tmp[4];
		float fRec8_tmp[8];
		float* fRec8 = &fRec8_tmp[4];
		float fRec9[4];
		float fRec6_tmp[8];
		float* fRec6 = &fRec6_tmp[4];
		float fRec7[4];
		float fRec4_tmp[8];
		float* fRec4 = &fRec4_tmp[4];
		float fRec5[4];
		float fRec2_tmp[8];
		float* fRec2 = &fRec2_tmp[4];
		float fRec3[4];
		float fRec14_tmp[8];
		float* fRec14 = &fRec14_tmp[4];
		float fRec15[4];
		float fRec13_tmp[8];
		float* fRec13 = &fRec13_tmp[4];
		float fRec11_tmp[8];
		float* fRec11 = &fRec11_tmp[4];
		float fRec12[4];
		float fRec0_tmp[8];
		float* fRec0 = &fRec0_tmp[4];
		float fRec19_tmp[8];
		float* fRec19 = &fRec19_tmp[4];
		float fRec20[4];
		float fRec18_tmp[8];
		float* fRec18 = &fRec18_tmp[4];
		float fRec16_tmp[8];
		float* fRec16 = &fRec16_tmp[4];
		float fRec17[4];
		float fRec1_tmp[8];
		float* fRec1 = &fRec1_tmp[4];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; (vindex <= (count - 4)); vindex = (vindex + 4)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = 4;
			/* Recursive loop 0 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec10_tmp[j0] = fRec10_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec10[i] = ((0.000500000024f * fRec10[(i - 2)]) + (0.999499977f * float(input0[i])));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec10_perm[j1] = fRec10_tmp[(vsize + j1)];
			}
			/* Recursive loop 1 */
			/* Pre code */
			fYec0_idx = ((fYec0_idx + fYec0_idx_save) & 255);
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec8_tmp[j2] = fRec8_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec0[((i + fYec0_idx) & 255)] = (fRec10[i] - (0.75f * fRec8[(i - 1)]));
				fRec8[i] = fYec0[(((i + fYec0_idx) - 142) & 255)];
				fRec9[i] = (0.75f * fYec0[((i + fYec0_idx) & 255)]);
			}
			/* Post code */
			fYec0_idx_save = vsize;
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec8_perm[j3] = fRec8_tmp[(vsize + j3)];
			}
			/* Recursive loop 2 */
			/* Pre code */
			fYec1_idx = ((fYec1_idx + fYec1_idx_save) & 127);
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec6_tmp[j4] = fRec6_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[((i + fYec1_idx) & 127)] = ((fRec9[i] + fRec8[(i - 1)]) - (0.75f * fRec6[(i - 1)]));
				fRec6[i] = fYec1[(((i + fYec1_idx) - 107) & 127)];
				fRec7[i] = (0.75f * fYec1[((i + fYec1_idx) & 127)]);
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec6_perm[j5] = fRec6_tmp[(vsize + j5)];
			}
			/* Recursive loop 3 */
			/* Pre code */
			fYec2_idx = ((fYec2_idx + fYec2_idx_save) & 511);
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec2[((i + fYec2_idx) & 511)] = ((fRec7[i] + fRec6[(i - 1)]) - (0.625f * fRec4[(i - 1)]));
				fRec4[i] = fYec2[(((i + fYec2_idx) - 379) & 511)];
				fRec5[i] = (0.625f * fYec2[((i + fYec2_idx) & 511)]);
			}
			/* Post code */
			fYec2_idx_save = vsize;
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec4_perm[j7] = fRec4_tmp[(vsize + j7)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 511);
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec2_tmp[j8] = fRec2_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec3[((i + fYec3_idx) & 511)] = ((fRec5[i] + fRec4[(i - 1)]) - (0.625f * fRec2[(i - 1)]));
				fRec2[i] = fYec3[(((i + fYec3_idx) - 277) & 511)];
				fRec3[i] = (0.625f * fYec3[((i + fYec3_idx) & 511)]);
			}
			/* Post code */
			fYec3_idx_save = vsize;
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec2_perm[j9] = fRec2_tmp[(vsize + j9)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 1023);
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec14_tmp[j10] = fRec14_perm[j10];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 8191);
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec13_tmp[j12] = fRec13_perm[j12];
			}
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 4095);
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec11_tmp[j14] = fRec11_perm[j14];
			}
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 4095);
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec0_tmp[j16] = fRec0_perm[j16];
			}
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 1023);
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec19_tmp[j18] = fRec19_perm[j18];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 8191);
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec18_tmp[j20] = fRec18_perm[j20];
			}
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 2047);
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 2047);
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec1_tmp[j24] = fRec1_perm[j24];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec4[((i + fYec4_idx) & 1023)] = ((0.699999988f * fRec14[(i - 1)]) + fRec1[(i - 1)]);
				fRec14[i] = fYec4[(((i + fYec4_idx) - 908) & 1023)];
				fRec15[i] = (0.0f - (0.699999988f * fYec4[((i + fYec4_idx) & 1023)]));
				fYec5[((i + fYec5_idx) & 8191)] = (fRec15[i] + fRec14[(i - 1)]);
				fRec13[i] = ((0.000500000024f * fRec13[(i - 1)]) + (0.999499977f * fYec5[(((i + fYec5_idx) - 4217) & 8191)]));
				fYec6[((i + fYec6_idx) & 4095)] = (fRec13[i] - fRec11[(i - 1)]);
				fRec11[i] = (0.5f * fYec6[(((i + fYec6_idx) - 2656) & 4095)]);
				fRec12[i] = (0.25f * fYec6[((i + fYec6_idx) & 4095)]);
				fYec7[((i + fYec7_idx) & 4095)] = (fRec12[i] + fRec11[(i - 1)]);
				fRec0[i] = (fRec2[(i - 1)] + (fRec3[i] + (0.5f * fYec7[(((i + fYec7_idx) - 2656) & 4095)])));
				fYec8[((i + fYec8_idx) & 1023)] = ((0.699999988f * fRec19[(i - 1)]) + fRec0[(i - 1)]);
				fRec19[i] = fYec8[(((i + fYec8_idx) - 672) & 1023)];
				fRec20[i] = (0.0f - (0.699999988f * fYec8[((i + fYec8_idx) & 1023)]));
				fYec9[((i + fYec9_idx) & 8191)] = (fRec20[i] + fRec19[(i - 1)]);
				fRec18[i] = ((0.000500000024f * fRec18[(i - 1)]) + (0.999499977f * fYec9[(((i + fYec9_idx) - 4453) & 8191)]));
				fYec10[((i + fYec10_idx) & 2047)] = (fRec18[i] - fRec16[(i - 1)]);
				fRec16[i] = (0.5f * fYec10[(((i + fYec10_idx) - 1800) & 2047)]);
				fRec17[i] = (0.25f * fYec10[((i + fYec10_idx) & 2047)]);
				fYec11[((i + fYec11_idx) & 2047)] = (fRec17[i] + fRec16[(i - 1)]);
				fRec1[i] = ((fRec3[i] + fRec2[(i - 1)]) + (0.5f * fYec11[(((i + fYec11_idx) - 1800) & 2047)]));
			}
			/* Post code */
			fYec11_idx_save = vsize;
			fYec10_idx_save = vsize;
			fYec9_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec19_perm[j19] = fRec19_tmp[(vsize + j19)];
			}
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec18_perm[j21] = fRec18_tmp[(vsize + j21)];
			}
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec16_perm[j23] = fRec16_tmp[(vsize + j23)];
			}
			fYec7_idx_save = vsize;
			fYec6_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec14_perm[j11] = fRec14_tmp[(vsize + j11)];
			}
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec13_perm[j13] = fRec13_tmp[(vsize + j13)];
			}
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec11_perm[j15] = fRec11_tmp[(vsize + j15)];
			}
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec0_perm[j17] = fRec0_tmp[(vsize + j17)];
			}
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec1_perm[j25] = fRec1_tmp[(vsize + j25)];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT((fRec0[i] + fRec1[i]));
			}
		}
		/* Remaining frames */
		if ((vindex < count)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = (count - vindex);
			/* Recursive loop 0 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec10_tmp[j0] = fRec10_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec10[i] = ((0.000500000024f * fRec10[(i - 2)]) + (0.999499977f * float(input0[i])));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec10_perm[j1] = fRec10_tmp[(vsize + j1)];
			}
			/* Recursive loop 1 */
			/* Pre code */
			fYec0_idx = ((fYec0_idx + fYec0_idx_save) & 255);
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec8_tmp[j2] = fRec8_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec0[((i + fYec0_idx) & 255)] = (fRec10[i] - (0.75f * fRec8[(i - 1)]));
				fRec8[i] = fYec0[(((i + fYec0_idx) - 142) & 255)];
				fRec9[i] = (0.75f * fYec0[((i + fYec0_idx) & 255)]);
			}
			/* Post code */
			fYec0_idx_save = vsize;
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec8_perm[j3] = fRec8_tmp[(vsize + j3)];
			}
			/* Recursive loop 2 */
			/* Pre code */
			fYec1_idx = ((fYec1_idx + fYec1_idx_save) & 127);
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec6_tmp[j4] = fRec6_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[((i + fYec1_idx) & 127)] = ((fRec9[i] + fRec8[(i - 1)]) - (0.75f * fRec6[(i - 1)]));
				fRec6[i] = fYec1[(((i + fYec1_idx) - 107) & 127)];
				fRec7[i] = (0.75f * fYec1[((i + fYec1_idx) & 127)]);
			}
			/* Post code */
			fYec1_idx_save = vsize;
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec6_perm[j5] = fRec6_tmp[(vsize + j5)];
			}
			/* Recursive loop 3 */
			/* Pre code */
			fYec2_idx = ((fYec2_idx + fYec2_idx_save) & 511);
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec2[((i + fYec2_idx) & 511)] = ((fRec7[i] + fRec6[(i - 1)]) - (0.625f * fRec4[(i - 1)]));
				fRec4[i] = fYec2[(((i + fYec2_idx) - 379) & 511)];
				fRec5[i] = (0.625f * fYec2[((i + fYec2_idx) & 511)]);
			}
			/* Post code */
			fYec2_idx_save = vsize;
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec4_perm[j7] = fRec4_tmp[(vsize + j7)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 511);
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec2_tmp[j8] = fRec2_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec3[((i + fYec3_idx) & 511)] = ((fRec5[i] + fRec4[(i - 1)]) - (0.625f * fRec2[(i - 1)]));
				fRec2[i] = fYec3[(((i + fYec3_idx) - 277) & 511)];
				fRec3[i] = (0.625f * fYec3[((i + fYec3_idx) & 511)]);
			}
			/* Post code */
			fYec3_idx_save = vsize;
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec2_perm[j9] = fRec2_tmp[(vsize + j9)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 1023);
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec14_tmp[j10] = fRec14_perm[j10];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 8191);
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec13_tmp[j12] = fRec13_perm[j12];
			}
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 4095);
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec11_tmp[j14] = fRec11_perm[j14];
			}
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 4095);
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec0_tmp[j16] = fRec0_perm[j16];
			}
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 1023);
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec19_tmp[j18] = fRec19_perm[j18];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 8191);
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec18_tmp[j20] = fRec18_perm[j20];
			}
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 2047);
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec16_tmp[j22] = fRec16_perm[j22];
			}
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 2047);
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec1_tmp[j24] = fRec1_perm[j24];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec4[((i + fYec4_idx) & 1023)] = ((0.699999988f * fRec14[(i - 1)]) + fRec1[(i - 1)]);
				fRec14[i] = fYec4[(((i + fYec4_idx) - 908) & 1023)];
				fRec15[i] = (0.0f - (0.699999988f * fYec4[((i + fYec4_idx) & 1023)]));
				fYec5[((i + fYec5_idx) & 8191)] = (fRec15[i] + fRec14[(i - 1)]);
				fRec13[i] = ((0.000500000024f * fRec13[(i - 1)]) + (0.999499977f * fYec5[(((i + fYec5_idx) - 4217) & 8191)]));
				fYec6[((i + fYec6_idx) & 4095)] = (fRec13[i] - fRec11[(i - 1)]);
				fRec11[i] = (0.5f * fYec6[(((i + fYec6_idx) - 2656) & 4095)]);
				fRec12[i] = (0.25f * fYec6[((i + fYec6_idx) & 4095)]);
				fYec7[((i + fYec7_idx) & 4095)] = (fRec12[i] + fRec11[(i - 1)]);
				fRec0[i] = (fRec2[(i - 1)] + (fRec3[i] + (0.5f * fYec7[(((i + fYec7_idx) - 2656) & 4095)])));
				fYec8[((i + fYec8_idx) & 1023)] = ((0.699999988f * fRec19[(i - 1)]) + fRec0[(i - 1)]);
				fRec19[i] = fYec8[(((i + fYec8_idx) - 672) & 1023)];
				fRec20[i] = (0.0f - (0.699999988f * fYec8[((i + fYec8_idx) & 1023)]));
				fYec9[((i + fYec9_idx) & 8191)] = (fRec20[i] + fRec19[(i - 1)]);
				fRec18[i] = ((0.000500000024f * fRec18[(i - 1)]) + (0.999499977f * fYec9[(((i + fYec9_idx) - 4453) & 8191)]));
				fYec10[((i + fYec10_idx) & 2047)] = (fRec18[i] - fRec16[(i - 1)]);
				fRec16[i] = (0.5f * fYec10[(((i + fYec10_idx) - 1800) & 2047)]);
				fRec17[i] = (0.25f * fYec10[((i + fYec10_idx) & 2047)]);
				fYec11[((i + fYec11_idx) & 2047)] = (fRec17[i] + fRec16[(i - 1)]);
				fRec1[i] = ((fRec3[i] + fRec2[(i - 1)]) + (0.5f * fYec11[(((i + fYec11_idx) - 1800) & 2047)]));
			}
			/* Post code */
			fYec11_idx_save = vsize;
			fYec10_idx_save = vsize;
			fYec9_idx_save = vsize;
			fYec8_idx_save = vsize;
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec19_perm[j19] = fRec19_tmp[(vsize + j19)];
			}
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec18_perm[j21] = fRec18_tmp[(vsize + j21)];
			}
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec16_perm[j23] = fRec16_tmp[(vsize + j23)];
			}
			fYec7_idx_save = vsize;
			fYec6_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec14_perm[j11] = fRec14_tmp[(vsize + j11)];
			}
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec13_perm[j13] = fRec13_tmp[(vsize + j13)];
			}
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec11_perm[j15] = fRec11_tmp[(vsize + j15)];
			}
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec0_perm[j17] = fRec0_tmp[(vsize + j17)];
			}
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec1_perm[j25] = fRec1_tmp[(vsize + j25)];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT((fRec0[i] + fRec1[i]));
			}
		}
	}

};
#endif

class Saw : public od::Object
{
  public:
    Saw();
    ~Saw();

#ifndef SWIGLUA
    virtual void process();
    od::Inlet mIn{"In"};
    od::Outlet mOut{"Out"};

  private:
    mydsp DSP;

#endif

};


#endif
