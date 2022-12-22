#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Images.h"

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
}

void loop() {
  digitalWrite(ledPin, HIGH);
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
//  bitmapTest();
//   happyBlinking();
}

void bitmapTest() {
  display.clearDisplay();
  display.drawBitmap(0, 0, happy_closed_eyes, 128, 64, 1);
  display.display();
  delay(1000);
  display.drawBitmap(0, 0, happy, 128, 64, 1);
  display.display();
  delay(1000);
}

void happyBlinking() {
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

void angryBlinking() {
  //makes the happy face 'blink'
  randomSeed(analogRead(randomPin));  //use unused analog pin to have random seed
  display.clearDisplay();
  display.drawBitmap(0, 0, angry, 128, 64, 1);
  display.display();
  delay(random(4000, 7000));  //blink every 4-7 seconds
  display.clearDisplay();
  display.drawBitmap(0, 0, angry_closed_eyes, 128, 64, 1);
  display.display();
  delay(250);
  int doubleBlink = random(1,5);
  Serial.println(doubleBlink);
  if(doubleBlink == 4) {  //0.25 chance to blink twice
    display.clearDisplay();
    display.drawBitmap(0, 0, angry, 128, 64, 1);
    display.display();
    delay(250);
    display.clearDisplay();
    display.drawBitmap(0, 0, angry_closed_eyes, 128, 64, 1);
    display.display();
    delay(250);
  }
}
