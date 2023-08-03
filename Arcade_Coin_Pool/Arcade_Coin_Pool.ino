#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "stdlib.h"
#include <Wire.h>
#include <avr/interrupt.h>
// Coin Mech on Pin 12 PB0 DIO 8 Output goes High to Add to Credit Pool
// P1 Credit Allocation on Pin 10 PD6 DIO 6
// P1 Credit Input to RPi on Pin 25 PC2 DIO A2 Output goes High to Apply Credit
// P2 Credit Allocation on Pin 11 PD7 DIO 7
// P2 Credit Input to RPi on Pin 26 PC3 DIO A3 Output goes High to Apply Credit
// P3 Credit Allocation on Pin 13 PB1 DIO 9
// P3 Credit Input to RPi on Pin 27 PC4 DIO A4 Output goes High to Apply Credit
// P4 Credit Allocation on Pin 9 PD5 DIO 5
// P4 Credit Input to RPi on Pin 28 PC5 DIO A5 Output goes High to Apply Credit

// assigned Pins ATMega 328P TQFP
int coinMech = 8;
int PlayC1A = 6;
int PlayI1 = A2;
int PlayC2A = 7;
int PlayI2 = A3;
int PlayC3A = 9;
int PlayI3 = A4;
int PlayC4A = 5;
int PlayI4 = A5;

//const
//int coins = 0;
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;

//variables
int coins;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled


volatile char tempStore1[10];

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

void setup(void) {
  cli();
  PCICR |=0b00000101;
  PCMSK0 |= 0b00000011;
  PCMSK2 |= 0b11100000;
  sei();
Serial.begin(9600);
  //Pin Allocations
  pinMode(coinMech, INPUT); //PCINT0
  pinMode(PlayC1A, INPUT); //PCINT22
  pinMode(PlayC2A, INPUT); //PCINT23
  pinMode(PlayC3A, INPUT); //PCINT1
  pinMode(PlayC4A, INPUT); //PCINT21
  pinMode(PlayI1, OUTPUT);
  pinMode(PlayI2, OUTPUT);
  pinMode(PlayI3, OUTPUT);
  pinMode(PlayI4, OUTPUT);

  //Interupt Setup
//attachInterrupt(digitalPinToInterrupt(coinMech), credit, RISING);
attachInterrupt(digitalPinToInterrupt(PlayC1A), Player1, CHANGE);
attachInterrupt(digitalPinToInterrupt(PlayC2A), Player2, CHANGE);
attachInterrupt(digitalPinToInterrupt(PlayC3A), Player3, CHANGE);
attachInterrupt(digitalPinToInterrupt(PlayC4A), Player4, CHANGE);

  u8g2.begin();
  coins = 0;
}

void loop(void) {
  u8g2.firstPage();
  do {
    dtostrf(coins, 2, 0, tempStore1);
    u8g2.setFont(u8g2_font_fub14_tf);
    u8g2.drawStr(20,25,"Coin Pool");
    u8g2.drawStr(2,50,"Credits: $");
    u8g2.drawStr(98,50,tempStore1);
  } while ( u8g2.nextPage() );
  Serial.println(coins);
}
ISR(PCINT0_vect)
{
  coins++;
}

void Player1()
{
  int reading = digitalRead(PlayC1A);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ( (millis() - lastDebounceTime) > debounceDelay) 
  { 
    if (reading != buttonState) {
      buttonState = reading;

      
      if (buttonState == LOW && coins >= 1) {
        digitalWrite(PlayI1, HIGH);
        coins--;
      }
      else
      {
        digitalWrite(PlayI1, LOW);
      }
    lastDebounceTime = millis();
    }
  }
}

void Player2()
{
  int reading = digitalRead(PlayC1A);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ( (millis() - lastDebounceTime) > debounceDelay) 
  { 
    if (reading != buttonState) {
      buttonState = reading;

      
      if (buttonState == LOW && coins >= 1) {
        digitalWrite(PlayI2, HIGH);
        coins--;
      }
      else
      {
        digitalWrite(PlayI2, LOW);
      }
    lastDebounceTime = millis();
    }
  }
}

void Player3()
{
  int reading = digitalRead(PlayC1A);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ( (millis() - lastDebounceTime) > debounceDelay) 
  { 
    if (reading != buttonState) {
      buttonState = reading;

      
      if (buttonState == LOW && coins >= 1) {
        digitalWrite(PlayI3, HIGH);
        coins--;
      }
      else
      {
        digitalWrite(PlayI3, LOW);
      }
    lastDebounceTime = millis();
    }
  }
}

void Player4()
{
  int reading = digitalRead(PlayC1A);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ( (millis() - lastDebounceTime) > debounceDelay) 
  { 
    if (reading != buttonState) {
      buttonState = reading;

      
      if (buttonState == LOW && coins >= 1) {
        digitalWrite(PlayI4, HIGH);
        coins--;
      }
      else
      {
        digitalWrite(PlayI4, LOW);
      }
    lastDebounceTime = millis();
    }
  }
}