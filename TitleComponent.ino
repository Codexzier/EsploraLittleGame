// ========================================================================================
// Description:       Startbildschirm "Photo Quest".
//                    Auswahl: Neues Spiel oder letzten Spielstand laden.
//                    Der Startbildschirm ist ein Fenster (WIN_TITLE), dadurch gilt die
//                    gleiche Steuerung wie in allen Dialogen:
//                    Joystick hoch / runter waehlt, Button 4 bestaetigt.
// ========================================================================================

// ========================================================================================
// Texte

const PROGMEM char mTitleName[] = "Photo Quest";
const PROGMEM char mTitleFooter[] = "[4] OK";

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Zeigt den Startbildschirm. "Spiel laden" gibt es nur mit gespeichertem Spielstand
// und ist dann vorausgewaehlt.
void showTitle() {

  openWindow(WIN_TITLE, NULL, NULL);
  addWindowOption(OPT_NEW_GAME);

  if(hasSaveGame()) {
    addWindowOption(OPT_LOAD_GAME);
    mWindowChoice = 1;
  }
}

// ========================================================================================
// Zeichnet den Startbildschirm: Name, Filmstreifen mit den vier Fotos und Fusszeile.
// Die Auswahl zeichnet drawWindowOptions().
void drawTitle() {

  EsploraTFT.fillRect(0, 0, MAP_WIDTH, EsploraTFT.height(), colorOf(1));

  const char* title = mTitleName;                                      // Name in doppelter Groesse
  for(byte i = 0; pgm_read_byte(title + i) != 0; i++) {
    EsploraTFT.drawChar(14 + i * 12, 14, pgm_read_byte(title + i), colorOf(12), colorOf(12), 2);
  }

  EsploraTFT.fillRect(0, 40, MAP_WIDTH, 30, colorOf(21));              // Filmstreifen mit den vier Fotos
  for(byte x = 4; x < MAP_WIDTH; x += 10) {
    EsploraTFT.fillRect(x, 42, 4, 3, colorOf(19));
    EsploraTFT.fillRect(x, 65, 4, 3, colorOf(19));
  }
  for(byte i = 0; i < 4; i++) {
    drawPackedIcon(24 + i * 32, 47, 16, 16, getItemIcon(ITEM_PHOTO + i), 1);
  }

  drawTextP(58, 116, mTitleFooter, colorOf(18));
  mWindowOptionsY = 84;
}
