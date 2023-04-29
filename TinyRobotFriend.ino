#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include <Wire.h>

#include "Snake.h"
#include "SnakeBoard.h"
#include "DirectionalPad.h"
#include "Joke.h"
#include "bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int randomPin = D0;

const int upPin = D7;
const int rightPin = D8;
const int downPin = D9;
const int leftPin = D10;

int upButton;
int rightButton;
int downButton;
int leftButton;

DirectionalPad *dPad;
int *dPadArr;

// used for switching to faces or other screens
bool isIdle = 1; // start as idle (show faces)

bool enterMenu = false; // check to see if index should be zero
int menuIndex = 0;      // TODO: change logic so that index gets reset to 0 when leaving and re-entering menu


//TODO: Add more faces: Add things like sleepy face if left idle for x seconds; Dizzy face if shaken? Seeed board might have thing for that
//      Actually work on the games - Wack a Mole should be easy-ish, no hit button - just hover over the mole to score
//      Snake: look into queue based system for the grid?

void setup()
{
  Serial.begin(9600);
  u8g2.begin();

  dPad = new DirectionalPad();

  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);

  // Clear the buffer
  u8g2.clearBuffer();
  // welcome();
}

void loop()
{
  // Snake test(u8g2);
  // Joke joke(&u8g2);
  
  readDPad();

  if (isIdle)
  {
    // switch control to happyBlinking
    happyBlinking();
  }
  else
  {
    // enterMenu = true;
    menu();
  }
}

void bitmapTest()
{
  u8g2.clearBuffer();
  u8g2.drawXBM(0, 0, 128, 64, happy_closed_eyes);
  u8g2.sendBuffer();
}

void happyBlinking()
{
  // makes the happy face 'blink'
  bool waiting = false;
  int randomMillis;
  unsigned long startTime;
  // start with normal face
  u8g2.clearBuffer();
  u8g2.drawXBM(0, 0, 128, 64, happy);
  u8g2.sendBuffer();
  while (true)
  {
    // see if a button has been pressed
    readDPad();
    if (upButton || rightButton || downButton || leftButton)
    {
      isIdle = false;
      return; // break out of function and return control to main loop()
    }

    if (!waiting)
    {
      randomSeed(analogRead(randomPin)); // use unused analog pin to have random seed
      randomMillis = random(4, 7);
      randomMillis *= 1000; // convert a number 4-7 to seconds format (i.e 5 -> 5000milliseconds)
      waiting = true;
      startTime = millis(); // start count time now to compare to next loop
    }
    else
    {
      if (millis() >= startTime + randomMillis)
      {                                 // if the time right now is x seconds more than when we started then blink
        int doubleBlink = random(3, 5); // 0.25 chance to blink twice
        int blinks;
        (doubleBlink == 4) ? blinks = 2 : blinks = 1;
        for (int i = 0; i < blinks; i++)
        {
          u8g2.clearBuffer();
          u8g2.drawXBM(0, 0, 128, 64, happy_closed_eyes);
          u8g2.sendBuffer();
          delay(150);
        }
        waiting = false; // set to false so next loop we generate a new timer
      }
      else
      { // otherwise keep looping and normal face until time is met
        u8g2.clearBuffer();
        u8g2.drawXBM(0, 0, 128, 64, happy);
        u8g2.sendBuffer();
      }
    }
  }
}

void welcome()
{
  u8g2.clearBuffer();
  // u8g2.setFont(u8g2_font_originalsans_tr);	// choose a suitable font
  u8g2.setFont(u8g2_font_profont10_tf); // choose a suitable font
  u8g2.drawRFrame(0, 0, 128, 64, 4);    // draw border rounded-rectangle
  u8g2.drawStr(5, 10, "Hi! My name is T.R.F");
  u8g2.drawStr(5, 20, "(Tiny Robot Friend)");
  u8g2.drawStr(5, 40, "Press a button to do stuff!");
  u8g2.sendBuffer();

  delay(5000);
}

void menu()
{
  const int yFirstOffset = 10;
  const int yWordOffset = 10;
  const int xFirstOffset = 5;
  const int xWordOffset = 5;

  const int yBoxSpacer = 2;
  const int xBoxSpacer = 3;
  const int boxHeight = 10;
  const int boxWidth = 70;

  // if(enterMenu) menuIndex = 0;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_profont10_tf);

  u8g2.drawRFrame(0, 0, 128, 64, 4); // draw border rounded-rectangle

  u8g2.drawStr(xFirstOffset, yFirstOffset, "Menu"); // 4
  u8g2.drawLine(5, 12, 30, 12);

  u8g2.drawStr(xWordOffset, yFirstOffset + (yWordOffset), "My face");         // 14
  u8g2.drawStr(xWordOffset, yFirstOffset + (yWordOffset * 2), "Wack-A-Mole"); // 24
  u8g2.drawStr(xWordOffset, yFirstOffset + (yWordOffset * 3), "Snake");       // 34
  u8g2.drawStr(xWordOffset, yFirstOffset + (yWordOffset * 4), "Tell me a joke");

  // selection indicator
  if (upButton)
  {
    if (menuIndex > 1)
      menuIndex--;
    delay(100); // TODO: make sure this is okay later lol
  }
  else if (downButton)
  {
    if (menuIndex < 4)
      menuIndex++;
    delay(100);
  }
  if (menuIndex != 0)
  {
    int ybo = yBoxOffset(menuIndex, yFirstOffset, yWordOffset, yBoxSpacer);
    u8g2.drawFrame(xBoxSpacer, ybo, boxWidth, boxHeight);
    // hit "enter" on the selection
    if (rightButton || leftButton)
    {
      switch (menuIndex)
      {
      case 1:
        happyBlinking();
        break;

      case 2:
        wackAMole();
        break;

      case 3:
        snake();
        break;

      case 4:
        jokes();
        break;

      default:
        break;
      }
    }
  }

  // TODO: when leaving menu, set enterMenu to false
  u8g2.sendBuffer();
}

// helper function to get y offset for the selection box (I see why people like lambdas now)
int yBoxOffset(int position, int yFirstOffset, int yWordOffset, int yBoxSpacer)
{
  return (yFirstOffset - 5) + (yWordOffset * position) - yBoxSpacer; // converting to u8g2 messes up the offsets hence the random 5
}

// place holder for entering game, will probably make a class
void wackAMole()
{
}

// very much a WIP lol
void snake()
{
  Snake snakeGame(u8g2);
}

void jokes()
{
   Joke joke(&u8g2);
}

void readDPad()
{
  // I know this is gross i'm sorry
  dPadArr = dPad->read();
  upButton = dPadArr[0];
  rightButton = dPadArr[1];
  downButton = dPadArr[2];
  leftButton = dPadArr[3];
}

/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣶⣿⣿⣷⣶⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣾⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡟⠁⣰⣿⣿⣿⡿⠿⠻⠿⣿⣿⣿⣿⣧⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⠏⠀⣴⣿⣿⣿⠉⠀⠀⠀⠀⠀⠈⢻⣿⣿⣇⠀⠀⠀
⠀⠀⠀⠀⢀⣠⣼⣿⣿⡏⠀⢠⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⡀⠀⠀
⠀⠀⠀⣰⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀
⠀⠀⢰⣿⣿⡿⣿⣿⣿⡇⠀⠘⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⢀⣸⣿⣿⣿⠁⠀⠀
⠀⠀⣿⣿⣿⠁⣿⣿⣿⡇⠀⠀⠻⣿⣿⣿⣷⣶⣶⣶⣶⣶⣿⣿⣿⣿⠃⠀⠀⠀
⠀⢰⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀
⠀⢸⣿⣿⡇⠀⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉⢉⣿⣿⠀⠀⠀⠀⠀⠀
⠀⢸⣿⣿⣇⠀⣿⣿⣿⠀⠀⠀⠀⠀⢀⣤⣤⣤⡀⠀⠀⢸⣿⣿⣿⣷⣦⠀⠀⠀
⠀⠀⢻⣿⣿⣶⣿⣿⣿⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣦⡀⠀⠉⠉⠻⣿⣿⡇⠀⠀
⠀⠀⠀⠛⠿⣿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠈⠹⣿⣿⣇⣀⠀⣠⣾⣿⣿⡇⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣦⣤⣤⣤⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠛⠋⠉⠉⠁⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠁
*/