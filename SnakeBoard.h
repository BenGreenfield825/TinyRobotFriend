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

    //--- soon to be irrelevant lel
    int headSize = 5;
    int headX = 64;
    int headY = 32;

    int body[100][2]; // hold snake body segment positions (max 100 segments, 2 coords positions)
    int segments = 0;
    //---

    static const int rows = 8;
    static const int cols = 16;
    int grid[rows][cols] = {0};

    int headPos[2] = {0}; // hold x/y (or r/c), ex: [3, 7]

    int spacer = 9;

    int foodSize = 5;
    int foodX;
    int foodY;
    bool isDead = false;

public:
    SnakeBoard(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u8g2);
    void parseDPad();
    void food();
    bool eatFood();
    void drawScreen(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u);
};

SnakeBoard::SnakeBoard(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u8g2)
{
    // first index represents vertical or horizontal, second index represents direction (0, 1 -> horizontal, right)
    int prevDir[2] = {0, 1};
    dPad = new DirectionalPad();

    food();

    // default starting position
    grid[int(floor(rows / 2))][int(floor(cols / 2))] = 1; // floor returns cringe double smh
    headPos[0] = int(floor(rows / 2));
    headPos[1] = int(floor(cols / 2));

    while (!isDead)
    {
        parseDPad();
        u8g2.clearBuffer();
        u8g2.drawFrame(0, 0, 128, 64);

        int r = 0;
        for (int i = 0; i < rows; i++)
        {
            u8g2.drawHLine(0, r += spacer, 128);
        }

        int h = 0;
        for (int i = 0; i < cols; i++)
        {
            u8g2.drawVLine(h += spacer, 0, 128);
        }

        // u8g2.drawBox(foodX, foodY, foodSize, foodSize); // draw food
        drawScreen(u8g2);

        // basically redoing all of this rip

        // if (eatFood())
        // {
        //     // TODO: make body grow
        //     score++;
        //     // I could probably do better than these two lines
        //     u8g2.clearBuffer();
        //     u8g2.drawFrame(0, 0, 128, 64);
        //     food();
        // }

        if (upButton)
        {
            prevDir[0] = 1;
            prevDir[1] = -1;
            //TODO: future format? Will need to check for out of bounds, also setting old squares back to 0
            grid[headPos[0] - 1][headPos[1]] = 1;
            headPos[0]--;
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
        // if (!prevDir[0])
        // {
        //     u8g2.drawBox(headX += prevDir[1] * 2, headY, headSize, headSize); // temp mult by 2 to make it "faster"
        // }
        // else
        // {
        //     u8g2.drawBox(headX, headY += prevDir[1] * 2, headSize, headSize);
        // }
        // if (segments > 0)
        // {
        //     // TODO: perhaps convert into a grid system, might make the body easier
        // }
        u8g2.sendBuffer();
        delay(gameSpeed);
    }
}

void SnakeBoard::drawScreen(U8G2_SSD1312_128X64_NONAME_F_SW_I2C u)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == 1)
            {
                u.drawBox(c ? c * spacer : spacer, r ? r * spacer : spacer, spacer, spacer);
            }
        }
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
    while (true)
    {
        foodX = random(0, cols);
        foodY = random(0, rows);
        // check to see if body piece exists
        // TODO: make this work right lol
        if (grid[foodY][foodX] == 0)
        {
            grid[foodY][foodX] = 1;
            break;
        }
        else
        {
            continue;
        }
    }
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