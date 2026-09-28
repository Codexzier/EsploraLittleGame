// ========================================================================================
// Description:       Inhalte und Methoden zum Darstellen eines Message Box Fensters.
// ----------------------------------------------------------------------------------------
// Steuerung:         Joystick hoch / runter = Auswahl wechseln
//                    Button 4 (rechts)      = Bestaetigen / Weiter
//                    Button 2 (links)       = Schliessen / Zurueck
// ========================================================================================

// ========================================================================================
// Functionsvariablen

#define WIN_X             4                                            // Position und Groesse des Fensters
#define WIN_Y             4
#define WIN_W             152
#define WIN_H             88
#define WIN_TEXT_X        (WIN_X + 6)
#define WIN_LINE_CHARS    23                                           // Zeichen pro Zeile
#define WIN_LINE_HEIGHT   9                                            // Pixel pro Zeile


// ----------------------------------------------------------------------------------------
// Texte

const PROGMEM char mOptOpenChest[] = "Oeffnen";
const PROGMEM char mOptKeepClosed[] = "Zu lassen";
const PROGMEM char mOptBuyCamera[] = "Kaufe Kamera (200)";
const PROGMEM char mOptSellCamera[] = "Verkaufe Kamera (140)";
const PROGMEM char mOptGivePhoto[] = "Gib Surie das Foto";
const PROGMEM char mOptBye[] = "Tschuess";
const PROGMEM char mOptTakePhoto[] = "Foto machen";
const PROGMEM char mOptNotNow[] = "Nicht jetzt";
const PROGMEM char mOptGivePhotos[] = "Gib Surie die Fotos";
const PROGMEM char mOptBuyBoatTicket[] = "Kaufe Ticket (60)";
const PROGMEM char mOptBuyBusTicket[] = "Kaufe Ticket (40)";
const PROGMEM char mOptTravel[] = "Losfahren";
const PROGMEM char mOptDrinkCoffee[] = "Kaffee trinken";
const PROGMEM char mOptNewGame[] = "Neues Spiel";
const PROGMEM char mOptLoadGame[] = "Spiel laden";

const PROGMEM char mWindowFooterNext[] = "[4] Weiter";
const PROGMEM char mWindowFooterChoice[] = "[4] OK    [2] Zurueck";

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Oeffnet ein Fenster. Gezeichnet wird es im naechsten Durchlauf.
// ----------------------------------------------------------------------------------------
// type  = WIN_MESSAGE, WIN_CHOICE oder WIN_END
// title = Titel im Flash Speicher (NULL = ohne Titel)
// text  = Text im Flash Speicher
void openWindow(byte type, const char* title, const char* text) {

  mWindowType = type;
  mWindowTitle = title;
  mWindowText = text;
  mWindowOptionCount = 0;
  mWindowChoice = 0;
  mWindowNeedsDraw = true;
  mWindowWaitRelease = true;
}

// ========================================================================================
// Fuegt dem Fenster eine Auswahl hinzu.
void addWindowOption(byte option) {

  if(mWindowOptionCount < WINDOW_MAX_OPTIONS) {
    mWindowOptions[mWindowOptionCount] = option;
    mWindowOptionCount++;
  }
}

// ========================================================================================
// Schliesst das Fenster und zeichnet die Karte darunter neu.
void closeWindow() {

  mWindowType = WIN_NONE;
  mWindowOptionCount = 0;

  if(mWindowFollowType != WIN_NONE) {                                  // naechstes Fenster direkt anzeigen
    byte type = mWindowFollowType;
    mWindowFollowType = WIN_NONE;
    openWindow(type, mWindowFollowTitle, mWindowFollowText);
    return;
  }

  if(mMapNeedsReload) {                                                // Figur hat die Karte verlassen
    mMapNeedsReload = false;
    byte latch = mBumpLatch;
    reloadMap();
    mBumpLatch = latch;
    return;
  }

  renderArea(WIN_X, WIN_Y, WIN_W, WIN_H);
  saveGame();                                                          // automatisch speichern
}

// ========================================================================================
// Legt ein Fenster fest, das nach dem Schliessen des aktuellen Fensters folgt.
// ----------------------------------------------------------------------------------------
// type  = WIN_MESSAGE oder WIN_END
// title = Titel im Flash Speicher
// text  = Text im Flash Speicher
void setWindowFollow(byte type, const char* title, const char* text) {

  mWindowFollowType = type;
  mWindowFollowTitle = title;
  mWindowFollowText = text;
}

// ========================================================================================
// Navigation im Fenster, wird in jedem Durchlauf aufgerufen.
void updateWindow(unsigned long now) {

  if(mWindowNeedsDraw) {
    drawWindow();
    mWindowNeedsDraw = false;
  }

  int8_t dx = 0;
  int8_t dy = 0;
  readStick(&dx, &dy);

  if(dy == 0) {                                                        // Stick in der Mitte
    mWindowWaitRelease = false;
    mLastMenuMove = 0;
  }
  else if(!mWindowWaitRelease && mWindowOptionCount > 1 &&             // verzoegert die Eingaben fuer die Navigation
          (mLastMenuMove == 0 || now - mLastMenuMove > MENU_REPEAT_INTERVAL)) {

    mWindowChoice = (mWindowChoice + mWindowOptionCount + dy) % mWindowOptionCount;
    mLastMenuMove = now;
    drawWindowOptions();
  }

  if(buttonPressed(SWITCH_4)) {                                        // Bestaetigen
    if(mWindowOptionCount > 0) {
      onWindowChoice(mWindowOptions[mWindowChoice]);
    }
    else {
      closeWindow();
    }
  }
  else if(buttonPressed(SWITCH_2) && mWindowType != WIN_TITLE) {       // Fenster schließen mit Button 2
    closeWindow();
  }
}

// ========================================================================================
// Zeichnet das Fenster mit einem Ensprechenden Text
void drawWindow() {

  if(mWindowType == WIN_TITLE) {                                       // Startbildschirm
    drawTitle();
    drawWindowOptions();
    return;
  }

  EsploraTFT.fillRect(WIN_X, WIN_Y, WIN_W, WIN_H, colorOf(1));
  EsploraTFT.drawRect(WIN_X, WIN_Y, WIN_W, WIN_H, colorOf(18));
  EsploraTFT.drawRect(WIN_X + 2, WIN_Y + 2, WIN_W - 4, WIN_H - 4, colorOf(18));

  int y = WIN_Y + 6;
  if(mWindowTitle != NULL) {                                           // Titel in gelb
    drawTextP(WIN_TEXT_X, y, mWindowTitle, colorOf(12));
    y += WIN_LINE_HEIGHT + 3;
  }

  byte lines = drawWrappedTextP(WIN_TEXT_X, y, mWindowText, colorOf(19));
  mWindowOptionsY = y + lines * WIN_LINE_HEIGHT + 3;
  drawWindowOptions();

  drawTextP(WIN_TEXT_X, WIN_Y + WIN_H - 13,
            mWindowType == WIN_CHOICE ? mWindowFooterChoice : mWindowFooterNext,
            colorOf(18));
}

// ========================================================================================
// Zeichnet die Auswahl. Die gewaehlte Zeile ist gelb und hat einen Pfeil.
void drawWindowOptions() {

  for(byte i = 0; i < mWindowOptionCount; i++) {
    int y = mWindowOptionsY + i * WIN_LINE_HEIGHT;
    bool selected = (i == mWindowChoice);
    uint16_t color = colorOf(selected ? 12 : 18);

    EsploraTFT.fillRect(WIN_TEXT_X, y, WIN_W - 12, 8, colorOf(1));
    if(selected) {
      EsploraTFT.drawChar(WIN_TEXT_X, y, '>', color, color, 1);
    }
    drawTextP(WIN_TEXT_X + 12, y, getOptionLabel(mWindowOptions[i]), color);
  }
}

// ========================================================================================
// Text zu einer Auswahl.
const char* getOptionLabel(byte option) {

  switch(option) {
    case(OPT_OPEN_CHEST):  { return mOptOpenChest; }
    case(OPT_KEEP_CLOSED): { return mOptKeepClosed; }
    case(OPT_BUY_CAMERA):  { return mOptBuyCamera; }
    case(OPT_SELL_CAMERA): { return mOptSellCamera; }
    case(OPT_GIVE_PHOTO):  { return mOptGivePhoto; }
    case(OPT_TAKE_PHOTO):  { return mOptTakePhoto; }
    case(OPT_NOT_NOW):     { return mOptNotNow; }
    case(OPT_GIVE_PHOTOS): { return mOptGivePhotos; }
    case(OPT_BUY_BOAT_TICKET): { return mOptBuyBoatTicket; }
    case(OPT_BUY_BUS_TICKET):  { return mOptBuyBusTicket; }
    case(OPT_TRAVEL):      { return mOptTravel; }
    case(OPT_DRINK_COFFEE): { return mOptDrinkCoffee; }
    case(OPT_NEW_GAME):    { return mOptNewGame; }
    case(OPT_LOAD_GAME):   { return mOptLoadGame; }
    default:               { return mOptBye; }
  }
}

// ========================================================================================
// Schreibt einen Text mit Zeilenumbruch an Wortgrenzen.
// ----------------------------------------------------------------------------------------
// x, y  = Anfangsposition
// text  = Text im Flash Speicher
// color = Farbwert (RGB565)
// Rueckgabe = Anzahl der geschriebenen Zeilen
byte drawWrappedTextP(int x, int y, const char* text, uint16_t color) {

  byte line = 0;
  byte column = 0;

  while(true) {
    char c = pgm_read_byte(text);
    if(c == 0) {
      break;
    }

    if(c == ' ') {                                                     // Leerzeichen nur innerhalb einer Zeile
      if(column > 0) {
        column++;
      }
      text++;
      continue;
    }

    byte length = 0;                                                   // Laenge des naechsten Wortes
    while(true) {
      char w = pgm_read_byte(text + length);
      if(w == 0 || w == ' ') {
        break;
      }
      length++;
    }

    if(column > 0 && column + length > WIN_LINE_CHARS) {               // Wort passt nicht mehr in die Zeile
      line++;
      column = 0;
    }

    for(byte i = 0; i < length; i++) {
      if(column >= WIN_LINE_CHARS) {                                   // sehr langes Wort trennen
        line++;
        column = 0;
      }
      EsploraTFT.drawChar(x + column * 6, y + line * WIN_LINE_HEIGHT,
                          pgm_read_byte(text + i), color, color, 1);
      column++;
    }

    text += length;
  }

  return line + 1;
}
