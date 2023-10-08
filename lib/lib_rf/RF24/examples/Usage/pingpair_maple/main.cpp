#ifdef MAPLE_IDE

#include <stdio.h>
#include "wirish.h"

extern void setup(void);
extern void loop(void);

void board_start(const char* program_name)
{
  // Set up the LED to steady on
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);

  // Setup the button as input
  pinMode(BOARD_BUTTON_PIN, INPUT);
  digitalWrite(BOARD_BUTTON_PIN, HIGH);

  SerialUSB.begin();
  SerialUSB.println("Press BUT");

  // Wait for button press
  while ( !isButtonPressed() )
  {
  }

  SerialUSB.println("Welcome!");
  SerialUSB.println(program_name);

  int i = 11;
  while (i--)
  {
    toggleLED();
    delay(50);
  }
}

/**
 * Custom version of _write, which will print to the USB.
 * In order to use it you MUST ADD __attribute__((weak))
 * to _write in libmaple/syscalls.c
*/
extern "C" int _write (int file, char * ptr, int len)
{
  if ( (file != 1) && (file != 2) )
    return 0; 
  else
    SerialUSB.write(ptr,len);
  return len;
}

/**
 * Re-entrant version of _write.  Yagarto and Devkit now use
 * the re-entrant newlib, so these get called instead of the
 * non_r versions.
 */
extern "C" int _write_r (void*, int file, char * ptr, int len)
{
  return _write( file, ptr, len);
}

__attribute__((constructor)) __attribute__ ((weak)) void premain()
{
  init();
}

__attribute__((weak)) void setup(void)
{
  board_start("No program defined");
}

__attribute__((weak)) void loop(void)
{
}

__attribute__((weak)) int main(void)
{
  setup();

  while (true)
  {
    loop();
  }
  return 0;
}
#endif // ifdef MAPLE_IDE
// vim:cin:ai:sts=2 sw=2 ft=cpp
