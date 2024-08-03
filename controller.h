#pragma once

#include <Arduino.h>

namespace Controller {
    void setup_bootscreen();

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
