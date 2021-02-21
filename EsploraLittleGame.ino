// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       BlogPost 05 - Arduino Esplorer - Use flash memory
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

  // Abfragen zu den gesetzten joystick
  // Es kann nur in eine Richtung die Bedingung erfüllt werden.
  
  // Wenn nach links oder rechts gedrückt wird.
  if(buttonLeft && !buttonRight && lastPosX > 0) {
    // nach links und letzte Position Y ist groesser als '0'.
    if(CanEnterArea(lastPosX - 1, lastPosY)) {
      lastPosX--;
    }
  }
  else if(!buttonLeft && buttonRight && lastPosX < EsploraTFT.width() - 16) {
    // nach rechts und letzte Position X ist kleiner als die TFT Pixel Breite.
    if(CanEnterArea(lastPosX + 10, lastPosY)) {
      lastPosX++;
    }
  }

  if(buttonUp && !buttonDown && lastPosY > 0) {
    // nach oben und letzte Position Y ist groesser als '0'.
    if(CanEnterArea(lastPosX, lastPosY - 1)) {
      lastPosY--;
    }
  }
  else if(!buttonUp && buttonDown && lastPosY < 110) {
    // nach unten und letzte Position X ist kleiner als die TFT Pixel hoehe.
    if(CanEnterArea(lastPosX, lastPosY + 16)) {
      lastPosY++;
    }
  }

  // Wenn sich X oder Y Position unterscheiden, dann den zu bewegenden Punkt neu zeichen.
  if(lastPosX != lastPosXtemp || lastPosY != lastPosYtemp) {

    int directX = 0;
    int directY = 0;

    if(lastPosXtemp > lastPosX) { directX = -1; }
    if(lastPosXtemp < lastPosX) { directX = 1; }
    if(lastPosYtemp > lastPosY) { directY = -1; }
    if(lastPosYtemp < lastPosY) { directY = 1; }

    renderMap(lastPosX, lastPosY, false);
    drawFigure(directX, directY, lastPosX, lastPosY);
  }
}

// kopiert den Array Inhalt vom Flashspeicher in den SRAM
void memCopy(byte arrayContent[]) {
  for(byte index = 0; index < 160; index++) {
    tempArray[index] = pgm_read_byte_near(arrayContent + index);
  }
}

