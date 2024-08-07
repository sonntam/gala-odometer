#pragma once

#include <LiquidCrystal.h>

extern volatile uint32_t pulsesAbs, pulsesRel;
extern volatile uint16_t PULSES_PER_ROUND;
extern volatile uint16_t MILLIMETERS_PER_ROUND;

void displayFloat(LiquidCrystal &lcd, float distance, int roundToDigits, int col, int row, float &lastDispVal, size_t &lastWrittenCharCount, const char* unit, bool rightAligned = false);

size_t numberLength(uint32_t number);

void reverseString(char* str);
