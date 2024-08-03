#pragma once

#include <Arduino.h>

namespace Controller {

    enum command {
      NONE = 0,
      GOTO_SETUP,
      LEAVE_SETUP,
      ACTIVATE_CIRCUMFERENCE,
      ACTIVATE_PULSESPERROUND
    };

    void setup_bootscreen();
    enum command loop_bootscreen();

    void setup_setupscreen();
    void exit_setupscreen();
    
    void setup_setupPulsesPerRound();
    enum command loop_setupPulsesPerRound();

    void setup_setupCircumference();
    enum command loop_setupCircumference();

    void setup_mainmenu();
    void loop_mainmenu();

    float pulseToMillimeters(uint32_t pulseCount);
    float pulseToMeters(uint32_t pulseCount);
    float pulseToKilometers(uint32_t pulseCount);

    extern uint32_t pulsesAbs, pulsesRel;
    extern float speed; // Current speed in km/h

    extern volatile uint16_t PULSES_PER_ROUND;
    extern volatile uint16_t MILLIMETERS_PER_ROUND;
}
