#ifndef STATE_H
#define STATE_H

// UI State enum
enum UIState {
  IDLE,
  COUNTING,
  RUNNING,
  SETTINGS,
  COMPLETE
};

// Driving state enum
enum DriveState {
  STOPPED,
  TURNING,
  DRIVING
};

#endif // STATE_H 