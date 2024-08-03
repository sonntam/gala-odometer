#include "controller.h"
#include "peripherals.h"
#include "display_util.h"
#include <UniversalTimer.h>

using namespace Peripherals;

namespace Controller {

  UniversalTimer absResetTimer(2000, false);
  UniversalTimer relResetTimer(250, false);
  UniversalTimer displayUpdateTimer(1000, true);
  
  UniversalTimer enc1ValResetTimer(2000, false);
  UniversalTimer enc2ValResetTimer(2000, false);
  
  UniversalTimer speedCalcTimer(SPEED_CALC_INTERVAL_MS, true);

  static bool HandleEnc1();
  static bool HandleEnc2();
  static void CalculateSpeed();
  static void UpdateDisplay();

  // For determining the directional change of encoder
  static int enc1_lastval = 0, enc2_lastval = 0;

  // For calculating speed
  static uint32_t lastPulsesAbs = 0;

  static float lastOdoAbsDisp = -1.0; // Initialize with an invalid value
  static size_t lastOdoAbsStringSz = 0;
  
  static float lastOdoRelDisp = -1.0; // Initialize with an invalid value
  static size_t lastOdoRelStringSz = 0;
  
  static float lastSpeedDisp = 0.0;
  static size_t lastSpeedStringSz = 0;
  
  static float lastSpeedMKSDisp = 0.0;
  static size_t lastSpeedMKSStringSz = 0;

  // The exported pulse counters
  uint32_t pulsesAbs = 0, pulsesRel = 0;
  float speed = 0.0f;

  volatile uint16_t PULSES_PER_ROUND = 45;
  volatile uint16_t MILLIMETERS_PER_ROUND = 2070;

  void setup_bootscreen() {
    Serial.println("Controller::setup_bootscreen()");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("RALLYE ODOMETER");
    lcd.setCursor(4,2);
    lcd.print("by Autojuwel");
    lcd.setCursor(0,3);
    lcd.print("V1.0");
  }

  void setup_mainmenu() {
    Serial.println("Controller::setup_mainmenu()");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ABSOLUT");

    lcd.setCursor(0,2);
    lcd.print("Vx");

    lcd.setCursor(DISP_NUMCOLS-1,0);
    lcd.rightToLeft();
    lcd.print("VITALER");
    lcd.leftToRight();
    
    displayUpdateTimer.start();
    speedCalcTimer.start();  
  }
  
  void loop_mainmenu() {

    bool hasEvent = false;
    
    hasEvent |= HandleEnc1();
    hasEvent |= HandleEnc2();

    CalculateSpeed();

    if( hasEvent || displayUpdateTimer.check() )
      UpdateDisplay();

    // TODO Take this from interrupts
    pulsesAbs++;
    pulsesRel++;
  }
  

  static bool HandleEnc1() {
    bool hasEvent = false;   
  
    if (enc1.valueChanged())             // do we have a new encoder value?
    {
        int newval = enc1.getValue();
        Serial.print("enc1 new value: ");
        Serial.println(newval);
  
        enc1ValResetTimer.start();
        enc1ValResetTimer.resetTimerValue();
  
        if( newval > enc1_lastval )
        {
          noInterrupts();
          // Add 100 meters
          pulsesAbs += ((uint32_t)100000 * (uint32_t)PULSES_PER_ROUND) / MILLIMETERS_PER_ROUND ;
          interrupts();
        }
        else
        {
          noInterrupts();
          // Sub 100 meters
          pulsesAbs -= ((uint32_t)100000 * (uint32_t)PULSES_PER_ROUND) / MILLIMETERS_PER_ROUND ;
          interrupts();
        }
  
        enc1_lastval = newval;
  
        hasEvent = true;
    }
    if (enc1.buttonChanged())            // do we have a new button state?
    {
        Serial.print("button1 state: ");
        Serial.println(enc1.getButton() == LOW ? "pressed" : "released");
  
        if( enc1.getButton() == LOW ) {
          absResetTimer.start();
        } else {
          absResetTimer.stop();
          absResetTimer.resetTimerValue();
        }
    }
  
    if( enc1ValResetTimer.check() ) {
      enc1_lastval = 0;
      enc1.setValue(0);
    }

    if( absResetTimer.check() ) {
      noInterrupts();
      pulsesAbs = 0;
      interrupts();
      hasEvent = true;
    }

    return hasEvent;
  }
  
  static bool HandleEnc2() {

    bool hasEvent = false;   
    
    if (enc2.valueChanged())             // do we have a new encoder value?
    {
        int newval = enc2.getValue();
        Serial.print("enc2 new value: ");
        Serial.println(newval);
  
        enc2ValResetTimer.start();
        enc2ValResetTimer.resetTimerValue();
  
        if( newval > enc2_lastval )
        {
          noInterrupts();
          // Add 10 meters
          pulsesRel += ((uint32_t)10000 * (uint32_t)PULSES_PER_ROUND) / MILLIMETERS_PER_ROUND ;
          interrupts();
        }
        else
        {
          noInterrupts();
          // Sub 10 meters
          pulsesRel -= ((uint32_t)10000 * (uint32_t)PULSES_PER_ROUND) / MILLIMETERS_PER_ROUND ;
          interrupts();
        }
  
        enc2_lastval = newval;
  
        hasEvent = true;
    }
    if (enc2.buttonChanged())            // do we have a new button state?
    {
        Serial.print("button2 state: ");
        Serial.println(enc2.getButton() == LOW ? "pressed" : "released");
  
        if( enc2.getButton() == LOW ) {
          relResetTimer.start();
        } else {
          relResetTimer.stop();
          relResetTimer.resetTimerValue();
        }
    }
  
    if( enc2ValResetTimer.check() ) {
      enc2_lastval = 0;
      enc2.setValue(0);
    }

    if( relResetTimer.check() ) {
      noInterrupts();
      pulsesRel = 0;
      interrupts();
      hasEvent = true;
    } 

    return hasEvent;
  }
  
  static void CalculateSpeed() {
     // Calculate speed
    if( speedCalcTimer.check() ) {
      speed = (float)(pulsesAbs - lastPulsesAbs)/(float)PULSES_PER_ROUND * (float)MILLIMETERS_PER_ROUND / (float)(SPEED_CALC_INTERVAL_MS)*3.6f;
      lastPulsesAbs = pulsesAbs;
    }
  }
  
  static void UpdateDisplay() {
      displayFloat(lcd, pulseToKilometers(pulsesAbs), 2, 0, 1, lastOdoAbsDisp, lastOdoAbsStringSz, " km", false);
      displayFloat(lcd, pulseToMeters(pulsesRel), 0, DISP_NUMCOLS-1, 1, lastOdoRelDisp, lastOdoRelStringSz, " m", true);
      displayFloat(lcd, speed, 1, 3, 3, lastSpeedDisp, lastSpeedStringSz, " km/h", false);
      displayFloat(lcd, speed/3.6f, 1, 3, 2, lastSpeedMKSDisp, lastSpeedMKSStringSz, " m/s", false);
    
  }

  float pulseToMillimeters(uint32_t pulseCount) {
    return (float)MILLIMETERS_PER_ROUND/(float)PULSES_PER_ROUND * pulseCount;
  }
  
  float pulseToMeters(uint32_t pulseCount) {
    return (float)MILLIMETERS_PER_ROUND/(float)PULSES_PER_ROUND/1000.0f * pulseCount;
  }
  
  float pulseToKilometers(uint32_t pulseCount) {
    return (float)MILLIMETERS_PER_ROUND/(float)PULSES_PER_ROUND/1000000.0f * pulseCount;
  }
}