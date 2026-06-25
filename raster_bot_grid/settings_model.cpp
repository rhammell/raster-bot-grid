#include "settings_model.h"

// Settings option mapping
const SettingInfo SETTINGS_INFO[] = { 
  { BRIGHTNESS, "Brightness" },
  { DRIVE_SPEED, "Drive Speed" },
  { DRIVE_DISTANCE, "Drive Distance" }
};
const char* DRIVE_SPEED_LABELS[] = { "Slow", "Standard", "Fast" };
const char* DRIVE_DISTANCE_LABELS[] = { "Compact", "Standard", "Extended" };

// Constructor
SettingsModel::SettingsModel() {
  // Initialize with default values
  displayBrightness = 60;
  driveSpeed = SPEED_STANDARD;
  driveDistance = DISTANCE_STANDARD;
}

// Brightness methods
int SettingsModel::getDisplayBrightness() const {
  return displayBrightness;
}

void SettingsModel::setDisplayBrightness(int brightness) {
  displayBrightness = constrain(brightness, 10, 100);
}

void SettingsModel::adjustBrightness(int delta) {
  setDisplayBrightness(displayBrightness + delta);
}

// Drive speed methods
DriveSpeed SettingsModel::getDriveSpeed() const {
  return driveSpeed;
}

void SettingsModel::setDriveSpeed(DriveSpeed speed) {
  driveSpeed = speed;
}

void SettingsModel::increaseDriveSpeed() {
  if (driveSpeed < SPEED_FAST) {
    driveSpeed = (DriveSpeed)(driveSpeed + 1);
  }
}

void SettingsModel::decreaseDriveSpeed() {
  if (driveSpeed > SPEED_SLOW) {
    driveSpeed = (DriveSpeed)(driveSpeed - 1);
  }
}

const char* SettingsModel::getDriveSpeedLabel() const {
  return DRIVE_SPEED_LABELS[driveSpeed];
}

// Drive distance methods
DriveDistance SettingsModel::getDriveDistance() const {
  return driveDistance;
}

void SettingsModel::setDriveDistance(DriveDistance distance) {
  driveDistance = distance;
}

void SettingsModel::increaseDriveDistance() {
  if (driveDistance < DISTANCE_EXTENDED) {
    driveDistance = (DriveDistance)(driveDistance + 1);
  }
}

void SettingsModel::decreaseDriveDistance() {
  if (driveDistance > DISTANCE_COMPACT) {
    driveDistance = (DriveDistance)(driveDistance - 1);
  }
}

const char* SettingsModel::getDriveDistanceLabel() const {
  return DRIVE_DISTANCE_LABELS[driveDistance];
}

// Settings labels
const String* SettingsModel::getSettingsLabels() {
  static String labels[3];
  for (int i = 0; i < 3; i++) {
    labels[i] = String(SETTINGS_INFO[i].label);
  }
  return labels;
}

int SettingsModel::getSettingsLabelsCount() {
  return sizeof(SETTINGS_INFO) / sizeof(SETTINGS_INFO[0]);
}

void SettingsModel::adjustSetting(SettingOption option, int direction) {
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
