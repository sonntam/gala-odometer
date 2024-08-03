#include "peripherals.h"
#include "ui_fsm.h"
#include "controller.h"

#define ON_ENTRY if( previousState != currentState )
#define ON_EXIT if( nextState != currentState )
#define DURING
#define AFTER_MS(X) if( stateTimer.getTimerValue() >= (X) )

namespace ui_fsm {
  
  static UniversalTimer stateTimer(0,false);
  
  static enum UI_FSM_STATE currentState = UNKNOWN;
  
  void setup()
  {
    Serial.println("ui_fsm::setup()");
    currentState = UNKNOWN;

    stateTimer.start();
  }
  
  void loop() 
  {
    enum UI_FSM_STATE previousState = currentState;
    enum UI_FSM_STATE nextState = currentState;
  
    do
    {
      if( nextState != currentState ) {
        Serial.print("ui_fsm::loop() change state ");
        Serial.print(currentState);
        Serial.print(" -> ");
        Serial.println(nextState);
        previousState = currentState;
        currentState = nextState;
        
        // Restart timer
        stateTimer.stop();
        stateTimer.resetTimerValue();
        stateTimer.start();
      }
  
      switch( currentState ) {
        case UNKNOWN:

          DURING {
            nextState = BOOT_SCREEN;
          }

          break;
        case BOOT_SCREEN:
  
          ON_ENTRY {
            Controller::setup_bootscreen();
          }
  
          DURING {
            AFTER_MS(3000) {
              nextState = MAIN_MENU;
            }
          }
          break;
  
        case MAIN_MENU:
          ON_ENTRY {
            Controller::setup_mainmenu();
          }
  
          DURING {
  
            Controller::loop_mainmenu();
          }
          break;
      }    
    } while( nextState != currentState );
  }
  

}
