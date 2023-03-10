/*===================================================================
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the 0.54" 14-segment LED HT16K33
  Backpack found on Aliexpress (Red PCB) 

  These displays use I2C to communicate, 2 pins are required to 
  interface. VI2C pins tied to 3V or 5V depending on you choice.
  There are multiple selectable I2C addresses.
  2 Address Select pins: 0x70, 0x71, 0x72 or 0x73
  3 Address Select pins: 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76 or 0x77

  Written by David C. Boyce -Feb 28 2023.
  YouTube: @PCBoardRepair
  email: pcboardrepair@gmail.com   
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#ifndef _BitValue
  #define _BitValue(bit) (1<<(bit))
#endif

Adafruit_LEDBackpack matrix = Adafruit_LEDBackpack();

uint8_t count = 0;

const unsigned char seg14[] = {
  0x00, 0x00, //Space
  0x40, 0x06, //!
  0x02, 0x20, //"
  0x12, 0xC7, //#
  0x12, 0xED, //$
  0x24, 0x24, //%
  0x0B, 0x59, //&
  0x04, 0x00, //'
  0x00, 0x39, //(
  0x00, 0x0F, //)
  0x3f, 0x00, //*
  0x12, 0xC0, //+
  0x20, 0x00, //,
  0x00, 0xC0, //-
  0x40, 0x00, //.
  0x24, 0x00, ///
  0x00, 0x3F, //0
  0x00, 0x06, //1
  0x00, 0xDB, //2
  0x00, 0xCF, //3
  0x00, 0xE6, //4
  0x00, 0xED, //5
  0x00, 0xFD, //6
  0x00, 0x27, //7
  0x00, 0xFF, //8
  0x00, 0xEF, //9
  0x0C, 0x00, //:
  0x21, 0x00, //;
  0x0C, 0x40, //<
  0x00, 0xC8, //=
  0x21, 0x80, //>
  0x50, 0x83, //?
  0x10, 0xBF, //@
  0x24, 0x86, //A
  0x04, 0xFD, //B
  0x00, 0x3B, //C
  0x12, 0x0F, //D
  0x00, 0xF9, //E
  0x00, 0x71, //F
  0x00, 0xBD, //G
  0x00, 0xF6, //H
  0x12, 0x09, //I
  0x00, 0x1E, //J
  0x0C, 0x70, //K
  0x00, 0x38, //L
  0x05, 0x36, //M
  0x09, 0x36, //N
  0x00, 0x3F, //O
  0x00, 0xF3, //P
  0x08, 0x3F, //Q
  0x08, 0xF3, //R
  0x01, 0x9D, //S
  0x12, 0x01, //T
  0x00, 0x3E, //U
  0x24, 0x30, //V
  0x28, 0x36, //W
  0x09, 0xD2, //X
  0X15, 0X08, //Y
  0x24, 0x09, //Z
  0x21, 0x00, //;
  0x0C, 0x40, //<
  0x00, 0xC8, //=
  0x21, 0x80, //>
  0x50, 0x83, //?
  0x10, 0xBF, //@
  0x30, 0x48, //a
  0x12, 0x8C, //b
  0x00, 0x58, //c
  0x12, 0x58, //d
  0x20, 0x58, //e
  0x14, 0xC0, //f
  0x04, 0x8E, //g
  0x12, 0x84, //h
  0x14, 0x00, //i
  0x04, 0x0C, //j
  0x1E, 0x00, //k
  0x32, 0x00, //l
  0x10, 0xD4, //m
  0x10, 0xC4, //n
  0x00, 0xDC, //o
  0x12, 0x83, //p
  0x12, 0x61, //q
  0x11, 0x80, //r
  0x08, 0x88, //s
  0x0A, 0xC0, //t
  0x10, 0x18, //u
  0x20, 0x10, //v
  0x28, 0x14, //w
  0x2C, 0x40, //x
  0x25, 0x00, //y
  0x20, 0x48, //z
  0x12, 0x49, //{
  0x12, 0x00, //|
  0x12, 0x89, //}
  0x00, 0x01, //~
};

#define DIGIT_14SEG 4
unsigned long int disp_buff[DIGIT_14SEG];

void str_display(char *str) {
  int i;
  char ch;
  for (i=0; i<DIGIT_14SEG; i++) {
    ch = (str[DIGIT_14SEG-i-1]-' ');
    matrix.displaybuffer[DIGIT_14SEG-i-1] = ((int)seg14[ch*2]<<8) + seg14[ch*2+1];
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("HT16K33 test");
  
  matrix.begin(0x70);  // pass in the address

  str_display("TEST"); delay(500);
  matrix.writeDisplay();

  delay(5000);

}

void loop() {
  while(count <= 16){
    //Paint one LED per row.
    //The HT16K33 internal memory looks like 8x16 bit matrix (8 rows, 16 columns)
    for (uint8_t i=0; i<4; i++) {
      matrix.displaybuffer[i] = _BitValue(count) ;
    }
    //Write the changes to the display
    matrix.writeDisplay();
    delay(250);
    if (count >= 15) {count = 0; break; } else { count++; }
  }

  for (uint8_t c=0; c<95; c++){
    for (uint8_t i=0; i<4; i++) {
        matrix.displaybuffer[DIGIT_14SEG-i-1] = ((int)seg14[c*2]<<8) + seg14[c*2+1];
    }
    //Write the changes to the display
    matrix.writeDisplay();
    delay(500);
  }

}
