// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Arduino Esplora - Clean up (Teil 9)
// Author:        Johannes P. Langner
// Controller:    Arduino Esplora
// Sensors:       Joystick, Buttons
// Actor:         TFT 1.8" 128x160 SPI
// Description:   Kleines Adventure Spiel entiwckeln
// ========================================================================================

#include <SPI.h>
#include <TFT.h>
#include <Esplora.h>
#include <avr/pgmspace.h>

// ========================================================================================
// Steuerung der Figur

// TODO: Wird ersetz durch die System Zeit
long mGameTime = 0;                                                    // die Spielinterval

int mLastPosX = 16;                                                    // ruft die letzte Postion X ab. (Pixel Position)
int mLastPosY = 16;                                                    // ruft die letzte Postion Y ab. (Pixel Position)

int mDirectionX = 0;                                                   // Bewegungsrichtung auf der X Achse
int mDirectionY = 0;                                                   // Bewegungsrichtung auf der Y Achse

int mOffsetX = -3;                                                     // Kalibrierungs wert für die Mittelstellung des Joystick X
int mOffsetY = 4;                                                      // Kalibrierungs wert für die Mittelstellung des Joystick Y
byte mMaxDeathbandX = 5;                                               // plus minus breich, wo der Stick noch nicht reagieren soll auf der X Achse
byte mMaxDeathbandY = 5;                                               // plus minus breich, wo der Stick noch nicht reagieren soll auf der Y Achse

// ========================================================================================
// Fenster und Navigation Steuerung

bool mShowWindow = false;                                              // blendet das Fenster ein.
bool mMenueNavigation = false;                                         // wird verwendet das laufen der Figur gegen eine Menu Navigation zu wechseln.

int mMenueNavigationDelay = 5;                                         // navigations verzoegerung
int mMenueNavigationDelayCountX = 0;                                   // verzoegerungszaehler fuer X Achsen 
int mMenueNavigationDelayCountY = 0;                                   // verzoegerungszaehler fuer Y Achsen 
int mMenueNavigationLastPosX = 0;                                      // actuelle Navigationsposition X
int mMenueNavigationLastPosY = 0;                                      // actuelle Navigationsposition Y
int mMenueNavigationLastPosXLast = 0;                                  // letzte Navigationsposition X
int mMenueNavigationLastPosYLast = 0;                                  // letzte Navigationsposition Y




// ========================================================================================
// Karten Eigenschaften und Einstellungen

byte mMapTileSize = 16;                                                // Groeße einer Kachel
byte mMapTileCountX = 10;                                              // Anzahl Kacheln auf der X Achse
byte mMapTileCountY = 6;                                               // Anzahl Kacheln auf der Y Achse

const PROGMEM byte mMapContent[160] =  {                               // Karten Informationen
  1,1,1,1,1,1,1,1,1,1,                                                // 1 = Hindernis
  1,0,0,0,2,1,0,0,0,1,                                                // 2 = Item Schluessel
  1,0,0,0,0,1,0,6,0,1,                                                // 3 = Item Kamera
  1,0,0,0,0,1,0,0,0,1,                                                // 4 = Item Foto
  1,7,0,0,0,5,0,0,0,1,                                                // 5 = Gegenstand Tuer
  1,1,1,1,1,1,1,1,1,1,                                                // 6 = Haendler
};                                                                    // 7 = Kiste

const PROGMEM uint16_t mMapBarrierUsageDoor01 = 1;                     // Eindeutige Id des verwendungszweck

// ----------------------------------------------------------------------------------------
// Kartenspezifischeeinstellung

bool mMapBarrierDoorIsOpen = false;                                    // Tuer die geoeffnet werden kann
bool mMapKeyIsGet = false;                                             // schluessel der aufgehoben werden kann

byte mMapFigurePositionX = -1;                                         // Position X Haendler Figur, wird erst spaeter zugewiesen
byte mMapFigurePositionY = -1;                                         // Position Y Haendler Figur, wird erst spaeter zugewiesen
bool mMapFigureRerender = false;                                       // Figur neu zeichnen, wird erst spaeter zugewiesen

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
void setup() {

  EsploraTFT.begin();                                                 // init display
  EsploraTFT.initR(INITR_BLACKTAB);               
  EsploraTFT.setRotation(1);                                          // festlegen der Bildschirm ausrichtung
  EsploraTFT.background(0, 0, 0);                                     // Hintergrund komplett schwarz einfaerben

  renderMap(mLastPosX, mLastPosY, true);                                // render gesammte Karte
  drawFigure(mDirectionX, mDirectionY, mLastPosX, mLastPosY);             // Figur Zeichnen an der Start Position
  drawEmptyPlaces();                                                  // Zeichne Leere Taschenplaetze
  drawCoinsStatus(false);                                             // Zeichne Coin Status
}

// ========================================================================================
void loop() {

  // TODO: wird noch geaendert
  mGameTime++;                                                         // aktuelle laufzeit


// ----------------------------------------------------------------------------------------
// Spiel zuruecksetzten, wenn der Button 1 gedrueckt wird.
  if(!Esplora.readButton(SWITCH_1)) {                                 // Reset Einstellungen
    mLastPosX = 16;                                                    // Figur an Start Position X zurueck setzten
    mLastPosY = 16;                                                    // Figur an Start Position Y zurueck setzten

// ----------------------------------------------------------------------------------------
// Kartenspezifischeeinstellung

    mMapBarrierDoorIsOpen = false;                                     // tuer schließen
    mMapKeyIsGet = false;                                              // schluessel zurueck legen

    EsploraTFT.background(0, 0, 0);                                   // Bildschirm Inhalt zurueck setzten
    renderMap(mLastPosX, mLastPosY, true);                              // Karte neu zeichnen
    drawFigure(mDirectionX, mDirectionY, mLastPosX, mLastPosY);           // Figur neu zeichnen
    drawEmptyPlaces();                                                // Taschenplaetze neu zeichnen
    drawCoinsStatus(false);                                           // Coin Status zurueck setzten
  }

// ----------------------------------------------------------------------------------------
// Stick Eingabe aufbereiten

  int stickX = Esplora.readJoystickX() + (mOffsetX * -1);              // X Achse des Joystick einlesen
  int stickY = Esplora.readJoystickY() + (mOffsetY * -1);              // Y Achse des Joystick einlesen

  boolean buttonLeft = false;                                         // nach links laufen
  boolean buttonRight = false;                                        // nach rechts laufen
  boolean buttonUp = false;                                           // nach oben
  boolean buttonDown = false;                                         // nach unten

  if(stickX > mMaxDeathbandX || stickX < (mMaxDeathbandX * -1)) {       // wird der Stick nach links oder rechts bewegt
    buttonLeft = stickX > mMaxDeathbandX;                              // nach links, wenn der mindest Wert ueberschritten wird.
    buttonRight = stickX < (mMaxDeathbandX * -1);                      // nach rechts, wenn der mindest Wert ueberschritten wird.
  }
  else if(stickY > mMaxDeathbandY || stickY < (mMaxDeathbandY * -1)) {  // wird der Stick nach oben oder unten bewegt.
    buttonUp = stickY < (mMaxDeathbandY * -1);                         // nach oben, wenn der mindest Wert ueberschritten wird.
    buttonDown = stickY > mMaxDeathbandY;                              // nach unten, wenn der mindest Wert ueberschritten wird.
  }

  int lastPosXtemp = mLastPosX;                                        // Temporaer letzte Position zur X Achse merken
  int lastPosYtemp = mLastPosY;                                        // Temporaer letzte Position zur Y Achse merken

  mDirectionX = 0;                                                     // Ausrichtung X Achse zurueck setzen
  mDirectionY = 0;                                                     // Ausrichtung Y Achse zurueck setzen

// ----------------------------------------------------------------------------------------
// Bedingtes Laufen bzw. Menue Navigation

  if(buttonLeft && !buttonRight && mLastPosX > 0) {                    // nach links
    if(mMenueNavigation) {                                             // Im Menu oder Taschen Plaetzen navigieren
      // TODO: noch offen
    }
    else {
      mDirectionX = -1;                                                // Ausrichtung nach links
      if(CanEnterArea(mLastPosX, mLastPosY)) {                          // kann der Bereich nach links betreten werden
        mLastPosX--;                                                   // Position um einen Pixel nach Links bewegen
      }
    }
  }
  else if(!buttonLeft && buttonRight &&                               // nach rechts 
          mLastPosX < EsploraTFT.width() - 16) { 
    
    if(mMenueNavigation) {                                             // Im Menu oder Taschen Plaetzen navigieren
      // TODO: noch offen
    }
    else {
      mDirectionX = 1;                                                 // Ausrichtung nach rechts
      if(CanEnterArea(mLastPosX, mLastPosY)) {                          // kann der Bereich nach rechts betreten werden
        mLastPosX++;                                                   // Position um einen Pixel nach rechts bewegen
      }
    }
  }

  if(buttonUp && !buttonDown && mLastPosY > 0) {                       // nach oben
    if(mMenueNavigation) {                                             // Im Menu oder Taschen Plaetzen navigieren
      // TODO: noch offen
    }
    else {
      mDirectionY = -1;                                                // Ausrichtung nach oben
      if(CanEnterArea(mLastPosX, mLastPosY)) {                          // kann der Bereich nach oben betreten werden
        mLastPosY--;                                                   // Position um einen Pixel nach oben bewegen
      }
    }
  }
  else if(!buttonUp && buttonDown && mLastPosY < 110) {                // nach unten
    if(mMenueNavigation) {                                             // Im Menu oder Taschen Plaetzen navigieren
      // TODO: noch offen
    }
    else {
      mDirectionY = 1;
      if(CanEnterArea(mLastPosX, mLastPosY)) {                          // kann der Bereich nach unten betreten werden
        mLastPosY++;                                                   // Position um einen Pixel nach unten bewegen
      }
    }
  }

  if(!mMenueNavigation && mLastPosX != lastPosXtemp ||                  // Wenn sich X oder Y Position unterscheiden, 
      mLastPosY != lastPosYtemp) {                                     // dann den zu bewegenden Punkt neu zeichen.

    drawStack(false);                                                 // zeichenet die Figur und die Haendlering neu
  }
  else if(mMenueNavigation) {                                          // wenn in der Menue Navigiert wird
    menueNavigateWithDelay();                                         // Navigation mit verzogerte Bewegung
  }

// ----------------------------------------------------------------------------------------
// Inhalte neu zeichnen

  drawWindow(mLastPosX > EsploraTFT.width() / 2);                      // Zeichnet das Fenster neu, wenn activ
  drawCoinsStatus(false);                                             // zeichnet den Coin Status neu
}

// ========================================================================================
// verzoegert die Eingaben für die Navigation durch das Menue.
void menueNavigateWithDelay() {

  if(!Esplora.readButton(SWITCH_2)) {                                 // Fenster schließen mit Button 2
// ----------------------------------------------------------------------------------------
// zurueck setzen
    mMenueNavigation = false;
    mMenueNavigationDelayCountX = 0;
    mMenueNavigationDelayCountY = 0;
    mDirectionY = 1;
    mLastPosY++;
    drawStack(true);
  }
  
  if(mMenueNavigationDelayCountX > mMenueNavigationDelay ||             // Navigationszaehler ueberschritten
     mMenueNavigationDelayCountY > mMenueNavigationDelay) {

      // TODO: wird weiter ausgebaut

      mMenueNavigationDelayCountX = 0;                                 // X Achse delay counter zurueck setzen
      mMenueNavigationDelayCountY = 0;                                 // Y Achse delay counter zurueck setzen
  }
  else {
    if(mMenueNavigationLastPosXLast != mMenueNavigationLastPosX) {      // x Achse, wenn aktuelle und letzte position sich geaendert hat
      mMenueNavigationDelayCountY = 0;                                 // y verzogerungszaehler zuruecksetzen
      mMenueNavigationDelayCountX++;                                   // x verzoegerung um einen hoch zaehlen
    }
    else if(mMenueNavigationLastPosYLast != mMenueNavigationLastPosY) { // y Achse, wenn aktuelle und letzte position sich geaendert hat
      mMenueNavigationDelayCountX = 0;                                 // x verzogerungszaehler zuruecksetzen
      mMenueNavigationDelayCountY++;                                   // y verzoegerung um einen hoch zaehlen
    }

    mMenueNavigationLastPosXLast = mMenueNavigationLastPosX;            // X Achse aktuelle Position an letzte zuweisen
    mMenueNavigationLastPosYLast = mMenueNavigationLastPosY;            // Y Achse aktuelle Position an letzte zuweisen
  }
}

// ========================================================================================
// Zeichnet den zu rendernden Inhalt in der Vorgesehenen Reihenfolge.
// ----------------------------------------------------------------------------------------
// renderMapAll = Rendert den gesammten Karten Inhalt neu.
void drawStack(bool renderMapAll) {
  renderMap(mLastPosX, mLastPosY, renderMapAll);                        // Karte neu Zeichnen

    if(mLastPosY > mMapFigurePositionY) {                               // Y Achse, wenn die Figur in den Aktionsradius 
                                                                      // hoeher liegt als die Haendlerin
      
      drawTrader(1, mMapFigurePositionX, mMapFigurePositionY);          // 1 = id Nummer von Haendlerin
      drawFigure(mDirectionX, mDirectionY, mLastPosX, mLastPosY);         // Figur neu zeichnen
    }
    else {
      drawFigure(mDirectionX, mDirectionY, mLastPosX, mLastPosY);         // Figur neu zeichnen
      drawTrader(1, mMapFigurePositionX, mMapFigurePositionY);          // 1 = id Nummer von Haendlerin
    }
}