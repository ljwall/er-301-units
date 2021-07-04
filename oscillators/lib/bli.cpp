#include "bli.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <hal/log.h>

namespace ljw
{
  float Bli::bli[BLI_LEN];
  float Bli::step_corrections[BLI_LEN];
  bool Bli::made = false;

  void Bli::make()
  {
    if (made)
      return;
    made = true;

    double sinc, x, blackman, t, step_size,
           TwoPi = 2.0*M_PI,
           FourPi = 4.0*M_PI;

    for (int i = 0; i<BLI_LEN; i++)
    {
      x = M_PI*(-(double)BLI_CROSSINGS + (double)i/((double)BLI_OVERSAMPLE));
      if (fabs(x) < 0.1)
      {
        sinc = 1.0 - pow(x, 2.0) /6.0 + pow(x, 4.0) / 120.0;
      }
      else
      {
        sinc = sin(x)/x;
      }
      t = (double)i / (double)BLI_LEN;
      blackman = 0.42 - 0.5*cos(TwoPi * t) + 0.08 * cos(FourPi * t);

      bli[i] = (float)(sinc * blackman);
    }

    // Calulate running sum
    for (int i = 0; i<BLI_LEN; i++)
    {
      step_corrections[i] = (i==0 ? 0.0 : step_corrections[i - 1]) + bli[i]/(double)BLI_OVERSAMPLE;
    }

    // record the step up
    step_size = step_corrections[BLI_LEN - 1];
    // normalise
    for (int i = 0; i<BLI_LEN; i++)
    {
      step_corrections[i] /= step_size;
    }

    // From the central point onward, subtract off the step, so were
    // left with (band limited step func)-(step func).
    for (int i = (BLI_LEN)/2; i< BLI_LEN; i++)
    {
      step_corrections[i] -= 1.0f;
    }
  }
}
