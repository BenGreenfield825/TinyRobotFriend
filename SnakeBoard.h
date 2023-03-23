#ifndef SnakeBoard_H
#define SnakeBoard_H

#include "Arduino.h"
#include "U8g2lib.h"

#include "DirectionalPad.h"

class SnakeBoard
{
private:
    // TODO: make an enum and make this a setting for difficulty, i.e. easy medium hard -> 50 25 10
    int gameSpeed = 25; // in milliseconds
    DirectionalPad *dPad;
    int upButton;
    int rightButton;
    int downButton;
    int leftButton;
    int *dPadArr;

    int score = 0;

    int randomPin = D0;

    int headSize = 5;
    int headX = 64;
    int headY = 32;

    int body[100][2]; // hold snake body segment positions (max 100 segments, 2 coords positions)
    int segments = 0;

    int foodSize = 5;
    int foodX;
    int foodY;
    bool isDead = false;

public:
    SnakeBoard(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2);
    void parseDPad();
    void food();
    bool eatFood();
};

SnakeBoard::SnakeBoard(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2)
{
    // first index represents vertical or horizontal, second index represents direction (0, 1 -> horizontal, right)
    int prevDir[2] = {0, 1};
    dPad = new DirectionalPad();
    food();
    while (!isDead)
    {
        parseDPad();
        u8g2.clearBuffer();
        // u8g2.drawFrame(0, 0, 128, 64);

        // u8g2.drawVLine(10, 0, 1);
        // for (int i = 0; i < 129; i += 7)
        // {
        //     u8g2.drawVLine(i, 0, 64);
        //     u8g2.drawHLine(0, i, 128);
        // }

        int r = 0;
        for (int i =0; i<8 ; i++) {
            u8g2.drawHLine(0, r+=8, 128);
        }

        int h = 0;
        for (int i =0; i<16 ; i++) {
            u8g2.drawVLine(h+=8, 0, 128);
        }

        u8g2.drawBox(foodX, foodY, foodSize, foodSize); // draw food
        if (eatFood())
        {
            // TODO: make body grow
            score++;
            // I could probably do better than these two lines
            u8g2.clearBuffer();
            u8g2.drawFrame(0, 0, 128, 64);
            food();
        }

        if (upButton)
        {
            prevDir[0] = 1;
            prevDir[1] = -1;
        }
        else if (rightButton)
        {
            prevDir[0] = 0;
            prevDir[1] = 1;
        }
        else if (downButton)
        {
            prevDir[0] = 1;
            prevDir[1] = 1;
        }
        else if (leftButton)
        {
            prevDir[0] = 0;
            prevDir[1] = -1;
        }
        if (!prevDir[0])
        {
            u8g2.drawBox(headX += prevDir[1] * 2, headY, headSize, headSize); // temp mult by 2 to make it "faster"
        }
        else
        {
            u8g2.drawBox(headX, headY += prevDir[1] * 2, headSize, headSize);
        }
        if (segments > 0)
        {
            // TODO: perhaps convert into a grid system, might make the body easier
        }
        u8g2.sendBuffer();
        delay(gameSpeed);
    }
}

void SnakeBoard::parseDPad()
{
    dPadArr = dPad->read();
    upButton = dPadArr[0];
    rightButton = dPadArr[1];
    downButton = dPadArr[2];
    leftButton = dPadArr[3];
}

void SnakeBoard::food()
{
    randomSeed(analogRead(randomPin));
    // TODO: add logic to not add food where snake body exists
    // Make sure food is only on even positions (makes it easier to line up with food (that is if I keep the x2 pixel movement above))
    foodX = random(2, 127); // range 2 - 126 to provide wall buffer
    if (foodX % 2 != 0)
        foodX += 1;
    foodY = random(2, 31); // range 2 - 30 to provide wall buffer
    if (foodY % 2 != 0)
        foodY += 1;
}

bool SnakeBoard::eatFood()
{
    if (headX <= (foodX + foodSize) && headX >= foodX)
    {
        if (headY <= (foodY + foodSize) && headY >= foodY)
        {
            Serial.println("EAT");
            return true;
        }
    }
    return false;
}

#endif