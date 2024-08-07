#include "peripherals.h"
#include "controller.h"

// External Connections
// --------------------
/*
 * LCD: RS - 12 gn
 *      E  - 11 bl
 *      D4 -  7 pk 
 *      D5 -  6 gy
 *      D6 -  5 wh
 *      D7 -  4 bk
 * POWER SWITCH: 8 pk
 * ENC1: A  - A0 bn
 *       B  - A1 rd
 *       SW - A2 or
 * ENC2: A  - A3 bn
 *       B  - A4 rd
 *       SW - A5 or
 * GALA:  2 gy
 * LCD contrast: 9 ye
 */

namespace Peripherals {

  LiquidCrystal lcd(12, 11, 7, 6, 5, 4); // Pins for RS, E, D4, D5, D6, D7
  PolledEncoder enc1, enc2;

  Bounce2::Button powerSwitch = Bounce2::Button();
  
  static uint8_t contrast = 75; // useful range up to 100, down to 40

  static void galaPulse();

  void setup() {

    // Set contrast (hard-coded)
    pinMode(9, OUTPUT);
    analogWrite(9, contrast);
  
    // Wait for contrast cap to charge
    delay(100);
    
    lcd.begin(DISP_NUMCOLS, DISP_NUMROWS); // Initialize a 16x2 display
    lcd.clear();

    enc1.begin(A0,A1,A2);
    enc2.begin(A3,A4,A5);

    powerSwitch.interval(9);
    powerSwitch.attach(8, INPUT);
    powerSwitch.setPressedState(LOW);

    // Setup the interrupt pin to detect the GALA signal
    pinMode(2, INPUT_PULLUP); // GALA is push-pull so we don't need the internal pull-up
    attachInterrupt(digitalPinToInterrupt(2), galaPulse, RISING);
    
  }

  void loop() {
    powerSwitch.update();
    enc1.tick();
    enc2.tick();
  }

  void galaPulse() {
    Controller::pulsesAbs++;
    Controller::pulsesRel++;
  }

}
