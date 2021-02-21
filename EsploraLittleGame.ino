// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Arduino - Bewegter Punkt
// Author:        Johannes P. Langner
// Controller:    Arduino
// Sensor:        4 Buttons
// TFT:           1.8" TFT Module
// Description:   Einen Punkt bewegen
// ========================================================================================

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Verkabelung / Pins
// SCLK     => D13
// MOSI/DIN => D11
// CS       => D10
// DC       => D9
// RST      => D8

// instanz anlegen zu dem verwendeten Bildschirm.
Adafruit_ST7735 tft = Adafruit_ST7735(10, 9, 8); 

// Pins festlegen
// ruft die Pin Nummer ab, um die helligkeit des Displays anzusteuern.
const int pinBrightness = 5;
// ruft die Pin Nummer ab, um den status des linken Buttons abzufragen
const int pinButtonLeft = 6;
// ruft die Pin Nummer ab, um den status des rechten Buttons abzufragen
const int pinButtonRight = 3;
// ruft die Pin Nummer ab, um den status des oberen Buttons abzufragen
const int pinButtonUp = 2;
// ruft die Pin Nummer ab, um den status des unteren Buttons abzufragen
const int pinButtonDown = 7;

// ruft die letzte Postion X ab. (Pixel Position)
int lastPosX = 5;
// ruft die letzte Postion Y ab. (Pixel Position)
int lastPosY = 5;

void setup() {
  
  // initalisiere display
  tft.initR(INITR_BLACKTAB);

  // pin Eingänge festlegen
  pinMode(pinButtonLeft, INPUT);
  pinMode(pinButtonRight, INPUT);
  pinMode(pinButtonUp, INPUT);
  pinMode(pinButtonDown, INPUT);

  // pin für tft helligkeit festlegen
  pinMode(pinBrightness, OUTPUT);
  // helligkeit auf maximal einstellen
  analogWrite(pinBrightness, 255);

  // Bildschirm Schwarzausfüllen und um 90 Grad drehen.
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
}

void loop() {

  // Eingänge einlesen
  boolean buttonLeft = digitalRead(pinButtonLeft);
  boolean buttonRight = digitalRead(pinButtonRight);
  boolean buttonUp = digitalRead(pinButtonUp);
  boolean buttonDown = digitalRead(pinButtonDown);

  // Temporaer letzte Position merken
  int lastPosXtemp = lastPosX;
  int lastPosYtemp = lastPosY;

  // Abfragen zu den gedrückten Buttons
  // Es kann nur in eine Richtung die Bedingung erfüllt werden.
  
  // Wenn nach links oder rechts gedrückt wird.
  if(buttonLeft && !buttonRight && lastPosX > 0) {
    // nach links und letzte Position Y ist groesser als '0'.
    lastPosX--;
  }
  else if(!buttonLeft && buttonRight && lastPosX < tft.width()) {
    // nach rechts und letzte Position X ist kleiner als die TFT Pixel Breite.
    lastPosX++;
  }

  // wenn nach oben oder unten gedrückt wird.
  if(buttonUp && !buttonDown && lastPosY > 0) {
    // nach oben und letzte Position Y ist groesser als '0'.
    lastPosY--;
  }
  else if(!buttonUp && buttonDown && lastPosY < tft.height()) {
    // nach unten und letzte Position X ist kleiner als die TFT Pixel hoehe.
    lastPosY++;
  }

  // Wenn sich X oder Y Position unterscheiden, dann den zu bewegenden Punkt neu zeichen.
  if(lastPosX != lastPosXtemp || lastPosY != lastPosYtemp) {
    // vorrigen punkt entfernen mit den temporaeren positionen.
    drawPoint(lastPosXtemp, lastPosYtemp, false);
    // neuen punkt zeichnen mit der neuen Position.
    drawPoint(lastPosX, lastPosY, true);
  }
}

// Einachen Punkt Zeichnen, der nicht ausgefuellt ist.
void drawPoint(int x, int y, boolean setColor) {
  
  int setupColor = ST7735_BLACK;
  // farbe festlegen
  if(setColor) {
    setupColor = ST7735_CYAN;
  }
  tft.drawCircle(x, y, 2, setupColor);
  delay(2);
}
