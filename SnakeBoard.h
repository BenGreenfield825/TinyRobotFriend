#ifndef SnakeBoard_H
#define SnakeBoard_H

#include "Arduino.h"
#include "U8g2lib.h"

#include "DirectionalPad.h"

class SnakeBoard
{
private:
    //TODO: make an enum and make this a setting for difficulty, i.e. easy medium hard -> 50 25 10
    int gameSpeed = 25; //in milliseconds
    DirectionalPad* dPad;
public:
    SnakeBoard(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2);
};

SnakeBoard::SnakeBoard(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2)
{
    //start at i = 1 to not hit border (for loop is just a test rn anyways)
    dPad = new DirectionalPad();
    for(int i=1; i<30; i++) {
        u8g2.clearBuffer();
        u8g2.drawFrame(0,0,128,64);
        u8g2.drawBox(i+2, 30, 5, 5);
        u8g2.sendBuffer();
        // dPad->read();
        delay(gameSpeed);
    }
}

#endif