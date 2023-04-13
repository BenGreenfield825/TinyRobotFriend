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
    const int randomPin = D0;
    DirectionalPad *dPad;
    int *dPadArr;
    int upButton = 0;
    int rightButton = 0;
    int downButton = 0;
    int leftButton = 0;

    // I'd rather put this in a txt file, but just having them in the class makes life simpler
    std::vector<const char *> jokeList = {  //u8g2 expects text as const char* which is dumb and does not auto convert from string which is more dumb (I could probably just cast string vector to const char in u8g2 but maybe later)
    // std::vector<std::string> jokeList = {
        "A funny haha joke",
        "Another more different joke",
        "A really long joke that is meant to test how many characters I can get on the screen"};

public:
    Joke(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2);
    void parseDPad();
};

Joke::Joke(U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_tf);
    dPad = new DirectionalPad();
    randomSeed(analogRead(randomPin));
    int randomIndex = random(0, jokeList.size());
    u8g2.drawStr(10, 10, jokeList[randomIndex]);
    // u8g2.drawStr(10, 10, jokeList[randomIndex].c_str());
    while (true)
    {
        // u8g2.drawRFrame(0, 0, 128, 64, 4); // draw border rounded-rectangle
        parseDPad();
        delay(100); // prevent button freaking out
        if (upButton || rightButton || downButton || leftButton)
        {
            u8g2.clearBuffer();
            randomSeed(analogRead(randomPin));
            randomIndex = random(0, jokeList.size());
            // if(strlen(jokeList[randomIndex]) > 22)  //22 characters is just about the edge of the screen
            // {
            //     std::string temp = jokeList[randomIndex];    //temporarily store as a string
            //     // u8g2.drawStr(10, 10, jokeList[randomIndex].substr(0, 22));
            //     // u8g2.drawStr(10, 10, temp.substr(0, 22).c_str());
            //     // u8g2.drawStr(10, 20, jokeList[randomIndex].substr(22, jokeList.size())); //temp line, will need to check to see if it is another 22 chars longer
            //     // u8g2.drawStr(10, 20, temp.substr(22, jokeList.size()).c_str());
            // }
            // else{
            //     u8g2.drawStr(10, 10, jokeList[randomIndex]);
            // }
            u8g2.drawStr(10, 10, jokeList[randomIndex]);
            // u8g2.drawStr(10, 10, jokeList[randomIndex].c_str());
            u8g2.sendBuffer();
            // return;
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

#endif