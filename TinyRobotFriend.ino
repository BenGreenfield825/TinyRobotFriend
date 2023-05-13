#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>

#include <Adafruit_NeoPixel.h>

#include "Snake.h"
#include "SnakeBoard.h"
#include "DirectionalPad.h"
#include "Joke.h"
#include "bitmaps.h"

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int Power = 11;
int PIN = 12;
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
unsigned long idleStartTime = 0;

bool enterMenu = false; // check to see if index should be zero
int menuIndex = 0;      // TODO: change logic so that index gets reset to 0 when leaving and re-entering menu

// TODO: Add more faces: Add things like sleepy face if left idle for x seconds; Dizzy face if shaken? Seeed board might have thing for that
//       Actually work on the games - Wack a Mole should be easy-ish, no hit button - just hover over the mole to score
//       Snake: look into queue based system for the grid?
//       LED: Seeed board has nice bright LED, let use it

void setup()
{
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();

  dPad = new DirectionalPad();

  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);

  pixels.begin();
  pinMode(Power, OUTPUT);
  digitalWrite(Power, HIGH);
  idleStartTime = millis();
}

void loop()
{
  //TODO: this actually does kinda work, but only when you press a button since control is happening in happyBlinking(), I think
  unsigned long currentMillis = millis();
  if (currentMillis >= idleStartTime +1)
  {
    pixels.setPixelColor(0, pixels.Color(150, 0, 0)); //red
    pixels.setBrightness(10);
    pixels.show();
  }
  else
  {
    pixels.setPixelColor(0, pixels.Color(0, 150, 0)); //green
    pixels.setBrightness(10);
    pixels.show();
  }

  // Snake test(u8g2);
  // Joke joke(&display);

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
  display.clearDisplay();
  display.drawBitmap(0, 0, happy, 128, 64, 1);
  display.display();
}

void happyBlinking()
{
  // makes the happy face 'blink'
  bool waiting = false;
  int randomMillis;
  unsigned long startTime;
  // start with normal face
  display.clearDisplay();
  display.drawBitmap(0, 0, happy, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.display();
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
          display.clearDisplay();
          display.drawBitmap(0, 0, happy_closed_eyes, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
          display.display();
          delay(150);
        }
        waiting = false; // set to false so next loop we generate a new timer
      }
      else
      { // otherwise keep looping and normal face until time is met
        display.clearDisplay();
        display.drawBitmap(0, 0, happy, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        display.display();
        ;
      }
    }
  }
}

void menu()
{
  const int yFirstOffset = 5;
  const int yWordOffset = 10;
  const int xFirstOffset = 5;
  const int xWordOffset = 5;

  const int yBoxSpacer = 4;
  const int xBoxSpacer = 3;
  const int boxHeight = 10;
  const int boxWidth = 86;

  // if(enterMenu) menuIndex = 0;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
  display.setCursor(xFirstOffset, yFirstOffset);
  display.write("Menu");
  display.drawLine(5, 12, 30, 12, 1);

  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset));
  display.write("My face");
  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset * 2));
  display.write("Wack-A-Mole");
  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset * 3));
  display.write("Snake");
  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset * 4));
  display.write("Tell me a joke");

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
    display.drawRect(xBoxSpacer, ybo, boxWidth, boxHeight, 1);
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

  // TODO: when leaving menu, set enterMenu to false?
  display.display();
}

// helper function to get y offset for the selection box (I see why people like lambdas now)
int yBoxOffset(int position, int yFirstOffset, int yWordOffset, int yBoxSpacer)
{
  return (yFirstOffset + 2) + (yWordOffset * position) - yBoxSpacer; // idry I add the first offset but i need it whoops
}

// place holder for entering game, will probably make a class
void wackAMole()
{
}

// very much a WIP lol
void snake()
{
  //   Snake snakeGame(u8g2);
}

void jokes()
{
  Joke joke(&display);
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
