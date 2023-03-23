#ifndef SnakeGrid_H
#define SnakeGrid_H

#include "Arduino.h"
#include "U8g2lib.h"

class SnakeGrid
{
private:
    int rows = 8; //uhh idk
    int columns = 17;   //128 pixels - 2 for border = 126 usable, snake will be 6x6 pixels, 21 columns for 21x6=126
    int grid[rows][columns] = {};
public:
}

#endif