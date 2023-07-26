#ifndef WhackAMole_H
#define WhackAMole_H

//All of this is absolutely horrible and I would fix it but idr how it works and I don't care to find out and I'm out of time so #goodenough

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DirectionalPad.h"

#define CURSOR_H 8
#define CURSOR_W 8
const unsigned char cursorImg[] PROGMEM = {
  0B00111100,
  0B01011010,
  0B10011001,
  0B11111111,
  0B11111111,
  0B10011001,
  0B01011010,
  0B00111100
};
#define MOLE 8
const unsigned char moleMap[] PROGMEM = {
  0B10100101,
  0B01000010,
  0B10100101,
  0B00000000,
  0B11111111,
  0B10000001,
  0B01000010,
  0B00111000
};


class WhackAMole
{
private:
  Adafruit_SSD1306 *display;
  const int buttonPin = 2;
  const int Xpin = A0;
  const int Ypin = A1;
  int hTopRowY = 20;
  int hBottomRowY = 40;
  int h1x = 30;
  int h2x = 60;
  int h3x = 90;
  int h4x = 45;
  int h5x = 75;
  int cursorOffsetX = 3;
  int cursorOffsetY = 4;
  int cursorPosX;
  int cursorPosY;
  int moleX;
  int moleY;
  int score = 0;
  int interruptCounter = 0;

  DirectionalPad *dPad;
  int *dPadArr;
  int upButton = 0;
  int rightButton = 0;
  int downButton = 0;
  int leftButton = 0;

public:
  WhackAMole(Adafruit_SSD1306 *displayObj);
  void parseDPad();
  void moleSetup();
  void Holes();
  void game();
  void MoleHole();
};

WhackAMole::WhackAMole(Adafruit_SSD1306 *displayObj)
{
  Serial.println("constructor");
  display = displayObj;
  dPad = new DirectionalPad();
  display->clearDisplay();
  moleSetup();
  game();
}

void WhackAMole::parseDPad()
{
    dPadArr = dPad->read();
    upButton = dPadArr[0];
    rightButton = dPadArr[1];
    downButton = dPadArr[2];
    leftButton = dPadArr[3];
}

void WhackAMole::moleSetup() {
  // Serial.begin(9600);
  // pinMode(Xpin, INPUT);
  // pinMode(Ypin, INPUT);
  // pinMode(buttonPin, INPUT_PULLUP);
  // Timer1.initialize(4e+6);
  // Timer1.attachInterrupt(MoleHole); // Call MoleHole to decide what hole every 4 seconds
  cursorPosX = h1x; //where cursor should start
  cursorPosY = hTopRowY;
  moleX = -10;  //start mole off screen
  moleY = -10;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // if (!wackDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for (;;); // Don't proceed, loop forever
  // }

  display->clearDisplay();

}

void WhackAMole::Holes() {
  int circleRad = 10;
  display->clearDisplay();
  //draw holes
  display->fillCircle(h1x, hTopRowY, circleRad, SSD1306_INVERSE);
  display->fillCircle(h2x, hTopRowY, circleRad, SSD1306_INVERSE);
  display->fillCircle(h3x, hTopRowY, circleRad, SSD1306_INVERSE);
  display->fillCircle(h4x, hBottomRowY, circleRad, SSD1306_INVERSE);
  display->fillCircle(h5x, hBottomRowY, circleRad, SSD1306_INVERSE);
  //draw cursor and mole
  display->drawBitmap(cursorPosX - cursorOffsetX, cursorPosY - cursorOffsetY, cursorImg, CURSOR_W, CURSOR_H, SSD1306_INVERSE); //cursor
  display->drawBitmap(moleX - cursorOffsetX, moleY - cursorOffsetY, moleMap, MOLE, MOLE, SSD1306_INVERSE); //mole
  //draw score on screen
  display->setTextSize(1);      
  display->setTextColor(SSD1306_WHITE); 
  display->setCursor(0, 55);     
  display->print(F("S: "));
  display->print(score);
  display->display();
}

void WhackAMole::game() {
  unsigned long prev;
  int period = 1500;
  // MoleHole();
  while(true) {
    if((millis() - prev) > period)
    {
      prev = millis();
      MoleHole();
    }
    parseDPad();
    int rawX;
    int rawY;
    // int rawX = analogRead(Xpin);
    // int rawY = analogRead(Ypin);

    Holes();
    if (upButton) {
    // if ((rawX < 1024 && rawX > 1020) && (rawY < 900 && rawY > 500)) {
      //up
      if (cursorPosY == hTopRowY) {
        cursorPosY = hBottomRowY;
        if (cursorPosX == h1x) {
          cursorPosX = h4x;
        }
        else if (cursorPosX == h2x || cursorPosX == h3x) {
          cursorPosX = h5x;
        }
      }
      else if (cursorPosY == hBottomRowY) {
        cursorPosY = hTopRowY;
        if (cursorPosX == h4x) {
          cursorPosX = h1x;
        }
        else if (cursorPosX == h5x) {
          cursorPosX = h2x;
        }
      }
      delay(150);
    }
    if (downButton) {
    // if ((rawX < 100 && rawX >= 0) && (rawY < 900 && rawY > 500)) {
      //down
      if (cursorPosY == hTopRowY) {
        cursorPosY = hBottomRowY;
        if (cursorPosX == h1x) {
          cursorPosX = h4x;
        }
        else if (cursorPosX == h2x || cursorPosX == h3x) {
          cursorPosX = h5x;
        }
      }
      else if (cursorPosY == hBottomRowY) {
        cursorPosY = hTopRowY;
        if (cursorPosX == h4x) {
          cursorPosX = h1x;
        }
        else if (cursorPosX == h5x) {
          cursorPosX = h2x;
        }
      }
      delay(150);
    }
    if (leftButton) {
    // if ((rawX < 900 && rawX > 300) && (rawY < 1024 && rawY > 1000)) {
      //left
      if (cursorPosX == h1x) {
        cursorPosX = h3x;
      }
      else if (cursorPosX == h2x) {
        cursorPosX = h1x;
      }
      else if (cursorPosX == h3x) {
        cursorPosX = h2x;
      }
      else if (cursorPosX == h4x) {
        cursorPosX = h5x;
      }
      else if (cursorPosX == h5x) {
        cursorPosX = h4x;
      }
      delay(150);
    }
    if (rightButton) {
    // if ((rawX < 535 && rawX > 500) && (rawY < 100 && rawY >= 0)) {
      //right
      if (cursorPosX == h1x) {
        cursorPosX = h2x;
      }
      else if (cursorPosX == h2x) {
        cursorPosX = h3x;
      }
      else if (cursorPosX == h3x) {
        cursorPosX = h1x;
      }
      else if (cursorPosX == h4x) {
        cursorPosX = h5x;
      }
      else if (cursorPosX == h5x) {
        cursorPosX = h4x;
      }
      delay(150);
    }

    if (cursorPosX == moleX && cursorPosY == moleY) {
      int button = digitalRead(buttonPin);
      if (button == LOW) {
        score++;
        Serial.print("Score: ");
        Serial.println(score);
        moleX = -10;  //start mole off screen
        moleY = -10;
      }
    }
  }
}


void WhackAMole::MoleHole() { //called every 4 seconds
  randomSeed(analogRead(D1));  //use unused analog pin to have random seed
  int randHole = random(1, 5); //number 1-5
  if (randHole == 1) {
    moleX = h1x;
    moleY = hTopRowY;
  }
  else if (randHole == 2) {
    moleX = h2x;
    moleY = hTopRowY;
  }
  else if (randHole == 3) {
    moleX = h3x;
    moleY = hTopRowY;
  }
  else if (randHole == 4) {
    moleX = h4x;
    moleY = hBottomRowY;
  }
  else if (randHole == 5) {
    moleX = h5x;
    moleY = hBottomRowY;
  }
}


#endif