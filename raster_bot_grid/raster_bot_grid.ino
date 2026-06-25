#include <Raster_Bot.h>
#include "icons.h"
#include "ui_elements.h"
#include "grid_model.h"
#include "settings_manager.h"
#include "drive_controller.h"

// UI state for the application's screen/mode state machine
enum UIState {
  IDLE,
  COUNTING,
  RUNNING,
  SETTINGS,
  COMPLETE
};

// Raster Bot instance (handles display and touch)
Raster_Bot bot;

// Screen dimensions
int screenWidth;
int screenHeight;

// Grid model instance
GridModel gridModel;

// Drive controller (operates on the grid model)
DriveController driveController(gridModel);

// Settings manager instance
SettingsManager settingsManager;

// UI elements
UIIconButton undoButton;
UITextButton startButton;
UIIconButton settingsButton;
UISettingsMenu settingsMenu;
UIGrid uiGrid;

// Current UI state
UIState uiState = IDLE;

// Default countdown state
unsigned long countdownStart = 0;
const int countdownDuration = 5000;

// Touch debounce delay
unsigned long lastTouchTime = 0;
const unsigned long touchDebounceDelay = 200;

// Forward declarations
void updateStartButton(int countdownNumber = -1);
void refreshStartButton(int countdownNumber = -1);

void setup() {
  Serial.begin(115200);

  // Initialize display and capacitive touchscreen
  if (!bot.display.begin()) {
    Serial.println("Display init failed");
    while (1);
  }

  // Set display orientation and brightness
  bot.display.setRotation(0);
  setBrightness();

  // Get screen dimensions
  screenWidth = bot.display.width();
  screenHeight = bot.display.height();

  // Initialize grid model
  initGridModel();

  // Layout and draw UI
  initUI();
  drawUI();
}

void initGridModel(){
  // Calculate grid size to fit in screen
  int availableHeight = screenHeight - BUTTON_HEIGHT - BUTTON_MARGIN - 1;
  int availableWidth = screenWidth - 1;
  int numRows = availableHeight / CELL_SIZE;
  int numCols = (availableWidth / CELL_SIZE) - ((availableWidth / CELL_SIZE) % 2 == 0 ? 1 : 0);
  
  // Initialize grid model
  gridModel.initGrid(numRows, numCols);
}

void initUI() {
  // Set UI grid size
  uiGrid.setSize(gridModel.getNumRows(), gridModel.getNumCols());

  // Set grid bounds
  int gridWidth = gridModel.getNumCols() * CELL_SIZE;
  int gridHeight = gridModel.getNumRows() * CELL_SIZE;
  int gridX = (screenWidth - (gridWidth + 1)) / 2;
  int totalGroupHeight = gridHeight + BUTTON_MARGIN + BUTTON_HEIGHT + 1;
  int gridY = (screenHeight - totalGroupHeight) / 2;
  uiGrid.setBounds(gridX, gridY, gridWidth, gridHeight);

  // Button row y position
  int y = gridY + gridHeight + BUTTON_MARGIN + 1;

  // Set undo button bounds and icon
  undoButton.setBounds(gridX, y, UNDO_BUTTON_WIDTH, BUTTON_HEIGHT);
  undoButton.setIcon(UNDO_ICON, 24, 24);

  // Set start button bounds and width
  int startButtonWidth = gridWidth - UNDO_BUTTON_WIDTH - BUTTON_MARGIN -
                      SETTINGS_BUTTON_WIDTH - BUTTON_MARGIN + 1;
  int startX = undoButton.x + undoButton.width + BUTTON_MARGIN;
  startButton.setBounds(startX, y, startButtonWidth, BUTTON_HEIGHT);
  
  // Update start button text and color
  updateStartButton();

  // Set settings button bounds and icon
  int settingsX = startButton.x + startButton.width + BUTTON_MARGIN;
  settingsButton.setBounds(settingsX, y, SETTINGS_BUTTON_WIDTH, BUTTON_HEIGHT);
  settingsButton.setIcon(SETTINGS_ICON, 24, 24);

  // Set settings menu options
  settingsMenu.setupOptions(SettingsManager::getSettingsLabels(), SettingsManager::getSettingsLabelsCount());
  settingsMenu.updateOptionValue(BRIGHTNESS, String(settingsManager.getDisplayBrightness()) + "%");
  settingsMenu.updateOptionValue(DRIVE_SPEED, settingsManager.getDriveSpeedLabel());
  settingsMenu.updateOptionValue(DRIVE_DISTANCE, settingsManager.getDriveDistanceLabel());

  // Set settings menu bounds
  int menuWidth = gridWidth * 0.85;
  int menuHeight = 225;
  int menuX = gridX + (gridWidth - menuWidth) / 2;
  int menuY = gridY + (gridHeight - menuHeight) / 2;
  settingsMenu.setPosition(menuX, menuY, menuWidth, menuHeight);

  // Set settings options positions
  settingsMenu.layout();
}

GridRenderMode currentGridRenderMode() {
  // Translate app state into a view-layer render mode
  switch (uiState) {
    case IDLE:
      return GRID_RENDER_EDIT;
    case RUNNING:
    case COMPLETE:
      return GRID_RENDER_PROGRESS;
    default:
      return GRID_RENDER_PLAIN;
  }
}

void drawUI() {
  // Draw all UI elements
  uiGrid.drawGridLines(bot.display);
  uiGrid.drawGridCells(bot.display, gridModel, currentGridRenderMode());
  undoButton.draw(bot.display);
  startButton.draw(bot.display);
  settingsButton.draw(bot.display);
}

void setBrightness() {
  // Get brightness percentage from settings manager
  int displayBrightness = settingsManager.getDisplayBrightness();

  // Convert percentage (0-100) to PWM value (0-255)
  int pwmOutput = map(displayBrightness, 0, 100, 0, 255);

  // Set display brightness
  bot.display.setBrightness(pwmOutput);
}

void updateStartButton(int countdownNumber) {
  // Button properties
  uint16_t currentColor;
  String buttonText;
  
  // Define properties based on current state
  switch (uiState) {
    case COUNTING:
      currentColor = BUTTON_COUNTING_COLOR;
      buttonText = String(countdownNumber);
      break;
    case RUNNING:
      currentColor = BUTTON_RUNNING_COLOR;
      buttonText = "Stop";
      break;
    case COMPLETE:
      currentColor = BUTTON_COMPLETE_COLOR;
      buttonText = "Done!";
      break;
    case IDLE:
    default:
      currentColor = BUTTON_IDLE_COLOR;
      buttonText = "Start";
      break;
  }
  
  // Set button text and color
  startButton.setLabel(buttonText);
  startButton.setBgColor(currentColor);
}

void refreshStartButton(int countdownNumber) {
  // Update the start button's label/color, then redraw it
  updateStartButton(countdownNumber);
  startButton.draw(bot.display);
}

void handleState() {
  // Handle countdown logic
  if (uiState == COUNTING) {
    handleCountdown();
  } 
  // Handle movement execution
  else if (uiState == RUNNING) {
    // Advance the drive controller and react to its events
    DriveEvent event = driveController.update();

    // Redraw the cell the bot just reached
    if (event == DRIVE_CELL_REACHED || event == DRIVE_PATH_COMPLETE) {
      PathCell cell = driveController.getLastReachedCell();
      uiGrid.drawGridCells(bot.display, gridModel, currentGridRenderMode(), cell.row, cell.row, cell.col, cell.col);
    }

    // Path finished: show completion state
    if (event == DRIVE_PATH_COMPLETE) {
      uiState = COMPLETE;
      refreshStartButton();
    }
  }
}

void handleCountdown() {
  // Track last displayed number to avoid unnecessary updates
  static int lastCountdownNumber = -1;

  // Calculate time since countdown started
  int countdownElapsed = millis() - countdownStart;

  // Check if countdown duration has elapsed
  if (countdownElapsed >= countdownDuration) {

    // Change state to running
    uiState = RUNNING;

    // Begin executing the path
    driveController.start();

    // Reset our static variable for the next countdown
    lastCountdownNumber = -1;

    // Update and draw start button
    refreshStartButton();
  } 
  // Counting still in progress
  else {
    // Calculate current number
    int countdownNumber = (countdownDuration / 1000) - (countdownElapsed / 1000); //(countdownDuration - countdownElapsed) / 1000;
    
    // Check if number has changed
    if (countdownNumber != lastCountdownNumber) {

      // Update button text and draw
      refreshStartButton(countdownNumber);

      // Update last displayed number
      lastCountdownNumber = countdownNumber;
    }
  }
}

void handleTouch(TS_Point p) {
  // End processing if touch occurred within debounce period
  unsigned long now = millis();
  if (now - lastTouchTime < touchDebounceDelay) {
    return;
  }

  // Update last touch time
  lastTouchTime = now;

  // getTouchPoint() already returns rotation-mapped screen coordinates
  int pixelX = p.x;
  int pixelY = p.y;

  // --- Touch Event Dispatching ---
  // 
  // Handle start button interactions outside of settings state
  if (startButton.contains(pixelX, pixelY) && uiState != SETTINGS) {
    onTouchStartButton();
  }
  // Handle undo button interactions in idle state
  else if (undoButton.contains(pixelX, pixelY) && uiState == IDLE) {
    onTouchUndoButton();
  }
  // Handle settings button interactions
  else if (settingsButton.contains(pixelX, pixelY)) {
    onTouchSettingsButton();  
  }
  // Handle grid interactions in idle state
  else if (uiGrid.contains(pixelX, pixelY) && uiState == IDLE) {
    onTouchGrid(pixelX, pixelY);
  }
  // Handle settings menu interactions in settings state
  else if (settingsMenu.contains(pixelX, pixelY) && uiState == SETTINGS) {
    onTouchSettingsMenu(pixelX, pixelY);
  }
}

void onTouchStartButton() {
  Serial.println("Start button toucheddd");

  // Handle start button touch in different states
  switch (uiState) {
    case IDLE:
      // Change state to counting set countdown values
      uiState = COUNTING;
      countdownStart = millis();

      // Update button text and draw
      refreshStartButton(countdownDuration / 1000);
      break;

    case COUNTING:
      // Fallthrough to next case

    case RUNNING:
      // Fallthrough to next case

    case COMPLETE:
      // Change state to idle and reset drive state
      uiState = IDLE;
      driveController.stop();

      // Update and draw start button
      refreshStartButton();
      break;
  }

  // Redraw grid cells
  uiGrid.drawGridCells(bot.display, gridModel, currentGridRenderMode());
}

void onTouchUndoButton() {
  Serial.println("Undo button touched");

  // Reset grid values and default path
  gridModel.resetPath();

  // Redraw grid cells
  uiGrid.drawGridCells(bot.display, gridModel, currentGridRenderMode());
}

void onTouchSettingsButton() {
  Serial.println("Settings button touched");

  // In idle state, change to settings state and draw settings menu
  if (uiState == IDLE) {
    uiState = SETTINGS;
    settingsMenu.draw(bot.display);
  } 
  // In settings state, change to idle state and draw UI
  else if (uiState == SETTINGS) {
    uiState = IDLE;
    drawUI();
  }
}

void onTouchGrid(int pixelX, int pixelY) {
  Serial.println("Grid touched");

  // Calculate grid column and row
  int gridCol = (pixelX - uiGrid.x) / CELL_SIZE;
  int gridRow = (pixelY - uiGrid.y) / CELL_SIZE;

  // Check if cell is selectable
  if (gridModel.isSelectable(gridRow, gridCol)) {
    // Add path cell to model
    gridModel.pathAdd(gridRow, gridCol);

    // Redraw grid cells
    uiGrid.drawGridCells(bot.display, gridModel, currentGridRenderMode(), gridRow - 2, gridRow + 2, gridCol - 2, gridCol + 2);
  }
}

void onTouchSettingsMenu(int pixelX, int pixelY) {
  // Determine which option was touched
  int optionIndex = settingsMenu.optionIndexContaining(pixelX, pixelY);
  
  // Handle arrow interactions
  if (optionIndex >= 0) {
    if (settingsMenu.leftArrowContains(pixelX, pixelY, optionIndex)) {
      handleSettingsArrow(static_cast<SettingOption>(optionIndex), -1);
    } else if (settingsMenu.rightArrowContains(pixelX, pixelY, optionIndex)) {
      handleSettingsArrow(static_cast<SettingOption>(optionIndex), 1);
    }
  }
}

void handleSettingsArrow(SettingOption option, int direction) {
  // Update the setting option value in the manager
  settingsManager.adjustSetting(option, direction);

  // Update value in settings menu and redraw
  switch (option) {
    case BRIGHTNESS:
      setBrightness();
      settingsMenu.updateOptionValue(BRIGHTNESS, String(settingsManager.getDisplayBrightness()) + "%");
      settingsMenu.redrawOption(BRIGHTNESS, bot.display);
      break;
    case DRIVE_SPEED:
      settingsMenu.updateOptionValue(DRIVE_SPEED, settingsManager.getDriveSpeedLabel());
      settingsMenu.redrawOption(DRIVE_SPEED, bot.display);
      break;
    case DRIVE_DISTANCE:
      settingsMenu.updateOptionValue(DRIVE_DISTANCE, settingsManager.getDriveDistanceLabel());
      settingsMenu.redrawOption(DRIVE_DISTANCE, bot.display);
      break;
  }
}

void loop() {
  // Handle countdown and movement states
  handleState();

  // Check for and process discrete user touch events
  if (bot.display.touched()) {
    TS_Point p = bot.display.getTouchPoint();
    handleTouch(p);
  }

  // Loop delay
  delay(50);
}