#ifndef SNAKEGAMEBOARD_H
#define SNAKEGAMEBOARD_H

#include "Arduino.h"
#include "U8g2lib.h"

#ifdef U8X8_HAVE_HW_SPI
#include "SPI.h"
#endif
#ifdef U8X8_HAVE_HW_I2C
#include "Wire.h"
#endif

#include "SPI.h"
#include "Wire.h"
#include "../bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

using namespace std;

class SnakeGameBoard {
private:

public:
    SnakeGameBoard();
    ~SnakeGameBoard();
};

#endif