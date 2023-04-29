#ifndef Snake_H
#define Snake_H

#include "Arduino.h"
#include "U8g2lib.h"

#include "SnakeBoard.h"

class Snake
{
private:
    SnakeBoard* board;
public:
    Snake(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u8g2);
};

Snake::Snake(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u8g2)
    {
        board = new SnakeBoard(u8g2);
    }

#endif