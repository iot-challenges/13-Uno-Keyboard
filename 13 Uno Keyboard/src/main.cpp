/**
 *
 *****************************************************************
 * IOT Challenges - https://instagram.com/iotchallenges          *
 *****************************************************************
 * Evandro Fruhauf                                               *
 *  GitHub - https://github.com/evandrodp                        *
 *  LinkedIn - http://linkedin.com/in/evandrofruhauf             *
 *****************************************************************
 * This code can contain parts from other open-sources projects, *
 * whenever possible it will be quoted.                          *
 *****************************************************************
 *
 * Keyboard
 *
 * Challenge: Use the keyboard and show the value on the led matrix
 *
 * This source is using the Keypad library. (See lib folder)
 * Source base for Matrix: Nerd First https://www.youtube.com/watch?v=NM7wXta8crM
 * Matrix Generator: https://xantorohara.github.io/led-matrix-editor
 *
 * This Letters:
 * https://xantorohara.github.io/led-matrix-editor/#7e1818181c181800|7e060c3060663c00|3c66603860663c00|30307e3234383000|3c6660603e067e00|3c66663e06663c00|1818183030667e00|3c66663c66663c00|3c66607c66663c00|3c66666e76663c00|6666667e66663c00|3e66663e66663e00|3c66060606663c00|3e66666666663e00|006c38fe386c0000|2424ff2424ff2424
 *
**/
#include <Arduino.h>
#include "Keypad.h"

// Define the Registers codes
#define DATA 2
#define SHIFT 3
#define STORE 4

// Functions will be used on the Matrix
void store();
void displayImage(const byte* image);
void clearImage();

// Variables to define the images and the time
byte image[8];
long lastTime = 0;
long timeDelay = 800;

// Collection of images
const byte IMAGES[][8] PROGMEM = {
  {B00000000, B00011000, B00011000, B00111000, B00011000, B00011000, B00011000, B01111110}, // 1
  {B00000000, B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110}, // 2
  {B00000000, B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100}, // 3
  {B00000000, B00111100, B01100110, B01100110, B01111110, B01100110, B01100110, B01100110}, // A
  {B00000000, B00001100, B00011100, B00101100, B01001100, B01111110, B00001100, B00001100}, // 4
  {B00000000, B01111110, B01100000, B01111100, B00000110, B00000110, B01100110, B00111100}, // 5
  {B00000000, B00111100, B01100110, B01100000, B01111100, B01100110, B01100110, B00111100}, // 6
  {B00000000, B01111100, B01100110, B01100110, B01111100, B01100110, B01100110, B01111100}, // B
  {B00000000, B01111110, B01100110, B00001100, B00001100, B00011000, B00011000, B00011000}, // 7
  {B00000000, B00111100, B01100110, B01100110, B00111100, B01100110, B01100110, B00111100}, // 8
  {B00000000, B00111100, B01100110, B01100110, B00111110, B00000110, B01100110, B00111100}, // 9
  {B00000000, B00111100, B01100110, B01100000, B01100000, B01100000, B01100110, B00111100}, // C
  {B00000000, B00000000, B00110110, B00011100, B01111111, B00011100, B00110110, B00000000}, // *
  {B00000000, B00111100, B01100110, B01101110, B01110110, B01100110, B01100110, B00111100}, // 0
  {B00100100, B00100100, B11111111, B00100100, B00100100, B11111111, B00100100, B00100100}, // #
  {B00000000, B01111100, B01100110, B01100110, B01100110, B01100110, B01100110, B01111100}, // D
  {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000}  //
};
const int IMAGES_LEN = sizeof(IMAGES)/8;
// initialise the keyboard
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// define the values on the buttons of the keypads
// These values above are in order with the Images array
char hexaKeys[ROWS][COLS] = {
  {1,   2,  3,  4},
  {5,   6,  7,  8},
  {9,  10, 11, 12},
  {13, 14, 15, 16}
};
byte rowPins[ROWS] = {12, 11, 10, 9}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5}; // connect to the column pinouts of the keypad

// initialise an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  pinMode(DATA, OUTPUT);
  pinMode(SHIFT, OUTPUT);
  pinMode(STORE, OUTPUT);
  clearImage();
}

void loop() {
  int key = myKeypad.getKey();
  if (key) {
    memcpy_P(image, &IMAGES[key-1], 8);
    lastTime = millis();
  }
  if ((millis() - lastTime) > timeDelay) {
    clearImage();
  }
  displayImage(image);
}

void store() {
  digitalWrite(STORE, HIGH);
  delayMicroseconds(10);
  digitalWrite(STORE, LOW);
  delayMicroseconds(10);
}

void displayImage(const byte* image) {
  for (int i = 0; i < 8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, ~image[i]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
  }
}

void clearImage() {
  // Last array is a clear image
  memcpy_P(image, &IMAGES[IMAGES_LEN-1], 8);
}