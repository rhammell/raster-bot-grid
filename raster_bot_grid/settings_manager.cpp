#include "settings_manager.h"

// Settings option mapping
const SettingInfo SETTINGS_INFO[] = { 
  { BRIGHTNESS, "Brightness" },
  { DRIVE_SPEED, "Drive Speed" },
  { DRIVE_DISTANCE, "Drive Distance" }
};
const char* DRIVE_SPEED_LABELS[] = { "Slow", "Standard", "Fast" };
const char* DRIVE_DISTANCE_LABELS[] = { "Compact", "Standard", "Extended" };

// Constructor
SettingsManager::SettingsManager() {
  // Initialize with default values
  displayBrightness = 60;
  driveSpeed = SPEED_STANDARD;
  driveDistance = DISTANCE_STANDARD;
}

// Brightness methods
int SettingsManager::getDisplayBrightness() const {
  return displayBrightness;
}

void SettingsManager::setDisplayBrightness(int brightness) {
  displayBrightness = constrain(brightness, 10, 100);
}

void SettingsManager::adjustBrightness(int delta) {
  setDisplayBrightness(displayBrightness + delta);
}

// Drive speed methods
DriveSpeed SettingsManager::getDriveSpeed() const {
  return driveSpeed;
}

void SettingsManager::setDriveSpeed(DriveSpeed speed) {
  driveSpeed = speed;
}

void SettingsManager::increaseDriveSpeed() {
  if (driveSpeed < SPEED_FAST) {
    driveSpeed = (DriveSpeed)(driveSpeed + 1);
  }
}

void SettingsManager::decreaseDriveSpeed() {
  if (driveSpeed > SPEED_SLOW) {
    driveSpeed = (DriveSpeed)(driveSpeed - 1);
  }
}

const char* SettingsManager::getDriveSpeedLabel() const {
  return DRIVE_SPEED_LABELS[driveSpeed];
}

// Drive distance methods
DriveDistance SettingsManager::getDriveDistance() const {
  return driveDistance;
}

void SettingsManager::setDriveDistance(DriveDistance distance) {
  driveDistance = distance;
}

void SettingsManager::increaseDriveDistance() {
  if (driveDistance < DISTANCE_EXTENDED) {
    driveDistance = (DriveDistance)(driveDistance + 1);
  }
}

void SettingsManager::decreaseDriveDistance() {
  if (driveDistance > DISTANCE_COMPACT) {
    driveDistance = (DriveDistance)(driveDistance - 1);
  }
}

const char* SettingsManager::getDriveDistanceLabel() const {
  return DRIVE_DISTANCE_LABELS[driveDistance];
}

// Settings labels
const String* SettingsManager::getSettingsLabels() {
  static String labels[3];
  for (int i = 0; i < 3; i++) {
    labels[i] = String(SETTINGS_INFO[i].label);
  }
  return labels;
}

int SettingsManager::getSettingsLabelsCount() {
  return sizeof(SETTINGS_INFO) / sizeof(SETTINGS_INFO[0]);
}

// Helper functions for robust mapping
const char* SettingsManager::getSettingLabel(SettingOption option) {
  for (int i = 0; i < getSettingsLabelsCount(); i++) {
    if (SETTINGS_INFO[i].option == option) {
      return SETTINGS_INFO[i].label;
    }
  }
  return "Unknown"; // Fallback
}

int SettingsManager::getSettingIndex(SettingOption option) {
  for (int i = 0; i < getSettingsLabelsCount(); i++) {
    if (SETTINGS_INFO[i].option == option) {
      return i;
    }
  }
  return -1; // Not found
}

// Utility methods
void SettingsManager::resetToDefaults() {
  displayBrightness = 60;
  driveSpeed = SPEED_STANDARD;
  driveDistance = DISTANCE_STANDARD;
}

void SettingsManager::adjustSetting(SettingOption option, int direction) {
    switch (option) {
        case BRIGHTNESS:
            adjustBrightness(10 * direction);
            break;
        case DRIVE_SPEED:
            if (direction < 0) decreaseDriveSpeed();
            else increaseDriveSpeed();
            break;
        case DRIVE_DISTANCE:
            if (direction < 0) decreaseDriveDistance();
            else increaseDriveDistance();
            break;
    }
} 