#include <iostream>
#include "blit.h"

using namespace ljw;
using namespace std;

void assertEquals(float a, float b, char* thing)
{
  if (!(a == 0 && b==0) && abs(a/b - 1) > 0.00001)
  {
    cout << "FAIL : " << thing << endl;
    cout << "     : expected " << a << endl;
    cout << "     : got      " << b << endl;
  }
  cout << " ✔ " << thing << endl;
}

void testSingleImpulse()
{
  char msg[100];
  cout << "== testSingleImpulse ==" << endl;

  float bli[] = {1.0f, 0.8f, 0.6f, 0.4f};
  Blit blit(bli, 4, 2);

  blit.addImpulse(0, 0.5);

  sprintf(msg, "blit.pulses_count");
  assertEquals(1, blit.pulses_count, msg);

  for (int i=0; i<2; i++)
  {
    sprintf(msg, "blit.next() at i=%i", i);
    assertEquals(0.5 - i*0.2, blit.next(), msg);
  }

  sprintf(msg, "blit.pulses_count");
  assertEquals(0, blit.pulses_count, msg);

  for (int i=0; i<2; i++)
  {
    sprintf(msg, "blit.next() at i=%i", i+2);
    assertEquals(0.0, blit.next(), msg);
  }
}

void testMultipleImpulse()
{
  char msg[100];
  cout << "== testMultipleImpulse ==" << endl;

  float bli[] = {1.0f, 0.8f, 0.6f, 0.4f};
  Blit blit(bli, 4, 2);

  blit.addImpulse(0, 1.0);

  sprintf(msg, "blit.pulses_count");
  assertEquals(1, blit.pulses_count, msg);

  sprintf(msg, "blit.next() [i=0]");
  assertEquals(1.0, blit.next(), msg);

  blit.addImpulse(0.5, 1.0);

  sprintf(msg, "blit.pulses_count");
  assertEquals(2, blit.pulses_count, msg);

  sprintf(msg, "blit.next() [i=2]");
  assertEquals(0.6 + 0.8, blit.next(), msg);

  sprintf(msg, "blit.pulses_count");
  assertEquals(1, blit.pulses_count, msg);

  sprintf(msg, "blit.next() [i=3]");
  assertEquals(0.4, blit.next(), msg);

  sprintf(msg, "blit.pulses_count");
  assertEquals(0, blit.pulses_count, msg);
}

int main()
{
  testSingleImpulse();
  testMultipleImpulse();
  return 0;
}
