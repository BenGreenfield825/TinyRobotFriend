

/*
* Use 4 direction d-pad, enter/back can be the left and right buttons when they are not
* being used as controls 
* 
* Wack-a-mole: No select/hit button, if you move your cursor to the location in time,
* count it as scoring
*
*/


#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

const int randomPin = 3;

const int upPin = 13;
const int rightPin = 12;
const int downPin = 11;
const int leftPin = 10;

const int ledPin = 9;

int upButton;
int rightButton;
int downButton;
int leftButton;

//used for switching to faces or other screens
bool isIdle = 1; //start as idle (show faces)

bool enterMenu = false; //check to see if index should be zero
int menuIndex = 0; //TODO: change logic so that index gets reset to 0 when leaving and re-entering menu

void setup() {
  Serial.begin(9600);
  
  u8g2.begin();

  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);

  pinMode(ledPin, OUTPUT);

  // Clear the buffer
  u8g2.clearBuffer();
  welcome();
}

void loop() {
  // displayPixelTest();
  digitalWrite(ledPin, HIGH);
	
  // readDPad();

  // if(isIdle) {
  //   //switch control to happyBlinking
  //   happyBlinking();
  // }
  // else {
  //   // enterMenu = true;
  //   menu(); //try switching to this just as a test
  // }
}

void happyBlinking() {
  //makes the happy face 'blink'
  bool waiting = false;
  int randomMillis;
  unsigned long startTime;
  //start with normal face
  display.clearDisplay();
  display.drawBitmap(0, 0, happy, 128, 64, 1);
  display.display();
  while(true) {
    upButton = digitalRead(upPin);
    rightButton = digitalRead(rightPin);
    downButton = digitalRead(downPin);
    leftButton = digitalRead(leftPin);
    //see if a button has been pressed
    if(upButton || rightButton || downButton || leftButton) {
      isIdle = false;
      return; //break out of function and return control to main loop()
    }

    if(!waiting) {
      randomSeed(analogRead(randomPin));  //use unused analog pin to have random seed
      randomMillis = random(4, 7);
      randomMillis *= 1000; //convert a number 4-7 to seconds format (i.e 5 -> 5000milliseconds)
      waiting = true;
      startTime = millis(); //start count time now to compare to next loop
    }
    else {
      if(millis() >= startTime + randomMillis) {  //if the time right now is x seconds more than when we started then blink
        int doubleBlink = random(3,5); //0.25 chance to blink twice
        int blinks;
        (doubleBlink == 4) ? blinks = 2 : blinks = 1;
        for(int i=0; i < blinks; i++) {
          display.clearDisplay();
          display.drawBitmap(0, 0, happy_closed_eyes, 128, 64, 1);
          display.display();
          //TODO: double blinking is messed up, trying to do another millis eventually breaks it
          delay(150);
          // int blinkingTime = millis();
          // while(millis() <= blinkingTime + 250) {}
          // display.clearDisplay();
          // display.drawBitmap(0, 0, happy, 128, 64, 1);
          // display.display();
        }
        waiting = false; //set to false so next loop we generate a new timer
      }
      else{ //otherwise keep looping and normal face until time is met
        display.clearDisplay();
        display.drawBitmap(0, 0, happy, 128, 64, 1);
        display.display();
      }     
    }
  }
}

void happyBlinkingOLD() {
  //makes the happy face 'blink'
  randomSeed(analogRead(randomPin));  //use unused analog pin to have random seed
  display.clearDisplay();
  display.drawBitmap(0, 0, happy, 128, 64, 1);
  display.display();
  delay(random(4000, 7000));  //blink every 4-7 seconds
  display.clearDisplay();
  display.drawBitmap(0, 0, happy_closed_eyes, 128, 64, 1);
  display.display();
  delay(250);
  int doubleBlink = random(1,5);
  Serial.println(doubleBlink);
  if(doubleBlink == 4) {  //0.25 chance to blink twice
    display.clearDisplay();
    display.drawBitmap(0, 0, happy, 128, 64, 1);
    display.display();
    delay(250);
    display.clearDisplay();
    display.drawBitmap(0, 0, happy_closed_eyes, 128, 64, 1);
    display.display();
    delay(250);
  }
}

void welcome() {
  u8g2.clearBuffer();	
  // u8g2.setFont(u8g2_font_originalsans_tr);	// choose a suitable font
  u8g2.setFont(u8g2_font_4x6_mf);	// choose a suitable font
  u8g2.drawRFrame(0,0,128,64,4);
  u8g2.drawStr(5,10,"Hi! My name is T.R.F");
  u8g2.drawStr(5,20,"(Tiny Robot Friend)");
  u8g2.drawStr(5,40,"Press a button to do stuff!");
  u8g2.sendBuffer();

  delay(5000);
}

void menu() {
  const int yFirstOffset = 4;
  const int yWordOffset = 10;
  const int xFirstOffset = 5;
  const int xWordOffset = 5;

  const int yBoxSpacer = 2;
  const int xBoxSpacer = 3;
  const int boxHeight = 11;
  const int boxWidth = 70;
  
  // if(enterMenu) menuIndex = 0;

  display.clearDisplay();

  display.drawRoundRect(0, 0, 128, 64, 4, SSD1306_WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  
  display.setCursor(xFirstOffset, yFirstOffset); //4
  display.println(F("Menu"));
  display.drawLine(5, 12, 30, 12, SSD1306_WHITE);

  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset)); //14
  display.println(F("My face"));

  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset * 2)); //24
  display.println(F("Wack-A-Mole"));

  display.setCursor(xWordOffset, yFirstOffset + (yWordOffset * 3)); //34
  display.println(F("Snake"));

  //selection indicator
  if(upButton) {
    if(menuIndex > 1) menuIndex--;
    delay(100); //TODO: make sure this is okay later lol
  }
  else if (downButton) {
    if(menuIndex < 3) menuIndex++;
    delay(100);
  }
  if(menuIndex != 0) {
    int ybo = yBoxOffset(menuIndex, yFirstOffset, yWordOffset, yBoxSpacer);
    display.drawRect(xBoxSpacer, ybo, boxWidth, boxHeight, SSD1306_WHITE);
    //hit "enter" on the selection
    if(rightButton) {
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
      
      default:
        break;
      }
    }
  }
  

  //TODO: when leaving menu, set enterMenu to false

  display.display();
}

int yBoxOffset(int position, int yFirstOffset, int yWordOffset, int yBoxSpacer) {
  return yFirstOffset + (yWordOffset * position) - yBoxSpacer;
}

//place holder for entering game, will probably make a class
void wackAMole() {
  display.clearDisplay();
  display.drawCircle(64, 32, 5, SSD1306_WHITE);
  display.display();
}

//place holder for entering game, will probably make a class
void snake() {
  display.clearDisplay();
  display.drawRect(64, 32, 5, 10, SSD1306_WHITE);
  display.display();
}

void readDPad() {
  upButton = digitalRead(upPin);
  rightButton = digitalRead(rightPin);
  downButton = digitalRead(downPin);
  leftButton = digitalRead(leftPin);
  Serial.print("Up: ");
  Serial.print(upButton);
  Serial.print(", Right: ");
  Serial.print(rightButton);
  Serial.print(", Down: ");
  Serial.print(downButton);
  Serial.print(", Left: ");
  Serial.print(leftButton);
  Serial.println();
}

void displayPixelTest() {
  display.clearDisplay();
  for(int i=0; i <= SCREEN_HEIGHT; i++) {
    display.drawPixel(10, i, SSD1306_WHITE);
    display.drawPixel(15, ++i, SSD1306_WHITE);
    display.display();
  }
}
