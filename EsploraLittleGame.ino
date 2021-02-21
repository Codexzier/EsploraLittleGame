// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       BlogPost 08 - Arduino Esplorer - Trader, box and coins
// Author:        Johannes P. Langner
// Controller:    Arduino Esplora
// TFT:           1.8" TFT Module
// Description:   Einsatz von Händler Boxen und Münzen
// ========================================================================================

// Trader, Box and coins

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

// blendet das Fenster ein.
bool showWindow = false;
// wird verwendet das laufen der Figur gegen eine Menu Navigation zu wechseln.
bool menueNavigation = false;
int menueNavigationDelay = 5;
int menueNavigationDelayCountX = 0;
int menueNavigationDelayCountY = 0;
int menueNavigationLastPosX = 0;
int menueNavigationLastPosY = 0;
int menueNavigationLastPosXLast = 0;
int menueNavigationLastPosYLast = 0;

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
// Karten und weitere Eigenschaften
// ########################################################

// Kleine Karte
const PROGMEM byte mapContent[160] =  { 
  1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,2,1,0,0,0,1,
  1,0,0,0,0,1,0,6,0,1,
  1,0,0,0,0,1,0,0,0,1,
  1,7,0,0,0,5,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,
};
// Karten Informationen
// 1 = Hindernis
// 2 = Item Schluessel
// 3 = Item Kamera
// 4 = Item Foto
// 5 = Gegenstand Tuer
// 6 = Haendler 
// 7 = Kiste
// Eindeutige Id des verwendungszweck
const PROGMEM uint16_t mapBarrierUsageDoor01 = 1;

// Kartenspezifischeeinstellung
bool mapBarrierDoorIsOpen = false;
bool mapKeyIsGet = false;

byte mapFigurePositionX = -1;
byte mapFigurePositionY = -1;
bool mapFigureRerender = false;

// ########################################################
// Allgemeine Karten Informationen
// ########################################################
// map size
// Groeße einer Kachel
byte mapTileSize = 16;
// Anzahl Kacheln auf der X Achse
byte mapTileCountX = 10;
// Anzahl Kacheln auf der Y Achse
byte mapTileCountY = 6;


void setup() {

  // init display
  EsploraTFT.begin();
  EsploraTFT.initR(INITR_BLACKTAB);
  EsploraTFT.setRotation(1);
  EsploraTFT.background(0, 0, 0);


  renderMap(lastPosX, lastPosY, true);
  drawFigure(directionX, directionY, lastPosX, lastPosY);
  drawEmptyPlaces();
  drawCoinsStatus(false);
}

void loop() {
  gameTime++;

  // Reset Einstellungen
  if(!Esplora.readButton(SWITCH_1)) {
    lastPosX = 16;
    lastPosY = 16;

    mapBarrierDoorIsOpen = false;
    mapKeyIsGet = false;
    EsploraTFT.fillRect(0, 6 * 16, mapTileSize * 3, mapTileSize * 2, ST7735_BLACK);
    renderMap(lastPosX, lastPosY, true);
  }
  
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

  // Joystick
  // Es kann nur in eine Richtung die Bedingung erfüllt werden.
  
  // Wenn nach links oder rechts gedrückt wird.
  if(buttonLeft && !buttonRight && lastPosX > 0) {
    if(menueNavigation) {
      
    }
    else {
      directionX = -1;
      // nach links und letzte Position Y ist groesser als '0'.
      if(CanEnterArea(lastPosX, lastPosY)) {
        lastPosX--;
      }
    }
  }
  else if(!buttonLeft && buttonRight && lastPosX < EsploraTFT.width() - 16) {
    if(menueNavigation) {
      
    }
    else {
      directionX = 1;
      // nach rechts und letzte Position X ist kleiner als die TFT Pixel Breite.
      if(CanEnterArea(lastPosX, lastPosY)) {
        lastPosX++;
      }
    }
  }

  if(buttonUp && !buttonDown && lastPosY > 0) {
    if(menueNavigation) {
      
    }
    else {
      directionY = -1;
      // nach oben und letzte Position Y ist groesser als '0'.
      if(CanEnterArea(lastPosX, lastPosY)) {
        lastPosY--;
      }
    }
  }
  else if(!buttonUp && buttonDown && lastPosY < 110) {
    if(menueNavigation) {
      
    }
    else {
      directionY = 1;
      // nach unten und letzte Position X ist kleiner als die TFT Pixel hoehe.
      if(CanEnterArea(lastPosX, lastPosY)) {
        lastPosY++;
      }
    }
  }

  // Wenn sich X oder Y Position unterscheiden, dann den zu bewegenden Punkt neu zeichnen.
  if(!menueNavigation && lastPosX != lastPosXtemp || lastPosY != lastPosYtemp) {

    drawStack(false);
  }
  else if(menueNavigation) {
    menueNavigateWithDelay();
  }

  drawWindow(lastPosX > EsploraTFT.width() / 2);
  drawCoinsStatus(false);
}

// verzögert die Eingaben für die Navigation durch das Menue.
void menueNavigateWithDelay() {

  // Button zum schließen
  if(!Esplora.readButton(SWITCH_2)) {

    // zurueck setzen
    menueNavigation = false;
    menueNavigationDelayCountX = 0;
    menueNavigationDelayCountY = 0;
    directionY = 1;
    lastPosY++;
    drawStack(true);
  }
  
  if(menueNavigationDelayCountX > menueNavigationDelay || menueNavigationDelayCountY > menueNavigationDelay) {

      // Navigationsbereich
      // delay counter zurueck setzen
      menueNavigationDelayCountX = 0;
      menueNavigationDelayCountY = 0;
  }
  else {
    if(menueNavigationLastPosXLast != menueNavigationLastPosX) {
      menueNavigationDelayCountY = 0;
      menueNavigationDelayCountX++;
    }
    else if(menueNavigationLastPosYLast != menueNavigationLastPosY) {
      menueNavigationDelayCountX = 0;
      menueNavigationDelayCountY++;
    }

    menueNavigationLastPosXLast = menueNavigationLastPosX;
    menueNavigationLastPosYLast = menueNavigationLastPosY;
  }
}

// Zeichnet den zu rendernden Inhalt in der Vorgesehenen Reihenfolge.
// renderMapAll = Rendert den gesammten Karten Inhalt neu.
void drawStack(bool renderMapAll) {
  renderMap(lastPosX, lastPosY, renderMapAll);

    if(lastPosY > mapFigurePositionY) {
      // 1 = id Nummer von Haendler
      drawTrader(1, mapFigurePositionX, mapFigurePositionY); // Haendlerin
      drawFigure(directionX, directionY, lastPosX, lastPosY);
    }
    else {
      drawFigure(directionX, directionY, lastPosX, lastPosY);
      drawTrader(1, mapFigurePositionX, mapFigurePositionY); // Haendlerin
    }
}

// kopiert den Array Inhalt vom Flashspeicher in den SRAM
// wird für die Figur verwendet.
void memCopy(byte arrayContent[]) {
  for(byte index = 0; index < 160; index++) {
    tempArray[index] = pgm_read_byte_near(arrayContent + index);
  }
}
