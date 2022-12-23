#include <TimerOne.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
const int buttonPin = 2;
const int Xpin = A0;
const int Ypin = A1;
int hTopRowY = 10;
int hBottomRowY = 20;
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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CURSOR_H 8
#define CURSOR_W 8
const unsigned char cursorImg[] PROGMEM = {
  B00111100,
  B01011010,
  B10011001,
  B11111111,
  B11111111,
  B10011001,
  B01011010,
  B00111100
};
#define MOLE 8
const unsigned char moleMap[] PROGMEM = {
  B10100101,
  B01000010,
  B10100101,
  B00000000,
  B11111111,
  B10000001,
  B01000010,
  B00111000
};

void setup() {
  Serial.begin(9600);
  pinMode(Xpin, INPUT);
  pinMode(Ypin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Timer1.initialize(4e+6);
  Timer1.attachInterrupt(MoleHole); // Call MoleHole to decide what hole every 4 seconds
  cursorPosX = h1x; //where cursor should start
  cursorPosY = hTopRowY;
  moleX = -10;  //start mole off screen
  moleY = -10;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

}

void loop() {
  int rawX = analogRead(Xpin);
  int rawY = analogRead(Ypin);

  Holes();

  if ((rawX < 1024 && rawX > 1020) && (rawY < 900 && rawY > 500)) {
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
    delay(250);
  }
  if ((rawX < 100 && rawX >= 0) && (rawY < 900 && rawY > 500)) {
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
    delay(250);
  }
  if ((rawX < 900 && rawX > 300) && (rawY < 1024 && rawY > 1000)) {
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
    delay(250);
  }
  if ((rawX < 535 && rawX > 500) && (rawY < 100 && rawY >= 0)) {
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
    delay(250);
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

void Holes() {
  display.clearDisplay();
  //draw holes
  display.fillCircle(h1x, hTopRowY, 6, SSD1306_INVERSE);
  display.fillCircle(h2x, hTopRowY, 6, SSD1306_INVERSE);
  display.fillCircle(h3x, hTopRowY, 6, SSD1306_INVERSE);
  display.fillCircle(h4x, hBottomRowY, 6, SSD1306_INVERSE);
  display.fillCircle(h5x, hBottomRowY, 6, SSD1306_INVERSE);
  //draw cursor and mole
  display.drawBitmap(cursorPosX - cursorOffsetX, cursorPosY - cursorOffsetY, cursorImg, CURSOR_W, CURSOR_H, SSD1306_INVERSE); //cursor
  display.drawBitmap(moleX - cursorOffsetX, moleY - cursorOffsetY, moleMap, MOLE, MOLE, SSD1306_INVERSE); //mole
  //draw score on screen
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 25);     
  display.print(F("S: "));
  display.print(score);
  display.display();
}
void MoleHole() { //called every 4 seconds
  randomSeed(analogRead(3));  //use unused analog pin to have random seed
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