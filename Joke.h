#ifndef Joke_H
#define Joke_H

#include "Arduino.h"
#include "U8g2lib.h"

#include "DirectionalPad.h"

#include <string>
#include <vector>

class Joke
{
private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C *u8g2;

    const int randomPin = D0;
    DirectionalPad *dPad;
    int *dPadArr;
    int upButton = 0;
    int rightButton = 0;
    int downButton = 0;
    int leftButton = 0;

    int lineStart = 15; //default starting value
    int linePos = lineStart; 
    int lineSpacing = 10;

    // I'd rather put this in a txt file, but just having them in the class makes life simpler
    std::vector<const char *> jokeList = { // u8g2 expects text as const char* which is dumb 
        "A funny haha joke",
        "Another more different joke",
        "A really long joke that is meant to test how many characters I can get on the screen",
        "Super long haha funny joke now that my thing works! Isn't it nice when everything just works nicely?",
        "TESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTESTTEST"};    //This is max length to fit on screen

public:
    Joke(U8G2_SSD1306_128X64_NONAME_F_SW_I2C *u8g2Obj);
    void parseDPad();
    const char *processString(arduino::String currentJoke);
    void writeJokeToScreen(const char *currentJoke);
};

Joke::Joke(U8G2_SSD1306_128X64_NONAME_F_SW_I2C *u8g2Obj)
{
    u8g2 = u8g2Obj;
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_profont10_tf);
    dPad = new DirectionalPad();
    randomSeed(analogRead(randomPin));
    int randomIndex = random(0, jokeList.size());
    u8g2->drawStr(10, 10, jokeList[randomIndex]);
    while (true)
    {
        parseDPad();
        delay(100); // prevent button freaking out
        if (upButton || rightButton || downButton || leftButton)
        {
            u8g2->clearBuffer();
            randomSeed(analogRead(randomPin));
            randomIndex = random(0, jokeList.size());
            const char *currentJoke = jokeList[randomIndex];
            writeJokeToScreen(currentJoke);
            u8g2->drawRFrame(0, 0, 128, 64, 4); // draw border rounded-rectangle
            u8g2->sendBuffer();
        }
    }
}

void Joke::parseDPad()
{
    dPadArr = dPad->read();
    upButton = dPadArr[0];
    rightButton = dPadArr[1];
    downButton = dPadArr[2];
    leftButton = dPadArr[3];
}

const char *Joke::processString(arduino::String currentJoke)
{
    arduino::String temp = currentJoke;
    temp = temp.substring(0, 22);
    return temp.c_str();
}

void Joke::writeJokeToScreen(const char *currentJoke)
{
    if (strlen(currentJoke) > 22) // 22 characters is just about the edge of the screen
    {
        const char *processedStr = processString(currentJoke);
        u8g2->drawStr(10, linePos, processedStr);
        arduino::String temp = currentJoke;
        temp = temp.substring(22, strlen(currentJoke));
        const char *remainingStr = temp.c_str();
        linePos += lineSpacing;
        writeJokeToScreen(remainingStr);
        return;
    }
    else
    {
        u8g2->drawStr(10, linePos, currentJoke);
        linePos = lineStart;   //reset the linePos (if we hit this else, we should be done)
        return;
    }
}

#endif