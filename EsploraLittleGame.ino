// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       BlogPost 02 - Arduino Esplorer - Bewegter Punkt
// Author:        Johannes P. Langner
// Controller:    Arduino Esplora
// TFT:           1.8" TFT Module
// Description:   Bewegter Punkt auf dem Esplora
// ========================================================================================

#include <SPI.h>
#include <TFT.h>
#include <Esplora.h>

// ruft die letzte Postion X ab. (Pixel Position)
int lastPosX = 5;
// ruft die letzte Postion Y ab. (Pixel Position)
int lastPosY = 5;

// mittelstellung des Joystick
int offsetX = -3;
int offsetY = 4;

void setup() {
  
 EsploraTFT.begin();
 EsploraTFT.background(0, 0, 0);
}

void loop() {

  // Eingänge einlesen
  int stickX = Esplora.readJoystickX() + (offsetX * -1);
  int stickY = Esplora.readJoystickY() + (offsetY * -1);

  boolean buttonLeft = false;
  boolean buttonRight = false;
  boolean buttonUp = false;
  boolean buttonDown = false;

  if(stickX > 3 || stickX < -3) {
    buttonLeft = stickX > 3;
    buttonRight = stickX < -3;
  }

  if(stickY > 3 || stickY < -3) {
    buttonUp = stickY < -3;
    buttonDown = stickY > 3;
  }

  // Werte ausgeben auf dem Bildschirm
  writeValue(10, 10, stickX, false);
  writeValue(10, 20, stickY, false);

  // Temporaer letzte Position merken
  int lastPosXtemp = lastPosX;
  int lastPosYtemp = lastPosY;

  // Abfragen zu den gesetzten joystick
  // Es kann nur in eine Richtung die Bedingung erfüllt werden.
  
  // Wenn nach links oder rechts gedrückt wird.
  if(buttonLeft && !buttonRight && lastPosX > 0) {
    // nach links und letzte Position Y ist groesser als '0'.
    lastPosX--;
  }
  else if(!buttonLeft && buttonRight && lastPosX < EsploraTFT.width()) {
    // nach rechts und letzte Position X ist kleiner als die TFT Pixel Breite.
    lastPosX++;
  }

  // wenn nach oben oder unten gedrückt wird.
  if(buttonUp && !buttonDown && lastPosY > 0) {
    // nach oben und letzte Position Y ist groesser als '0'.
    lastPosY--;
  }
  else if(!buttonUp && buttonDown && lastPosY < EsploraTFT.height()) {
    // nach unten und letzte Position X ist kleiner als die TFT Pixel hoehe.
    lastPosY++;
  }

  // Wenn sich X oder Y Position unterscheiden, dann den zu bewegenden Punkt neu zeichnen.
  if(lastPosX != lastPosXtemp || lastPosY != lastPosYtemp) {
    // vorigen punkt entfernen mit den temporären Positionen
    drawPoint(lastPosXtemp, lastPosYtemp, false);
    // neuen punkt zeichnen mit der neuen Position.
    drawPoint(lastPosX, lastPosY, true);
  }

  writeValue(10, 10, stickX, true);
  writeValue(10, 20, stickY, true);
}

void writeValue(int x, int y, int val, boolean clr) {
  if(clr) {
    EsploraTFT.stroke(0, 0, 0);
  }
  else {
    EsploraTFT.stroke(30, 200, 50);
  }
  String accResult = String(val);
  char valuePrint[5];
  accResult.toCharArray(valuePrint, 5);
  EsploraTFT.text(valuePrint, x, y);
}

// Einachen Punkt Zeichnen, der nicht ausgefuellt ist.
void drawPoint(int x, int y, boolean setColor) {

  // Schwarz Zeichnen.
  EsploraTFT.stroke(0, 0, 0);

  // Farbe festlegen
  if(setColor) {
    EsploraTFT.stroke(120, 100, 200);
  }
  
  EsploraTFT.circle(x, y, 2);
}
