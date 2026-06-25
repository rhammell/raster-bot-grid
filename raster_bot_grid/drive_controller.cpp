#include "drive_controller.h"

DriveController::DriveController(GridModel& model)
  : model(model), driveState(STOPPED), moveStartTime(0), lastReachedCell({ -1, -1 }) {}

void DriveController::start() {
  // Initialize path execution from the beginning
  model.setCurrentPathIndex(0);
  model.setCurrentDirection(UP);
  driveState = STOPPED;
}

void DriveController::stop() {
  driveState = STOPPED;
}

DriveState DriveController::getState() const {
  return driveState;
}

bool DriveController::isStopped() const {
  return driveState == STOPPED;
}

PathCell DriveController::getLastReachedCell() const {
  return lastReachedCell;
}

int DriveController::calculateTurn(Direction currentDir, Direction targetDir) {
  // Calculate difference between current and target direction
  int diff = static_cast<int>(targetDir) - static_cast<int>(currentDir);

  // Normalize to -1 (left) or 1 (right)
  if (diff == 3) diff = -1;
  if (diff == -3) diff = 1;

  return diff;
}

DriveEvent DriveController::update() {
  // Main movement state machine that handles bot navigation through the grid
  switch (driveState) {

    // Handle stopped state - determine next action (turn or drive)
    case STOPPED: {
      // Get the target direction from the current path point
      Direction targetDirection = model.getNextDirection();

      // Check if bot is already aligned with target direction
      if (model.getCurrentDirection() == targetDirection) {
        // Bot is aligned, transition to driving state
        driveState = DRIVING;
        // Here you would add actual motor control:
        // driveForward();
        Serial.println("Driving");
      }
      // Bot needs to turn to align with target direction
      else {
        // Transition to turning state
        driveState = TURNING;
        // Calculate which direction to turn (left or right)
        int turn = calculateTurn(model.getCurrentDirection(), targetDirection);
        // Here you would add actual motor control:
        // if (turn < 0) turnLeft();
        // if (turn > 0) turnRight();
        Serial.print("Turning ");
        Serial.println(turn < 0 ? "left" : "right");
      }
      // Record the start time for movement timing
      moveStartTime = millis();
      return DRIVE_NONE;
    }

    // Handle driving state - move forward for specified duration
    case DRIVING: {
      // Check if forward movement duration has elapsed
      if (millis() - moveStartTime >= FORWARD_MOVE_TIME) {
        // Remember the cell we just arrived at so the caller can redraw it
        lastReachedCell = model.getCurrentPathCell();

        // Check if we've reached the end of the path
        if (model.isPathComplete()) {
          // Path is complete, stop all movement
          // Here you would stop motors:
          // stopMotors();
          Serial.println("Path complete");
          driveState = STOPPED;
          return DRIVE_PATH_COMPLETE;
        }
        // Path is not complete, prepare for next movement
        else {
          // Reset timer for next movement segment
          moveStartTime = millis();
          // Get direction for next path point
          Direction nextDirection = model.getNextDirection();
          // Advance to next path point
          model.setCurrentPathIndex(model.getCurrentPathIndex() + 1);

          // Check if direction changes at next point
          if (nextDirection != model.getCurrentDirection()) {
            // Direction changes, stop for turn
            driveState = STOPPED;
            // Here you would stop motors:
            // stopMotors();
            Serial.println("Stopping for turn");
          }
          // Direction remains same, continue driving
          else {
            Serial.println("Continuing forward");
            // No need to modify motors, just keep driving
          }
          return DRIVE_CELL_REACHED;
        }
      }
      return DRIVE_NONE;
    }

    // Handle turning state - execute 90-degree turn
    case TURNING: {
      // Check if turn duration has elapsed
      if (millis() - moveStartTime >= TURN_MOVE_TIME) {
        // Get target direction for after turn
        Direction targetDirection = model.getNextDirection();
        // Update bot's current direction and transition to driving
        model.setCurrentDirection(targetDirection);
        driveState = DRIVING;
        moveStartTime = millis();
        // Here you would update motor control:
        // stopTurning();
        // driveForward();
        Serial.println("Turn complete, starting forward movement");
      }
      return DRIVE_NONE;
    }
  }

  return DRIVE_NONE;
}
