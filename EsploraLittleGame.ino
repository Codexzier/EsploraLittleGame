// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Use flash memory
// Author:        Johannes P. Langner
// Controller:    Arduino Esplora
// TFT:           1.8" TFT Module
// Description:   verwendung des Flash Speichers
// ========================================================================================

#include <SPI.h>
#include <TFT.h>
#include <Esplora.h>
#include <avr/pgmspace.h>

// ruft die letzte Postion X ab. (Pixel Position)
int lastPosX = 16;
// ruft die letzte Postion Y ab. (Pixel Position)
int lastPosY = 16;

int directionX = 0;
int directionY = 0;

// sprite byte array
byte tempArray[160];

// TODO: Wird ersetz durch die System Zeit
// die Spielinterval
long gameTime = 0;

// mittelstellung des Joystick
int offsetX = -3;
int offsetY = 4;
byte maxDeathbandX = 5;
byte maxDeathbandY = 5;

// Figur Sprites load from flash
const PROGMEM byte spriteFigureFrontLeft[160] = {
  0,0,1,1,1,1,1,1,0,0,0,1,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,1,1,3,3,4,4,2,4,3,3,1,1,3,5,5,2,2,5,5,3,1,1,3,4,1,2,2,1,4,3,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,1,0,0,1,8,8,3,3,8,8,6,1,1,8,8,8,8,8,8,6,2,1,1,2,1,8,8,8,8,1,1,0,0,1,1,10,10,7,7,1,0,0,0,1,9,10,1,7,6,1,0,0,0,0,1,1,1,7,6,1,0,0,0,0,0,0,1,9,11,1,0,0,0,0,0,0,0,1,1,0,0,0
  };

const PROGMEM byte spriteFigureFrontMiddle[160] = {
    0,0,1,1,1,1,1,1,0,0,0,1,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,1,1,3,3,4,2,4,4,3,3,1,1,3,5,5,2,2,5,5,3,1,1,3,4,1,2,2,1,4,3,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,0,0,0,1,8,8,3,3,8,8,1,0,1,8,6,8,8,8,8,6,8,1,1,2,1,8,8,8,8,1,2,1,0,1,1,7,7,7,7,1,1,0,0,0,1,6,7,7,6,1,0,0,0,0,1,6,7,7,6,1,0,0,0,0,1,9,10,10,9,1,0,0,0,0,0,1,1,1,1,0,0,0
  };
  
const PROGMEM byte spriteFigureSideLeft[160] = {
  0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,0,1,3,8,8,8,1,0,0,0,0,1,8,8,8,6,1,1,0,0,1,2,8,8,8,7,1,1,0,0,1,7,7,1,7,7,1,0,0,0,0,1,1,10,6,6,1,0,0,0,1,10,10,1,6,10,11,1,0,0,1,1,1,1,1,10,9,1,0,0,0,0,0,0,0,1,1,0,0
  };

const PROGMEM byte spriteFigureSideMiddle[160] = {
    0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,0,1,1,3,8,8,1,0,0,0,0,0,1,8,8,6,1,0,0,0,0,0,1,8,8,6,1,0,0,0,0,0,1,2,7,1,1,0,0,0,0,0,1,1,10,1,1,0,0,0,0,0,1,10,10,10,1,0,0,0,0,0,1,9,9,1,1,0,0,0,0,0,1,1,1,1,1,0,0
  };

const PROGMEM byte spriteFigureSideRight[160] = {
    0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,1,1,3,8,8,8,1,0,0,1,2,1,8,8,8,6,1,1,0,1,1,1,8,8,1,7,6,1,0,0,0,1,7,1,1,7,2,1,0,0,0,1,1,10,6,1,1,1,0,0,1,11,10,10,1,9,9,1,0,0,1,9,9,9,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0
  };

const PROGMEM byte spriteFigureBackLeft[160] = {
    0,0,1,1,1,1,1,1,0,0,0,1,3,4,4,3,3,4,1,0,1,4,3,3,3,3,3,3,4,1,1,3,3,3,3,3,3,3,3,1,1,4,3,3,3,3,10,3,4,1,1,5,4,3,3,3,3,3,10,1,0,1,10,10,3,3,3,10,1,0,0,1,1,1,1,1,1,1,0,0,1,8,8,8,8,8,8,6,1,0,1,2,1,6,8,8,6,8,8,1,0,1,1,8,8,8,8,1,2,1,0,0,1,6,7,7,7,1,1,0,0,0,1,6,6,1,6,6,1,0,0,0,1,7,7,1,1,1,0,0,0,0,1,9,9,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0
  };

const PROGMEM byte spriteFigureBackMiddle[160] = {
    0,0,1,1,1,1,1,1,0,0,0,1,3,4,4,3,3,4,1,0,1,4,3,3,3,3,3,3,4,1,1,3,3,3,3,3,3,3,3,1,1,4,3,3,3,3,10,3,4,1,1,5,4,3,3,3,3,3,10,1,0,1,10,10,3,3,3,10,1,0,0,0,1,1,1,1,1,1,0,0,0,1,8,8,8,8,8,6,1,0,1,8,6,6,8,8,6,6,8,1,1,2,1,8,8,8,8,1,2,1,0,1,1,7,6,6,7,1,1,0,0,0,1,6,7,7,6,1,0,0,0,0,1,9,9,9,9,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

// ########################################################
// Karten und weitere eigenschaften
// ########################################################

// Kleine Karte
const PROGMEM byte mapContent[160] =  { 
  1,1,1,1,1,1,1,1,1,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,1,0,0,1,0,0,0,1,
  1,0,1,0,0,1,1,1,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,
};

// map size
// Groeße einer Kachel
byte mapTileSize = 16;
// Anzahl Kacheln auf der X Achse
byte mapTileCountX = 10;
// Anzahl Kacheln auf der Y Achse
byte mapTileCountY = 6;

// ########################################################
// Variablen fuer Kollisionserkennung
// ########################################################
// index felder die zu prüfen sind
// wird in Abhaengigkeit der Richtung verwendet
int8_t collisionTiles[3] = { -1, 0, 1 };

void setup() {

  // init display
  EsploraTFT.begin();
  EsploraTFT.initR(INITR_BLACKTAB);
  EsploraTFT.setRotation(1);
  EsploraTFT.background(0, 0, 0);


  renderMap(lastPosX, lastPosY, true);
}

void loop() {

  gameTime++;
  
  // Eingänge einlesen
  int stickX = Esplora.readJoystickX() + (offsetX * -1);
  int stickY = Esplora.readJoystickY() + (offsetY * -1);

  boolean buttonLeft = false;
  boolean buttonRight = false;
  boolean buttonUp = false;
  boolean buttonDown = false;

  if(stickX > maxDeathbandX || stickX < (maxDeathbandX * -1)) {
    buttonLeft = stickX > maxDeathbandX;
    buttonRight = stickX < (maxDeathbandX * -1);
  }
  else if(stickY > maxDeathbandY || stickY < (maxDeathbandY * -1)) {
    buttonUp = stickY < (maxDeathbandY * -1);
    buttonDown = stickY > maxDeathbandY;
  }

  // Temporaer letzte Position merken
  int lastPosXtemp = lastPosX;
  int lastPosYtemp = lastPosY;

  directionX = 0;
  directionY = 0;

  // Abfragen zu den gesetzten joystick
  // Es kann nur in eine Richtung die Bedingung erfüllt werden.
  
  // Wenn nach links oder rechts gedrückt wird.
  if(buttonLeft && !buttonRight && lastPosX > 0) {
    directionX = -1;
    // nach links und letzte Position Y ist groesser als '0'.
    if(CanEnterArea(lastPosX, lastPosY)) {
      lastPosX--;
    }
  }
  else if(!buttonLeft && buttonRight && lastPosX < EsploraTFT.width() - 16) {
    directionX = 1;
    // nach rechts und letzte Position X ist kleiner als die TFT Pixel Breite.
    if(CanEnterArea(lastPosX, lastPosY)) {
      lastPosX++;
    }
  }

  if(buttonUp && !buttonDown && lastPosY > 0) {
    directionY = -1;
    // nach oben und letzte Position Y ist groesser als '0'.
    if(CanEnterArea(lastPosX, lastPosY)) {
      lastPosY--;
    }
  }
  else if(!buttonUp && buttonDown && lastPosY < 110) {
    directionY = 1;
    // nach unten und letzte Position X ist kleiner als die TFT Pixel hoehe.
    if(CanEnterArea(lastPosX, lastPosY)) {
      lastPosY++;
    }
  }

  // Wenn sich X oder Y Position unterscheiden, dann den zu bewegenden Punkt neu zeichen.
  if(lastPosX != lastPosXtemp || lastPosY != lastPosYtemp) {

    renderMap(lastPosX, lastPosY, false);
    drawFigure(directionX, directionY, lastPosX, lastPosY);
  }
}