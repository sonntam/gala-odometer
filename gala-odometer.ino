#include "peripherals.h"
#include "ui_fsm.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Peripherals::setup();
  ui_fsm::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  Peripherals::loop();
  ui_fsm::loop();
}
