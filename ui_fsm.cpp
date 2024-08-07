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
            enum Controller::command cmd;

            AFTER_MS(3000) {
              nextState = MAIN_MENU;
            }

            cmd = Controller::loop_bootscreen();
            if( cmd == Controller::GOTO_SETUP ) {
              nextState = SETUP_SCREEN;
            }
          }
          break;

        case SETUP_SCREEN:

          ON_ENTRY {
            Controller::setup_setupscreen();
          }

          DURING {
            nextState = SET_PULSES_PER_ROUND;
          }

          break;

        case SET_PULSES_PER_ROUND:

          ON_ENTRY {
            Controller::setup_setupPulsesPerRound();
          }

          DURING {
            
            enum Controller::command cmd;

            cmd = Controller::loop_setupPulsesPerRound();
            switch( cmd ) {
              case  Controller::LEAVE_SETUP:
                nextState = MAIN_MENU;
                break;
              case Controller::ACTIVATE_CIRCUMFERENCE:
                nextState = SET_WHEEL_CIRCUMFERENCE;
                break;
            }
          }

          ON_EXIT {
            Controller::exit_setupscreen();
          }

          break;
        
        case SET_WHEEL_CIRCUMFERENCE:

          ON_ENTRY {
            Controller::setup_setupCircumference();
          }

          DURING {
            
            enum Controller::command cmd;

            cmd = Controller::loop_setupCircumference();
            switch( cmd ) {
              case  Controller::LEAVE_SETUP:
                nextState = MAIN_MENU;
                break;
              case Controller::ACTIVATE_PULSESPERROUND:
                nextState = SET_PULSES_PER_ROUND;
                break;
            }
          }

          ON_EXIT {
            Controller::exit_setupscreen();
          }

          break;

        case MAIN_MENU:
          ON_ENTRY {
            Controller::setup_mainmenu();
          }
  
          DURING {
            enum Controller::command cmd;
            
            cmd = Controller::loop_mainmenu();

            switch( cmd ) {
              case Controller::POWER_OFF:
                nextState = SHUTDOWN_SAVE;
                break;
            }
            
          }
          break;

        case SHUTDOWN_SAVE:
          ON_ENTRY {
            Controller::setup_poweroff_menu();
          }

          DURING {
            enum Controller::command cmd;
            
            cmd = Controller::loop_poweroff_menu();

            switch( cmd ) {
              case Controller::RESET:
                nextState = BOOT_SCREEN;
                break;
            }
            
          }
          break;
      }    
    } while( nextState != currentState );
  }
  

}
