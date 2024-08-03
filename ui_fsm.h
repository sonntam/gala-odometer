#pragma once

namespace ui_fsm {
  enum UI_FSM_STATE {
    UNKNOWN = 0,
    BOOT_SCREEN,
    MAIN_MENU,
    SET_PULSES_PER_ROUND,
    SET_WHEEL_CIRCUMFERENCE,
    SHUTDOWN_SAVE,
    SHUTDOWN
  };

  void setup();
  void loop();
}
