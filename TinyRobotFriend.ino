/*
* Use 4 direction d-pad, enter/back can be the left and right buttons when they are not
* being used as controls 
* 
* Wack-a-mole: No select/hit button, if you move your cursor to the location in time,
* count it as scoring
*
*/

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

void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);

  pinMode(ledPin, OUTPUT);

  // Clear the buffer
  display.clearDisplay();
  welcome();
}

void loop() {
  digitalWrite(ledPin, HIGH);
	
  readDPad();

  if(isIdle) {
    //switch control to happyBlinking
    happyBlinking();
  }
  else {
    welcome(); //try switching to this just as a test
  }
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
  display.clearDisplay();

  display.drawRoundRect(0, 0, 128, 64, 4, SSD1306_WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(5,10);             // Start at top-left corner
  display.println(F("Hi! My name is T.R.F"));
  display.setCursor(5,20);  
  display.println(F("(Tiny Robot Friend)"));
  display.setCursor(5,40);  
  display.println(F("Press a button to do stuff!"));

  display.display();

  delay(5000);
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