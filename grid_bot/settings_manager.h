#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <Arduino.h>

// Drive speed enum
enum DriveSpeed {
  SPEED_SLOW,
  SPEED_STANDARD,
  SPEED_FAST
};

// Drive distance enum
enum DriveDistance {
  DISTANCE_COMPACT,
  DISTANCE_STANDARD,
  DISTANCE_EXTENDED
};

// Setting option enum
enum SettingOption { BRIGHTNESS, DRIVE_SPEED, DRIVE_DISTANCE };

// Struct to map setting options to their labels
struct SettingInfo {
  SettingOption option;
  const char* label;
};

// Settings option labels
extern const SettingInfo SETTINGS_INFO[];
extern const char* DRIVE_SPEED_LABELS[];
extern const char* DRIVE_DISTANCE_LABELS[];

class SettingsManager {
private:
  int displayBrightness;
  DriveSpeed driveSpeed;
  DriveDistance driveDistance;
  
public:
  // Constructor
  SettingsManager();
  
  // Brightness methods
  int getDisplayBrightness() const;
  void setDisplayBrightness(int brightness);
  void adjustBrightness(int delta);
  
  // Drive speed methods
  DriveSpeed getDriveSpeed() const;
  void setDriveSpeed(DriveSpeed speed);
  void increaseDriveSpeed();
  void decreaseDriveSpeed();
  const char* getDriveSpeedLabel() const;
  
  // Drive distance methods
  DriveDistance getDriveDistance() const;
  void setDriveDistance(DriveDistance distance);
  void increaseDriveDistance();
  void decreaseDriveDistance();
  const char* getDriveDistanceLabel() const;
  
  // Settings labels
  static const String* getSettingsLabels();
  static int getSettingsLabelsCount();
  
  // Helper functions for robust mapping
  static const char* getSettingLabel(SettingOption option);
  static int getSettingIndex(SettingOption option);
  
  // Utility methods
  void resetToDefaults();
  
  // New method to adjust settings
  void adjustSetting(SettingOption option, int direction);
};

#endif // SETTINGS_MANAGER_H 