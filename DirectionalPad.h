#ifndef DirectionalPad_H
#define DirectionalPad_H

#include "Arduino.h"

class DirectionalPad
{
private:
    const int upPin = D7;
    const int rightPin = D8;
    const int downPin = D10;
    const int leftPin = D9;

    int upButton = 0;
    int rightButton = 0;
    int downButton = 0;
    int leftButton = 0;

    int values[4];

public:
    DirectionalPad();
    int* read();
    // ~DirectionalPad()=default;
};

DirectionalPad::DirectionalPad()
{
    pinMode(upPin, INPUT);
    pinMode(rightPin, INPUT);
    pinMode(downPin, INPUT);
    pinMode(leftPin, INPUT);
}

int* DirectionalPad::read()
{
    values[0] = digitalRead(upPin);
    values[1] = digitalRead(rightPin);
    values[2] = digitalRead(downPin);
    values[3] = digitalRead(leftPin);
    // Serial.print("Up: ");
    // Serial.print(values[0]);
    // Serial.print(", Right: ");
    // Serial.print(values[1]);
    // Serial.print(", Down: ");
    // Serial.print(values[2]);
    // Serial.print(", Left: ");
    // Serial.print(values[3]);
    // Serial.println();

    return values;
}

#endif