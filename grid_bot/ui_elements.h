#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include "grid_model.h"
#include "state.h"

// UI Configuration Constants
#define CELL_SIZE 30

// Font metrics
const int FONT_CHAR_WIDTH = 6;
const int FONT_CHAR_HEIGHT = 8;

// Button dimensions
const int BUTTON_HEIGHT = 36;
const int BUTTON_MARGIN = 2;
const int UNDO_BUTTON_WIDTH = 36;
const int SETTINGS_BUTTON_WIDTH = 36;

// Background color
const uint16_t BACKGROUND_COLOR = 0x0000; // Black

// Button colors
const uint16_t BUTTON_BACKGROUND_COLOR = ILI9341_DARKGREY;
const uint16_t BUTTON_IDLE_COLOR =  0x4a7f;     // Blue (75, 75, 225)
const uint16_t BUTTON_COUNTING_COLOR = 0xebe4;  // Orange (240, 124, 36)
const uint16_t BUTTON_RUNNING_COLOR = 0xfa69;   // Red (255, 75, 75)
const uint16_t BUTTON_COMPLETE_COLOR = 0x4fe9;  // Green (75, 255, 75)
const uint16_t BUTTON_TEXT_COLOR = ILI9341_WHITE;     

// Grid colors
const uint16_t GRID_BACKGROUND_COLOR = ILI9341_BLACK;
const uint16_t GRID_SELECTED_COLOR = ILI9341_BLACK;
const uint16_t GRID_EMPTY_COLOR = ILI9341_WHITE; 
const uint16_t GRID_LINE_COLOR = ILI9341_DARKGREY;
const uint16_t GRID_SELECTABLE_COLOR = 0x4fe9;    // Green (75, 255, 75)
const uint16_t GRID_ARROW_COLOR = ILI9341_WHITE;

// Settings menu colors
const uint16_t SETTINGS_BACKGROUND_COLOR = ILI9341_DARKGREY;
const uint16_t SETTINGS_TEXT_COLOR = ILI9341_WHITE;
const uint16_t SETTINGS_BORDER_COLOR = ILI9341_WHITE;
const uint16_t SETTINGS_ARROW_COLOR = ILI9341_WHITE;

// Settings menu dimensions
const int SETTINGS_TEXT_SIZE = 2;
const int SETTINGS_ARROW_WIDTH = 38;
const int SETTINGS_ARROW_HEIGHT = 30;
const int SETTINGS_ARROW_MARGIN_X = 2;
const int SETTINGS_FONT_PADDING = SETTINGS_TEXT_SIZE;
const int SETTINGS_OPTION_SPACING = 70;

// Base class now only contains what is common to ALL buttons.
class UIButton {
public:
  int x;
  int y;
  int width;
  int height;
  uint16_t bgColor;

  UIButton() : x(0), y(0), width(0), height(0), bgColor(0) {}

  void setBounds(int bx, int by, int w, int h) {
    x = bx;
    y = by;
    width = w;
    height = h;
  }

  bool contains(int px, int py) const {
    return px >= x && px <= x + width && py >= y && py <= y + height;
  }

  void setBgColor(uint16_t color) { bgColor = color; }

  // The base draw method is now virtual.
  virtual void draw(Adafruit_ILI9341 &tft) const;
};

class UITextButton : public UIButton {
public:
  String label;
  uint16_t textColor;
  uint8_t textSize;

  UITextButton() : label(""), textColor(BUTTON_TEXT_COLOR), textSize(2) {}

  void setLabel(const String &l) { label = l; }
  void setTextColor(uint16_t c) { textColor = c; }
  void setTextSize(uint8_t size) { textSize = size; }

  // This is the primary drawing method for this class.
  void draw(Adafruit_ILI9341 &tft) const override;
};

class UIIconButton : public UIButton {
public:
  const uint16_t *icon;
  int iconWidth;
  int iconHeight;

  UIIconButton() : icon(nullptr), iconWidth(0), iconHeight(0) {
    bgColor = BUTTON_BACKGROUND_COLOR;  // Set default background color
  }

  void setIcon(const uint16_t *i, int w, int h) {
    icon = i;
    iconWidth = w;
    iconHeight = h;
  }

  // This is the primary drawing method for this class.
  void draw(Adafruit_ILI9341 &tft) const override;
};

enum ArrowDirection { ARROW_LEFT, ARROW_RIGHT };

class UIArrow : public UIButton {
public:
  ArrowDirection dir;
  int triangleWidth;   // New attribute for explicit triangle width
  int triangleHeight;  // New attribute for explicit triangle height
  uint16_t triangleColor;  // New attribute for triangle color

  UIArrow() : dir(ARROW_LEFT), triangleWidth(12), triangleHeight(18), triangleColor(SETTINGS_ARROW_COLOR) {}  // Default values based on previous 60% of 20x30

  void setDirection(ArrowDirection d) { dir = d; }
  void setTriangleSize(int width, int height) {  // New setter method
    triangleWidth = width;
    triangleHeight = height;
  }
  void setTriangleColor(uint16_t color) { triangleColor = color; }

  // Override the draw method to draw the arrow
  void draw(Adafruit_ILI9341 &tft) const override;
};

class UISettingsValue {
public:
    int x;
    int y;
    int width;
    uint16_t backgroundColor;
    uint16_t textColor;
    uint8_t textSize;
    String value;

    UISettingsValue() : x(0), y(0), width(0),
                       backgroundColor(SETTINGS_BACKGROUND_COLOR),
                       textColor(SETTINGS_TEXT_COLOR),
                       textSize(2),
                       value("") {}

    void setPosition(int bx, int by, int w) {
        x = bx;
        y = by;
        width = w;
    }

    void setColors(uint16_t bg, uint16_t txt) {
        backgroundColor = bg;
        textColor = txt;
    }

    void setTextSize(uint8_t size) { textSize = size; }
    void setValue(const String &val) { value = val; }

    void draw(Adafruit_ILI9341 &tft) const;
};

class UISettingsOption {
public:
    String label;
    UIArrow leftArrow;
    UIArrow rightArrow;
    UISettingsValue value;
    int x;
    int y;
    int width;
    int height;
    uint16_t backgroundColor;
    uint16_t textColor;
    uint8_t textSize;
    int arrowWidth;
    int arrowHeight;
    int arrowMarginX;
    int optionSpacing;
    int labelX;
    int labelY;

    UISettingsOption() : label(""), x(0), y(0), width(0), height(0),
                        backgroundColor(SETTINGS_BACKGROUND_COLOR),
                        textColor(SETTINGS_TEXT_COLOR),
                        textSize(2),
                        arrowWidth(38),
                        arrowHeight(30),
                        arrowMarginX(2),
                        optionSpacing(70) {}

    void setPosition(int bx, int by, int w, int h) {
        x = bx;
        y = by;
        width = w;
        height = h;
    }

    void setLabel(const String &l) { label = l; }
    void setColors(uint16_t bg, uint16_t txt) {
        backgroundColor = bg;
        textColor = txt;
    }
    void setTextSize(uint8_t size) { textSize = size; }
    void setArrowSize(int w, int h) {
        arrowWidth = w;
        arrowHeight = h;
    }
    void setArrowMargin(int margin) { arrowMarginX = margin; }
    void setOptionSpacing(int spacing) { optionSpacing = spacing; }
    void setValue(const String &val) { value.setValue(val); }

    void layout();
    void draw(Adafruit_ILI9341 &tft) const;
};

class UISettingsMenu {
public:
    int x;
    int y;
    int width;
    int height;
    uint16_t backgroundColor;
    uint16_t borderColor;
    uint16_t textColor;
    uint8_t textSize;
    int fontPadding;
    int arrowWidth;
    int arrowHeight;
    int arrowMarginX;
    int optionSpacing;

    UISettingsMenu() : x(0), y(0), width(0), height(0),
                      backgroundColor(SETTINGS_BACKGROUND_COLOR),
                      borderColor(SETTINGS_BORDER_COLOR),
                      textColor(SETTINGS_TEXT_COLOR),
                      textSize(SETTINGS_TEXT_SIZE),
                      fontPadding(SETTINGS_FONT_PADDING),
                      arrowWidth(SETTINGS_ARROW_WIDTH),
                      arrowHeight(SETTINGS_ARROW_HEIGHT),
                      arrowMarginX(SETTINGS_ARROW_MARGIN_X),
                      optionSpacing(SETTINGS_OPTION_SPACING),
                      numOptions(0) {}

    void setPosition(int bx, int by, int w, int h) {
        x = bx;
        y = by;
        width = w;
        height = h;
    }

    void setColors(uint16_t bg, uint16_t border, uint16_t txt) {
        backgroundColor = bg;
        borderColor = border;
        textColor = txt;
    }

    void setTextSize(uint8_t size) { textSize = size; }
    void setArrowSize(int w, int h) {
        arrowWidth = w;
        arrowHeight = h;
    }
    void setArrowMargin(int margin) { arrowMarginX = margin; }
    void setOptionSpacing(int spacing) { optionSpacing = spacing; }

    void layout();
    void draw(Adafruit_ILI9341 &tft) const;
    int optionIndexContaining(int px, int py) const; // Returns index of option containing the point, or -1 if none

    // Helper methods for setup and updates
    void setupOptions(const String* labels, int count);
    void updateOptionValue(int optionIndex, const String& value);

    // Add this method to the UISettingsMenu class
    void redrawOption(int optionIndex, Adafruit_ILI9341 &tft) const;

    // Helper functions for arrow hit-testing
    bool leftArrowContains(int px, int py, int optionIndex) const;
    bool rightArrowContains(int px, int py, int optionIndex) const;

    // Check if a point is within the menu bounds
    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    // Public getter for options
    UISettingsOption& getOption(int index) { return options[index]; }
    const UISettingsOption& getOption(int index) const { return options[index]; }

private:
    static const int MAX_OPTIONS = 3;
    UISettingsOption options[MAX_OPTIONS];
    int numOptions;
};

// --- UIGrid class for grid UI management ---
class UIGrid {
public:
    int x, y, width, height;
    int numRows, numCols;

    UIGrid();
    void setSize(int rows, int cols);
    void setBounds(int bx, int by, int w, int h);
    void drawGridLines(Adafruit_ILI9341 &tft);
    void drawGridCells(Adafruit_ILI9341 &tft, GridModel &model, UIState state);
    void drawGridCells(Adafruit_ILI9341 &tft, GridModel &model, UIState state, int startRow, int endRow, int startCol, int endCol);
    static bool isPointInRect(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight);
    bool contains(int x, int y) const;
};

#endif
