#pragma once
#define BLI_CROSSINGS 16  // Acutaly this is crossings on _each_ side of x=0
#define BLI_OVERSAMPLE 256

#define BLI_LEN (BLI_CROSSINGS * BLI_OVERSAMPLE * 2)


namespace ljw
{
  class Bli
  {
    public:
      static float step_corrections[BLI_LEN];
  };
}
