#include "peripherals.h"

namespace Peripherals {

  LiquidCrystal lcd(12, 11, 7, 6, 5, 4); // Pins for RS, E, D4, D5, D6, D7
  PolledEncoder enc1, enc2;

  Bounce2::Button powerSwitch = Bounce2::Button();

  void setup() {
    lcd.begin(DISP_NUMCOLS, DISP_NUMROWS); // Initialize a 16x2 display
    lcd.clear();

    enc1.begin(A0,A1,A2);
    enc2.begin(A3,A4,A5);

    powerSwitch.interval(5);
    powerSwitch.attach(8, INPUT);
    powerSwitch.setPressedState(LOW);
  }

  void loop() {
    powerSwitch.update();
    enc1.tick();
    enc2.tick();
  }

}
