#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <Arduino.h>
#include "grid_model.h"

// Driving state for the movement state machine
enum DriveState {
  STOPPED,
  TURNING,
  DRIVING
};

// Result of a single DriveController update tick
enum DriveEvent {
  DRIVE_NONE,           // Nothing notable happened this tick
  DRIVE_CELL_REACHED,   // Finished driving into a cell (more path remains)
  DRIVE_PATH_COMPLETE   // Reached the final cell; controller is now stopped
};

// Drives the bot along the GridModel's path. Owns the movement state machine
// and timing, and is the home for real motor control. It knows nothing about
// the display or UI: callers react to the events returned from update().
class DriveController {
public:
  DriveController(GridModel& model);

  // Begin executing the model's current path from the start
  void start();

  // Stop all movement
  void stop();

  // Advance the movement state machine; call once per loop while running.
  // Returns an event describing what happened this tick.
  DriveEvent update();

  // State queries
  DriveState getState() const;
  bool isStopped() const;

  // The path cell most recently completed.
  // Valid after update() returns DRIVE_CELL_REACHED or DRIVE_PATH_COMPLETE.
  PathCell getLastReachedCell() const;

private:
  GridModel& model;
  DriveState driveState;
  unsigned long moveStartTime;
  PathCell lastReachedCell;

  // Movement timing
  static const unsigned long FORWARD_MOVE_TIME = 2000;  // Time to move forward one cell
  static const unsigned long TURN_MOVE_TIME = 2000;     // Time to execute a 90-degree turn

  // Determine turn direction (-1 left, +1 right) between two headings
  int calculateTurn(Direction currentDir, Direction targetDir);
};

#endif // DRIVE_CONTROLLER_H
