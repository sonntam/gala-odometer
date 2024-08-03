#include "display_util.h"
#include "peripherals.h"


// Function to calculate distance in kilometers
void displayFloat(LiquidCrystal &lcd, float distance, int roundToDigits, int col, int row, float &lastDispVal, size_t &lastWrittenCharCount, const char* unit, bool rightAligned = false) {
    char buffer[DISP_NUMCOLS];
    char szDistance[DISP_NUMCOLS];

    size_t writtenChars = 0;

    // Round to <roundToDigits> digits
    float digitsMultiplier = 1.0f;
    for( int i = 0; i < roundToDigits; i++ ) {
      digitsMultiplier *= 10.0f;
    }

    // Round to the nearest meter (3 decimal places)
    float roundedDistance = static_cast<uint32_t>(distance * digitsMultiplier + 0.5) / digitsMultiplier;

    // Update the screen only if the distance has changed
    if (roundedDistance != lastDispVal) {
        lcd.setCursor(col, row);
        
        dtostrf(roundedDistance, 0, roundToDigits, szDistance);
        writtenChars = sprintf(buffer,"%s%s", szDistance, unit);


        if( rightAligned ) {
          lcd.rightToLeft();
          reverseString(buffer);
        }

        lcd.print(buffer);

        if( lastWrittenCharCount > writtenChars )
        {
          // Clear the remaining chars on screen
          for( int i = writtenChars; i < lastWrittenCharCount; i++ )
            lcd.print(' ');
        }

        lcd.leftToRight();

        lastWrittenCharCount = writtenChars;
        lastDispVal = roundedDistance; // Update last displayed distance
    }
}

void reverseString(char* str) {

    // Initialize start and end pointers
    int start = 0;
    int end = strlen(str) - 1;
    char temp;

    // Swap characters while start is less than end
    while (start < end) {
      
        // Swap characters
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move pointers
        start++;
        end--;
    }
}
