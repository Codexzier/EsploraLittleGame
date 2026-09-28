// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Arduino Esplora - Suries Foto (Teil 10)
// Author:        Johannes P. Langner
// Controller:    Arduino Esplora
// Sensors:       Joystick, Buttons
// Actor:         TFT 1.8" 128x160 SPI
// Description:   Kleines Adventure Spiel entiwckeln
// ----------------------------------------------------------------------------------------
// Hinweis:       Die Arduino IDE haengt alle .ino Dateien alphabetisch hinter diese Datei.
//                Konstanten und gemeinsam genutzte Zustaende stehen deshalb hier,
//                damit sie in allen Komponenten bekannt sind.
// ========================================================================================

#include <SPI.h>
#include <TFT.h>
#include <Esplora.h>
#include <avr/pgmspace.h>

// ========================================================================================
// Bildschirm Aufteilung

#define MAP_TILE_SIZE         16                                       // Groesse einer Kachel
#define MAP_TILE_COUNT_X      10                                       // Anzahl Kacheln auf der X Achse
#define MAP_TILE_COUNT_Y      6                                        // Anzahl Kacheln auf der Y Achse
#define MAP_TILE_COUNT        60                                       // Kacheln pro Karte
#define MAP_WIDTH             160                                      // Pixel Breite der Karte
#define MAP_HEIGHT            96                                       // Pixel Hoehe der Karte
#define HUD_POS_Y             96                                       // Unterer Bereich: Rucksack, Muenzen, Ziel

#define FIGURE_WIDTH          10                                       // Breite einer Figur
#define FIGURE_HEIGHT         16                                       // Hoehe einer Figur

// ========================================================================================
// Steuerung und Taktung

#define FRAME_INTERVAL        16                                       // ms zwischen zwei Bewegungsschritten
#define MENU_REPEAT_INTERVAL  220                                      // ms bis die Auswahl im Fenster weiter springt
#define RESET_HOLD_TIME       1000                                     // ms Button 1 halten fuer einen Neustart
#define STICK_DEADBAND        30                                       // plus minus Bereich, in dem der Stick nicht reagiert

// ========================================================================================
// Karten

#define MAP_HOUSE             0                                        // Haus mit zwei Raeumen
#define MAP_GARDEN            1                                        // Garten mit Baum und Haus
#define MAP_COUNT             2

#define TILE_FLOOR            0                                        // Boden (Haus)
#define TILE_WALL             1                                        // Wand
#define TILE_KEY              2                                        // Item Schluessel
#define TILE_DOOR             5                                        // Tuer, braucht den Schluessel
#define TILE_NPC              6                                        // Startplatz von Surie
#define TILE_CHEST            7                                        // Kiste
#define TILE_EXIT             8                                        // Ausgang zur naechsten Karte
#define TILE_GRASS            9                                        // Boden (Garten)
#define TILE_PATH             10                                       // Weg
#define TILE_TREE             11                                       // Baum
#define TILE_HOUSE_WALL       12                                       // Hauswand mit Fenster
#define TILE_ROOF             13                                       // Dach
#define TILE_COIN             14                                       // Muenze zum Aufsammeln
#define TILE_PHOTO_SPOT       15                                       // Fotopunkt
#define TILE_HEDGE            16                                       // Hecke
#define TILE_FLOWERS          17                                       // Blumen
#define TILE_HOUSE_DOOR       18                                       // Haustuer (verschlossen)

#define MOVE_FREE             255                                      // Rueckgabe: Bewegung moeglich
#define MOVE_BLOCKED_NPC      254                                      // Rueckgabe: Figur steht im Weg
#define MOVE_BLOCKED_BORDER   253                                      // Rueckgabe: Kartenrand
#define NO_TILE               255                                      // kein Kachel Index

// ========================================================================================
// Items

#define ITEM_NONE             0
#define ITEM_KEY              1
#define ITEM_CAMERA           2
#define ITEM_PHOTO            3
#define BACKPACK_PLACES_COUNT 6

// ========================================================================================
// Spielablauf

#define QUEST_START           0                                        // Surie noch nicht getroffen
#define QUEST_TALKED          1                                        // Auftrag von Surie erhalten
#define QUEST_DONE            2                                        // Foto abgegeben, Spiel geschafft

#define START_COINS           25                                       // Muenzen zu Spielbeginn
#define CHEST_COINS           75                                       // Muenzen in der Kiste
#define FLOOR_COINS           25                                       // Muenzen je aufgesammelter Muenze
#define PHOTO_REWARD          150                                      // Belohnung fuer das Foto

// ========================================================================================
// Fenster (Dialoge)

#define WIN_NONE              0
#define WIN_MESSAGE           1                                        // Nachricht ohne Auswahl
#define WIN_CHOICE            2                                        // Nachricht mit Auswahl
#define WIN_END               3                                        // Abschluss Fenster

#define OPT_NONE              0
#define OPT_OPEN_CHEST        1
#define OPT_KEEP_CLOSED       2
#define OPT_BUY_CAMERA        3
#define OPT_SELL_CAMERA       4
#define OPT_GIVE_PHOTO        5
#define OPT_BYE               6
#define OPT_TAKE_PHOTO        7
#define OPT_NOT_NOW           8
#define WINDOW_MAX_OPTIONS    4

// ========================================================================================
// Figur

int mPosX = 16;                                                        // Position X der Figur (Pixel)
int mPosY = 16;                                                        // Position Y der Figur (Pixel)
int8_t mFacingX = 0;                                                   // Blickrichtung X Achse
int8_t mFacingY = 1;                                                   // Blickrichtung Y Achse
bool mIsWalking = false;                                               // wird die Figur gerade bewegt

int mOffsetX = -3;                                                     // Kalibrierungs wert für die Mittelstellung des Joystick X
int mOffsetY = 4;                                                      // Kalibrierungs wert für die Mittelstellung des Joystick Y

// ========================================================================================
// Karte und Spielstand

byte mCurrentMap = MAP_HOUSE;                                          // aktuelle Karte
byte mTileConsumed[MAP_COUNT][8];                                      // je Kachel ein Bit: aufgesammelt / geoeffnet
byte mBumpLatch = MOVE_FREE;                                           // verhindert, dass ein Anstossen mehrfach ausloest
byte mTriggerLatch = NO_TILE;                                          // verhindert, dass ein Betreten mehrfach ausloest

bool mNpcActive = false;                                               // steht Surie auf der aktuellen Karte
int mNpcX = 0;                                                         // Position X von Surie
int mNpcY = 0;                                                         // Position Y von Surie

byte mQuestState = QUEST_START;                                        // Fortschritt im Spiel
int16_t mCoins = START_COINS;                                          // Muenzen im Besitz
uint16_t mBackPlaces[BACKPACK_PLACES_COUNT];                           // Taschenplaetze, 0 = frei

// ========================================================================================
// Fenster Zustand

byte mWindowType = WIN_NONE;                                           // aktuell angezeigtes Fenster
const char* mWindowTitle = NULL;                                       // Titel (Flash Speicher)
const char* mWindowText = NULL;                                        // Text (Flash Speicher)
byte mWindowOptions[WINDOW_MAX_OPTIONS];                               // Auswahl Moeglichkeiten
byte mWindowOptionCount = 0;                                           // Anzahl der Auswahl Moeglichkeiten
byte mWindowChoice = 0;                                                // ausgewaehlte Moeglichkeit
bool mWindowNeedsDraw = false;                                         // Fenster muss gezeichnet werden
bool mWindowShowEndNext = false;                                       // nach dem Schliessen das Abschluss Fenster zeigen

// ========================================================================================
// Eingaben

byte mButtonState = 0;                                                 // gedrueckte Buttons (Bit je Button)
byte mButtonPressed = 0;                                               // in diesem Durchlauf neu gedrueckte Buttons
unsigned long mLastFrame = 0;                                          // Zeitpunkt des letzten Bewegungsschritts
unsigned long mLastMenuMove = 0;                                       // Zeitpunkt der letzten Menue Bewegung
unsigned long mResetPressedSince = 0;                                  // seit wann Button 1 gehalten wird

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
void setup() {

  EsploraTFT.begin();                                                  // init display
  EsploraTFT.initR(INITR_BLACKTAB);
  EsploraTFT.setRotation(1);                                           // festlegen der Bildschirm ausrichtung
  EsploraTFT.background(0, 0, 0);                                      // Hintergrund komplett schwarz einfaerben

  resetGame();
}

// ========================================================================================
void loop() {

  unsigned long now = millis();
  updateButtons();

  if(handleResetButton(now)) {                                         // Button 1 gehalten: Neustart
    return;
  }

  if(mWindowType != WIN_NONE) {                                        // im Fenster wird navigiert, die Figur steht
    updateWindow(now);
    return;
  }

  if(now - mLastFrame < FRAME_INTERVAL) {                              // gleichmaessige Geschwindigkeit
    return;
  }
  mLastFrame = now;

  int8_t dx = 0;
  int8_t dy = 0;
  readStick(&dx, &dy);

  if(dx != 0 || dy != 0) {
    movePlayer(dx, dy);
  }
  else if(mIsWalking) {                                                // stehen bleiben
    mIsWalking = false;
    figureStand();
    renderFigureArea();
  }
}

// ========================================================================================
// Setzt das komplette Spiel auf den Anfang zurueck.
void resetGame() {

  for(byte m = 0; m < MAP_COUNT; m++) {
    for(byte i = 0; i < 8; i++) {
      mTileConsumed[m][i] = 0;
    }
  }

  for(byte i = 0; i < BACKPACK_PLACES_COUNT; i++) {
    mBackPlaces[i] = ITEM_NONE;
  }

  mQuestState = QUEST_START;
  mCoins = START_COINS;
  mFacingX = 0;
  mFacingY = 1;
  mIsWalking = false;
  mWindowType = WIN_NONE;
  mWindowShowEndNext = false;
  figureStand();

  loadMap(MAP_HOUSE, 16, 16);
  drawHud();
}

// ========================================================================================
// Bewegt die Figur um einen Pixel und loest Aktionen aus.
// ----------------------------------------------------------------------------------------
// dx = Richtung X Achse (-1, 0, 1)
// dy = Richtung Y Achse (-1, 0, 1)
void movePlayer(int8_t dx, int8_t dy) {

  bool turned = (dx != mFacingX || dy != mFacingY);
  mFacingX = dx;
  mFacingY = dy;
  mIsWalking = true;

  byte blocker = checkMove(mPosX + dx, mPosY + dy);

  if(blocker != MOVE_FREE) {                                           // stehen bleiben
    figureStand();
    if(turned) {
      renderFigureArea();                                              // nur umdrehen
    }

    if(blocker != mBumpLatch) {                                        // einmalig auf das Anstossen reagieren
      mBumpLatch = blocker;
      onBump(blocker);
    }
    return;
  }

  mBumpLatch = MOVE_FREE;
  figureAdvanceAnimation();

  int oldX = mPosX;
  int oldY = mPosY;
  mPosX += dx;
  mPosY += dy;

  renderArea(min(oldX, mPosX), min(oldY, mPosY),                        // alte und neue Position in einem Rutsch
             FIGURE_WIDTH + abs(dx), FIGURE_HEIGHT + abs(dy));

  checkTrigger();
}

// ========================================================================================
// Zeichnet den Bereich der Figur neu (z.B. beim Umdrehen).
void renderFigureArea() {
  renderArea(mPosX, mPosY, FIGURE_WIDTH, FIGURE_HEIGHT);
}

// ========================================================================================
// Liest den Joystick ein. Es wird immer nur eine Achse zurueck gegeben,
// die mit dem groesseren Ausschlag.
// ----------------------------------------------------------------------------------------
// dx = Ergebnis X Achse (-1 links, 1 rechts)
// dy = Ergebnis Y Achse (-1 oben, 1 unten)
void readStick(int8_t* dx, int8_t* dy) {

  int stickX = Esplora.readJoystickX() - mOffsetX;                     // X Achse des Joystick einlesen
  int stickY = Esplora.readJoystickY() - mOffsetY;                     // Y Achse des Joystick einlesen

  *dx = 0;
  *dy = 0;

  if(abs(stickX) > STICK_DEADBAND && abs(stickX) >= abs(stickY)) {
    *dx = stickX > 0 ? -1 : 1;                                         // die X Achse ist beim Esplora gespiegelt
  }
  else if(abs(stickY) > STICK_DEADBAND) {
    *dy = stickY > 0 ? 1 : -1;
  }
}

// ========================================================================================
// Liest die vier Buttons ein und merkt sich, welche neu gedrueckt wurden.
void updateButtons() {

  byte state = 0;
  for(byte sw = SWITCH_1; sw <= SWITCH_4; sw++) {
    if(Esplora.readButton(sw) == LOW) {                                // LOW = gedrueckt
      state |= (1 << sw);
    }
  }

  mButtonPressed = state & ~mButtonState;
  mButtonState = state;
}

// ========================================================================================
// Wurde der Button in diesem Durchlauf neu gedrueckt.
bool buttonPressed(byte sw) {
  return (mButtonPressed & (1 << sw)) != 0;
}

// ========================================================================================
// Button 1 fuer eine Sekunde halten setzt das Spiel zurueck.
// Das verhindert ein versehentliches Zuruecksetzen.
bool handleResetButton(unsigned long now) {

  if((mButtonState & (1 << SWITCH_1)) == 0) {
    mResetPressedSince = 0;
    return false;
  }

  if(mResetPressedSince == 0) {
    mResetPressedSince = now;
  }
  else if(now - mResetPressedSince > RESET_HOLD_TIME) {
    mResetPressedSince = 0;
    resetGame();
    return true;
  }

  return false;
}
