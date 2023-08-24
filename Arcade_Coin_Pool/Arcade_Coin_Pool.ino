#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "stdlib.h"
#include <Wire.h>
#include <avr/interrupt.h>

// Coin Mech on Pin 27 PC4 DIO A4  Output goes High to Add to Credit Pool
// P1 Credit Allocation on Pin 10 PD6 DIO 6
// P1 Credit Input to RPi on Pin 25 PC2 DIO A2 Output goes High to Apply Credit
// P2 Credit Allocation on Pin 11 PD7 DIO 7
// P2 Credit Input to RPi on Pin 26 PC3 DIO A3 Output goes High to Apply Credit
// P3 Credit Allocation on Pin 13 PB1 DIO 9
// P3 Credit Input to RPi on Pin 12 PB0 DIO 8 Output goes High to Apply Credit
// P4 Credit Allocation on Pin 28 PC5 DIO A5
// P4 Credit Input to RPi on Pin 9 PD5 DIO 5 Output goes High to Apply Credit

// assigned Pins ATMega 328P TQFP
int coinMech = 18;
int PlayC1A = 17;
int PlayI1 = 8;
int PlayC2A = 16;
int PlayI2 = 7;
int PlayC3A = 15;
int PlayI3 = 6;
int PlayC4A = 19;
int PlayI4 = 5;

//const
int coins = 0;

//variables
int timer1_match;


//boolean
volatile bool creditState = false;
volatile bool twoPlayer = false;
volatile bool playerState1 = false;
volatile bool playerState2 = false;
volatile bool playerState3 = false;
volatile bool playerState4 = false;

volatile char tempStore1[10];

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);


void setup(void) {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  timer1_match = 10;
  TCNT1 = timer1_match;
  TCCR1B |= (1 << CS11);
  //TCCR1B |= (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
//Serial.begin(9600);

//Pin Allocations
  pinMode(coinMech, INPUT_PULLUP);
  pinMode(PlayC1A, INPUT_PULLUP);
  pinMode(PlayC2A, INPUT_PULLUP);
  pinMode(PlayC3A, INPUT_PULLUP);
  pinMode(PlayC4A, INPUT_PULLUP);
  pinMode(PlayI1, OUTPUT);
  pinMode(PlayI2, OUTPUT);
  pinMode(PlayI3, OUTPUT);
  pinMode(PlayI4, OUTPUT);
  if(digitalRead(PlayC4A) == HIGH)
  {
    twoPlayer = !twoPlayer;
  }
  u8g2.begin();
}

void loop(void) {
  u8g2.firstPage();
  do {
    dtostrf(coins, 2, 0, tempStore1);
    u8g2.setFont(u8g2_font_fub14_tf);
    u8g2.drawStr(20,25,"Coin Pool");
    if(twoPlayer != HIGH)
    {
      u8g2.drawStr(2,50,"Credits: $");
      u8g2.drawStr(98,50,tempStore1);
    }
    else
    {
      u8g2.drawStr(25,50,"Disabled");
    }
    
  } while ( u8g2.nextPage() );
}
ISR(TIMER1_COMPA_vect)
{
  TCNT1 = timer1_match;
  if(digitalRead(coinMech) == HIGH && creditState == LOW && twoPlayer == LOW)
  {
    creditState = !creditState;
    coins++;
  }
  if(digitalRead(coinMech) == LOW && creditState == HIGH && twoPlayer == LOW)
  {
    creditState = !creditState;
  }
  if(digitalRead(coinMech) == HIGH && creditState == LOW && twoPlayer == HIGH)
  {
    creditState = !creditState;
    digitalWrite(PlayI1, HIGH);
  }
  if(digitalRead(coinMech) == LOW && creditState == HIGH && twoPlayer == HIGH)
  {
    creditState = !creditState;
    digitalWrite(PlayI1, LOW);
  }
  if(digitalRead(PlayC4A) == HIGH && playerState4 == LOW)
  {
    playerState4 = !playerState4;
    if (coins!=0)
    {
      coins--;
      digitalWrite(PlayI4, HIGH);
    }
  }
  if(digitalRead(PlayC4A) == LOW && playerState4 == HIGH)
  {
    digitalWrite(PlayI4, LOW);
    playerState4 = !playerState4;
  }
  
  if(digitalRead(PlayC3A) == HIGH && playerState3 == LOW)
  {
    playerState3 = !playerState3;
    if (coins!=0)
    {
      coins--;
      digitalWrite(PlayI3, HIGH);
    }
  }
  if(digitalRead(PlayC3A) == LOW && playerState3 == HIGH)
  {
    digitalWrite(PlayI3, LOW);
    playerState3 = !playerState3;
  }

  if(digitalRead(PlayC2A) == HIGH && playerState2 == LOW)
  {
    playerState2 = !playerState2;
    if (coins!=0)
    {
      coins--;
      digitalWrite(PlayI2, HIGH);
    }
  }
  if(digitalRead(PlayC2A) == LOW && playerState2 == HIGH)
  {
    digitalWrite(PlayI2, LOW);
    playerState2 = !playerState2;
  }

  if(digitalRead(PlayC1A) == HIGH && playerState1 == LOW)
  {
    playerState1 = !playerState1;
    if (coins!=0)
    {
      coins--;
      digitalWrite(PlayI1, HIGH);
    }
  }
  if(digitalRead(PlayC1A) == LOW && playerState1 == HIGH)
  {
    digitalWrite(PlayI1, LOW);
    playerState1 = !playerState1;
  }
}