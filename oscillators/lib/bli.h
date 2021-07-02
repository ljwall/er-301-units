#pragma once
#define BLI_CROSSINGS 16  // Acutaly this is crossings on _each_ side of x=0
#define BLI_OVERSAMPLE 512

#define BLI_LEN (BLI_CROSSINGS * BLI_OVERSAMPLE * 2 + 1)


namespace ljw
{
  class Bli
  {
    public:
      static float bli[BLI_LEN];
      static void make();
  };
}
