#pragma once

#include <LiquidCrystal.h>
#include <UniversalTimer.h>
#include <EncoderTool.h>
#include <Bounce2.h>

using namespace EncoderTool;

#define DISP_NUMCOLS 20
#define DISP_NUMROWS 4

#define SPEED_CALC_INTERVAL_MS 500

namespace Peripherals {

  extern LiquidCrystal lcd;
  extern PolledEncoder enc1, enc2;  
  extern Bounce2::Button powerSwitch;

  void loop();
  void setup();
}
