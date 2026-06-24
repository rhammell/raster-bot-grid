#include "ui_elements.h"
#include "grid_model.h"

// This helper is still useful for subclasses
void UIButton::draw(Adafruit_ILI9341 &tft) const {
  tft.fillRect(x, y, width, height, bgColor);
}

// Implementation is moved here. It now uses member variables.
void UITextButton::draw(Adafruit_ILI9341 &tft) const {
  UIButton::draw(tft); // Draw the background first!

  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  int charWidth = FONT_CHAR_WIDTH * textSize;
  int textWidth = label.length() * charWidth;
  int textX = x + (width - textWidth) / 2;
  int textY = y + (height - (FONT_CHAR_HEIGHT * textSize)) / 2;
  tft.setCursor(textX, textY);
  tft.print(label);
}

void UIIconButton::draw(Adafruit_ILI9341 &tft) const {
  UIButton::draw(tft);

  int iconX = x + (width - iconWidth) / 2;
  int iconY = y + (height - iconHeight) / 2;
  tft.drawRGBBitmap(iconX, iconY, icon, iconWidth, iconHeight);
}

void UIArrow::draw(Adafruit_ILI9341 &tft) const {
  // Draw the background first
  UIButton::draw(tft);

  // Calculate the triangle points to be centered within the button bounds
  int centerX = x + width / 2;
  int centerY = y + height / 2;

  if (dir == ARROW_LEFT) {
    tft.fillTriangle(
        centerX - triangleWidth/2, centerY,  // Left point
        centerX + triangleWidth/2, y + (height - triangleHeight) / 2,  // Top point
        centerX + triangleWidth/2, y + (height + triangleHeight) / 2,  // Bottom point
        triangleColor);  // Use the triangle color for the arrow
  } else {
    tft.fillTriangle(
        centerX + triangleWidth/2, centerY,  // Right point
        centerX - triangleWidth/2, y + (height - triangleHeight) / 2,  // Top point
        centerX - triangleWidth/2, y + (height + triangleHeight) / 2,  // Bottom point
        triangleColor);  // Use the triangle color for the arrow
  }
}

void UISettingsValue::draw(Adafruit_ILI9341 &tft) const {
    // Clear the entire value area using calculated height
    int textHeight = FONT_CHAR_HEIGHT * textSize;
    tft.fillRect(x, y, width, textHeight, backgroundColor);

    // Set text properties
    tft.setTextSize(textSize);
    tft.setTextColor(textColor);
    
    // Calculate centered position for text using FONT_CHAR_WIDTH
    int fontPadding = textSize;
    int scaledCharWidth = FONT_CHAR_WIDTH * textSize;
    int actualTextWidth = value.length() * scaledCharWidth - fontPadding;
    int textX = x + (width - actualTextWidth) / 2;
    
    tft.setCursor(textX, y);
    tft.print(value);
}

void UISettingsOption::layout() {
    // Calculate character width based on text size
    int charWidth = FONT_CHAR_WIDTH * textSize;
    int fontPadding = textSize;
    
    // Calculate label position (centered)
    int labelWidth = label.length() * charWidth - fontPadding;
    labelX = x + (width - labelWidth) / 2;
    labelY = y;
    
    // Calculate arrow positions
    int arrowY = labelY + 20;
    int leftArrowX = x + arrowMarginX;
    int rightArrowX = x + width - arrowMarginX - arrowWidth;
    
    // Set up arrows
    leftArrow.setBounds(leftArrowX, arrowY, arrowWidth, arrowHeight);
    leftArrow.setDirection(ARROW_LEFT);
    leftArrow.setTriangleSize(12, 18);
    leftArrow.setBgColor(backgroundColor);
    leftArrow.setTriangleColor(textColor);
    
    rightArrow.setBounds(rightArrowX, arrowY, arrowWidth, arrowHeight);
    rightArrow.setDirection(ARROW_RIGHT);
    rightArrow.setTriangleSize(12, 18);
    rightArrow.setBgColor(backgroundColor);
    rightArrow.setTriangleColor(textColor);
    
    // Calculate value position
    int maxValueChars = 8; // "Standard" is the longest label
    int maxValueWidth = maxValueChars * charWidth - fontPadding;
    int valueX = x + (width - maxValueWidth) / 2;
    int valueY = arrowY + (arrowHeight - textSize * FONT_CHAR_HEIGHT) / 2;
    value.setPosition(valueX, valueY, maxValueWidth);
    value.setColors(backgroundColor, textColor);
    value.setTextSize(textSize);
}

void UISettingsOption::draw(Adafruit_ILI9341 &tft) const {
    // Set text properties for label
    tft.setTextSize(textSize);
    tft.setTextColor(textColor);
    
    // Draw label using precomputed labelX and labelY
    tft.setCursor(labelX, labelY);
    tft.print(label);
    
    // Draw arrows
    leftArrow.draw(tft);
    rightArrow.draw(tft);
    
    // Draw value
    value.draw(tft);
}

void UISettingsMenu::layout() {
    // Set up each option with proper positioning
    for (int i = 0; i < numOptions; i++) {
        int optionY = y + 20 + (i * optionSpacing);
        options[i].setPosition(x, optionY, width, optionSpacing);
        options[i].setColors(backgroundColor, textColor);
        options[i].setTextSize(textSize);
        options[i].setArrowSize(arrowWidth, arrowHeight);
        options[i].setArrowMargin(arrowMarginX);
        options[i].layout();
    }
}

void UISettingsMenu::draw(Adafruit_ILI9341 &tft) const {
    // Draw menu background
    tft.fillRect(x, y, width, height, backgroundColor);
    tft.drawRect(x, y, width, height, borderColor);
    
    // Draw all options
    for (int i = 0; i < numOptions; i++) {
        options[i].draw(tft);
    }
}

int UISettingsMenu::optionIndexContaining(int px, int py) const {
    for (int i = 0; i < numOptions; i++) {
        if (px >= options[i].x && px <= options[i].x + options[i].width &&
            py >= options[i].y && py <= options[i].y + options[i].height) {
            return i;
        }
    }
    return -1;
}

void UISettingsMenu::setupOptions(const String* labels, int count) {
    numOptions = min(count, MAX_OPTIONS);
    for (int i = 0; i < numOptions; i++) {
        options[i].setLabel(labels[i]);
    }
}

void UISettingsMenu::updateOptionValue(int optionIndex, const String& value) {
    if (optionIndex >= 0 && optionIndex < numOptions) {
        options[optionIndex].setValue(value);
    }
}

void UISettingsMenu::redrawOption(int optionIndex, Adafruit_ILI9341 &tft) const {
    if (optionIndex >= 0 && optionIndex < numOptions) {
        options[optionIndex].draw(tft);
    }
}

// Implementation of arrow hit-testing helpers for UISettingsMenu
bool UISettingsMenu::leftArrowContains(int px, int py, int optionIndex) const {
    if (optionIndex >= 0 && optionIndex < numOptions) {
        return options[optionIndex].leftArrow.contains(px, py);
    }
    return false;
}

bool UISettingsMenu::rightArrowContains(int px, int py, int optionIndex) const {
    if (optionIndex >= 0 && optionIndex < numOptions) {
        return options[optionIndex].rightArrow.contains(px, py);
    }
    return false;
}

// --- UIGrid implementation ---
UIGrid::UIGrid() : x(0), y(0), width(0), height(0), numRows(0), numCols(0) {}

void UIGrid::setSize(int rows, int cols) {
    numRows = rows;
    numCols = cols;
}

void UIGrid::setBounds(int bx, int by, int w, int h) {
    x = bx;
    y = by;
    width = w;
    height = h;
}

void UIGrid::drawGridLines(Adafruit_ILI9341 &tft) {
    for (int i = 0; i < numRows + 1; i++) {
        tft.drawLine(
            x,
            i * CELL_SIZE + y,
            x + width,
            i * CELL_SIZE + y,
            GRID_LINE_COLOR);
    }
    for (int i = 0; i < numCols + 1; i++) {
        tft.drawLine(
            i * CELL_SIZE + x,
            y,
            i * CELL_SIZE + x,
            y + height,
            GRID_LINE_COLOR);
    }
}

// Helper for drawing cell direction (private to this file)
namespace {
void drawCellDirection(Adafruit_ILI9341 &tft, GridModel &model, int gridX, int gridY, int row, int col, int nextRow, int nextCol, uint16_t arrowColor) {
    int centerX = col * CELL_SIZE + gridX + (CELL_SIZE / 2);
    int centerY = row * CELL_SIZE + gridY + (CELL_SIZE / 2);
    PathCell lastPathCell = model.getPathCell(model.getPathLength() - 1);
    if (row == lastPathCell.row && col == lastPathCell.col) {
        int circleRadius = 3;
        tft.fillCircle(centerX, centerY, circleRadius, arrowColor);
        return;
    }
    int dx = nextCol - col;
    int dy = nextRow - row;
    if (dx > 0) {
        tft.fillTriangle(centerX - 3, centerY - 3, centerX - 3, centerY + 3, centerX + 3, centerY, arrowColor);
    } else if (dx < 0) {
        tft.fillTriangle(centerX + 3, centerY - 3, centerX + 3, centerY + 3, centerX - 3, centerY, arrowColor);
    } else if (dy > 0) {
        tft.fillTriangle(centerX - 3, centerY - 3, centerX + 3, centerY - 3, centerX, centerY + 3, arrowColor);
    } else if (dy < 0) {
        tft.fillTriangle(centerX - 3, centerY + 3, centerX + 3, centerY + 3, centerX, centerY - 3, arrowColor);
    }
}
}

void UIGrid::drawGridCells(Adafruit_ILI9341 &tft, GridModel &model, UIState state) {
    drawGridCells(tft, model, state, 0, model.getNumRows() - 1, 0, model.getNumCols() - 1);
}

void UIGrid::drawGridCells(Adafruit_ILI9341 &tft, GridModel &model, UIState state, int startRow, int endRow, int startCol, int endCol) {
    // Clamp values to grid bounds
    startRow = max(0, min(startRow, model.getNumRows() - 1));
    endRow = max(0, min(endRow, model.getNumRows() - 1));
    startCol = max(0, min(startCol, model.getNumCols() - 1));
    endCol = max(0, min(endCol, model.getNumCols() - 1));

    for (int i = startRow; i <= endRow; i++) {
        for (int j = startCol; j <= endCol; j++) {
            int color;
            if (model.isCellActivated(i, j)) {
                if (state == RUNNING || state == COMPLETE) {
                    bool isProcessed = false;
                    for (int p = 0; p <= model.getCurrentPathIndex(); p++) {
                        PathCell pathCell = model.getPathCell(p);
                        if (i == pathCell.row && j == pathCell.col) {
                            isProcessed = true;
                            break;
                        }
                    }
                    if (isProcessed) {
                        color = GRID_SELECTABLE_COLOR;
                    } else {
                        color = GRID_SELECTED_COLOR;
                    }
                } else {
                    color = GRID_SELECTED_COLOR;
                }
            } else if (state == IDLE && model.isSelectable(i, j)) {
                color = GRID_SELECTABLE_COLOR;
            } else {
                color = GRID_EMPTY_COLOR;
            }
            tft.fillRect(
                j * CELL_SIZE + x + 1,
                i * CELL_SIZE + y + 1,
                CELL_SIZE - 1,
                CELL_SIZE - 1,
                color);
            for (int p = 0; p < model.getPathLength(); p++) {
                PathCell pathCell = model.getPathCell(p);
                if (i == pathCell.row && j == pathCell.col) {
                    if (model.getPathLength() == 1) {
                        drawCellDirection(tft, model, x, y, pathCell.row, pathCell.col, pathCell.row - 1, pathCell.col, GRID_ARROW_COLOR);
                    } else if (p == model.getPathLength() - 1) {
                        drawCellDirection(tft, model, x, y, pathCell.row, pathCell.col, pathCell.row, pathCell.col, GRID_ARROW_COLOR);
                    } else {
                        PathCell nextPathCell = model.getPathCell(p + 1);
                        drawCellDirection(tft, model, x, y, pathCell.row, pathCell.col, nextPathCell.row, nextPathCell.col, GRID_ARROW_COLOR);
                    }
                }
            }
        }
    }
}

bool UIGrid::isPointInRect(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
    return x >= rectX && x <= rectX + rectWidth && y >= rectY && y <= rectY + rectHeight;
}

bool UIGrid::contains(int x, int y) const {
    return isPointInRect(x, y, this->x, this->y, this->width - 1, this->height - 1);
}
