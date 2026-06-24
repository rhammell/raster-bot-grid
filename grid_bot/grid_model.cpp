#include "grid_model.h"
#include <Adafruit_GFX.h>

GridModel::GridModel() {
  // Initialize member variables
  numRows = 0;
  numCols = 0;
  
  // Initialize grid data
  gridVals = nullptr;
  
  // Initialize path data
  path = nullptr;
  pathLength = 0;
  currentPathIndex = 0;
  currentDirection = UP;
}

void GridModel::initGrid(int numRows_, int numCols_) {
  // Set grid size based on parameters
  numRows = numRows_;
  numCols = numCols_;

  // Initialize 2D grid of cell values
  gridVals = new bool*[numRows];
  for (int i = 0; i < numRows; i++) {
    gridVals[i] = new bool[numCols];
  }

  // Reset all grid values to false
  resetGridValues();
  
  // Initialize path
  initPath();
}

void GridModel::resetGridValues() {
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      gridVals[i][j] = false;
    }
  }
}

void GridModel::initPath() {
  // Initialize array of path points
  path = new PathCell[numRows * numCols];
  
  // Set initial default path points
  resetDefaultPath();
}

void GridModel::resetDefaultPath() {
  // Set path length to zero
  pathLength = 0;
  currentPathIndex = 0;

  // Add two points at center bottom of grid
  int startRow = numRows - 1;
  int startCol = numCols / 2;
  pathAdd(startRow, startCol);
  pathAdd(startRow - 1, startCol);
}

void GridModel::pathAdd(int row, int col) {
  // Add coordinate to the list of path points
  path[pathLength] = { row, col };
  pathLength += 1;

  // Update the grid value of point
  gridVals[row][col] = true;
}

bool GridModel::isSelectable(int row, int col) {
  // If cell is already selected, it's not selectable
  if (gridVals[row][col]) {
    return false;
  }

  // Get the last point in the path
  PathCell lastPoint = path[pathLength - 1];

  // Check if cell is adjacent (not diagonal) to last point
  bool isAdjacent = (
    // Check horizontal adjacency
    (row == lastPoint.row && abs(col - lastPoint.col) == 1) ||
    // Check vertical adjacency
    (col == lastPoint.col && abs(row - lastPoint.row) == 1));

  return isAdjacent;
}

void GridModel::resetPath() {
  resetGridValues();
  resetDefaultPath();
}

bool GridModel::getGridValue(int row, int col) {
  if (isInGridBounds(row, col)) {
    return gridVals[row][col];
  }
  return false;
}

void GridModel::setGridValue(int row, int col, bool value) {
  if (isInGridBounds(row, col)) {
    gridVals[row][col] = value;
  }
}

bool GridModel::isCellActivated(int row, int col) {
  return getGridValue(row, col);
}

PathCell GridModel::getPathCell(int index) {
  if (index >= 0 && index < pathLength) {
    return path[index];
  }
  return { -1, -1 }; // Invalid cell
}

PathCell* GridModel::getPath() {
  return path;
}

int GridModel::getPathLength() {
  return pathLength;
}

int GridModel::getCurrentPathIndex() {
  return currentPathIndex;
}

void GridModel::setCurrentPathIndex(int index) {
  if (index >= 0 && index < pathLength) {
    currentPathIndex = index;
  }
}

PathCell GridModel::getCurrentPathCell() {
  return getPathCell(currentPathIndex);
}

PathCell GridModel::getNextPathCell() {
  return getPathCell(currentPathIndex + 1);
}

bool GridModel::isPathComplete() {
  return currentPathIndex >= pathLength - 1;
}

Direction GridModel::getCurrentDirection() {
  return currentDirection;
}

void GridModel::setCurrentDirection(Direction direction) {
  currentDirection = direction;
}

int GridModel::getNumRows() {
  return numRows;
}

int GridModel::getNumCols() {
  return numCols;
}

bool GridModel::isInGridBounds(int row, int col) {
  return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

void GridModel::clearAll() {
  resetGridValues();
  resetDefaultPath();
}

Direction GridModel::getNextDirection() {
  // If we're at the last cell, there's no next direction
  if (isPathComplete()) {
    return getCurrentDirection(); // Return current direction as fallback
  }
  
  // Get current and next path cells
  PathCell current = getCurrentPathCell();
  PathCell next = getNextPathCell();
  
  // Calculate direction between them
  if (next.row < current.row) return UP;    // Up
  if (next.col > current.col) return RIGHT; // Right
  if (next.row > current.row) return DOWN;  // Down
  if (next.col < current.col) return LEFT;  // Left
  return getCurrentDirection();             // No change
} 