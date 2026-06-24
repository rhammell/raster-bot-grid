#ifndef GRID_MODEL_H
#define GRID_MODEL_H

#include <Arduino.h>

// Direction enum for grid movement
enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

// Path cell structure
struct PathCell {
  int row;
  int col;
};

class GridModel {
private:
  // Grid dimensions
  int numRows;
  int numCols;
  
  // Grid data
  bool** gridVals;
  
  // Path data
  PathCell* path;
  int pathLength;
  int currentPathIndex;
  Direction currentDirection;
  
public:
  // Constructor
  GridModel();
  
  // Initialization methods
  void initGrid(int numRows, int numCols);
  void resetGridValues();
  void initPath();
  void resetDefaultPath();
  
  // Path manipulation methods
  void pathAdd(int row, int col);
  bool isSelectable(int row, int col);
  void resetPath();
  
  // Grid state methods
  bool getGridValue(int row, int col);
  void setGridValue(int row, int col, bool value);
  bool isCellActivated(int row, int col);
  
  // Path access methods
  PathCell getPathCell(int index);
  PathCell* getPath();
  int getPathLength();
  int getCurrentPathIndex();
  void setCurrentPathIndex(int index);
  PathCell getCurrentPathCell();
  PathCell getNextPathCell();
  bool isPathComplete();
  
  // Direction methods
  Direction getCurrentDirection();
  void setCurrentDirection(Direction direction);
  Direction getNextDirection();
  
  // Grid dimension getters
  int getNumRows();
  int getNumCols();
  
  // Utility methods
  bool isInGridBounds(int row, int col);
  void clearAll();
};

#endif // GRID_MODEL_H 