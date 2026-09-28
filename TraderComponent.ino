// ========================================================================================
// Description:       Die Haendler und Kisten Funktionen werden hier unter gebracht.
//                    Ein Handler kann verschiedene Rollen haben.
//                    - Verkaeufer
//                    - Auftraggeber (Surie wuenscht sich ein Foto)
// ========================================================================================

// ========================================================================================
// Common Text

const PROGMEM char mTraderIntroText[] = "Hallo, ich bin Surie! Ich wuensche mir ein Foto von Baum und Haus im Garten. Eine Kamera verkaufe ich dir fuer 200 Muenzen.";
const PROGMEM char mTraderStartText[] = "Hallo, was darf ich dir verkaufen?";      // Begruessungstext
const PROGMEM char mTraderThanksText[] = "Danke nochmal fuer das schoene Foto!";  // Begruessung nach dem Auftrag
const PROGMEM char mTraderNotEnough[] = "Du hast nicht genug Muenzen.";            // Wenn zu wenig Muenzen zum Kaufen da sind
const PROGMEM char mTraderBoughtText[] = "Danke! Viel Spass mit der Kamera.";
const PROGMEM char mTraderSoldText[] = "Danke, ich nehme sie gerne zurueck.";
const PROGMEM char mTraderPhotoText[] = "Wunderschoen! Genau so habe ich es mir vorgestellt. Hier sind 150 Muenzen fuer dich!";

// ========================================================================================
// Common Sprite

const PROGMEM byte mTraderSpriteFrontMen[160] = {                                  // Bild vom Haendler / Die Farbe des Shirts, kann veraendert werden.                                  // Bild vom Haendler / Die Farbe des Shirts, kann veraendert werden.
  0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,100,1,0,1,100,100,100,100,101,100,100,100,1,1,100,100,101,2,101,101,100,100,1,1,100,5,5,2,2,5,5,100,1,1,101,101,1,2,2,1,101,100,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,0,0,0,1,102,102,3,3,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,2,1,102,102,102,102,1,2,1,0,1,1,104,105,105,104,1,1,0,0,0,1,104,105,105,104,1,0,0,0,0,1,104,105,105,104,1,0,0,0,0,1,9,10,10,9,1,0,0,0,0,0,1,1,1,1,0,0,0 
};
const PROGMEM byte mTraderSpriteFrontWomen[160] = {                                // Bild vom Haendlerin / Die Farbe des Shirts, kann veraendert werden.                                // Bild vom Haendlerin / Die Farbe des Shirts, kann veraendert werden.
  0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,101,1,0,1,101,100,100,100,101,100,100,100,1,1,100,100,101,20,101,101,100,100,1,1,100,5,5,20,20,5,5,100,1,1,100,101,1,20,20,1,101,100,1,1,100,20,1,20,20,1,20,100,1,1,100,2,20,20,20,20,2,100,1,0,1,102,102,20,20,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,20,105,103,103,103,103,105,20,1,0,1,105,104,104,104,104,105,1,0,0,1,105,104,104,104,104,105,1,0,1,105,105,104,104,104,104,105,105,1,0,1,105,9,10,10,9,105,1,0,0,0,1,1,1,1,1,1,0,0 
};

// ========================================================================================
// TRADER
// ----------------------------------------------------------------------------------------

// '0' bedeutet immer nicht belegt.
// ========================================================================================
// ID 1
// Name des Handlers
const PROGMEM char mTrader01Name[] = "Surie";
// Kurze Beschreibung
const PROGMEM char mTrader01Description[] = "Verkaeuferin";
// Dinge zum verkauf
const PROGMEM byte mTrader01Items[4] = { ITEM_CAMERA, 0, 0, 0 };
// Farben: Haare 1, Haare 2, T-Shirt 1, T-Shirt 2, Hose 1, Hose 2 (Farbnummer 100 bis 105)
const PROGMEM uint16_t mTrader01Colors[6] = { 0xEEEC, 0xE662, 0xD69A, 0xB596, 0x0418, 0x0312 };

// ========================================================================================
// Box
// ----------------------------------------------------------------------------------------

// ========================================================================================
// '0' bedeutet immer nicht belegt.

// ========================================================================================
// ID 1
// Name der Kiste
const PROGMEM char mBox01Name[] = "Meine Kiste";
// Kurze Beschreibung
const PROGMEM char mBox01Description[] = "Dinge die man so braucht.";
const PROGMEM char mBox01FoundText[] = "Du findest 75 Muenzen!";
const PROGMEM char mBox01EmptyText[] = "Die Kiste ist leer.";

// ========================================================================================
// Functionsvariablen

byte mChestIndex = NO_TILE;                                            // Kachel Index der geoeffneten Kiste
int16_t mLastStateCoins = -1;                                          // zuletzt angezeigter Muenzstand

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Liefert die Farbnummer von Surie an einer Bildschirm Position.
// ----------------------------------------------------------------------------------------
// x, y     = Bildschirm Position
// Rueckgabe = Farbnummer, 0 = transparent / nicht getroffen
byte getNpcPixel(int x, int y) {

  if(!mNpcActive) {
    return 0;
  }

  int localX = x - mNpcX;
  int localY = y - mNpcY;

  if(localX < 0 || localX >= FIGURE_WIDTH || localY < 0 || localY >= FIGURE_HEIGHT) {
    return 0;
  }

  return pgm_read_byte(mTraderSpriteFrontWomen + localY * FIGURE_WIDTH + localX);
}

// ========================================================================================
// Liefert die veraenderbaren Farben von Surie (Farbnummer 100 bis 105).
uint16_t getNpcColor(byte c) {

  if(c < 100 || c > 105) {
    return ST7735_RED;
  }

  return pgm_read_word(&mTrader01Colors[c - 100]);
}

// ========================================================================================
// Gespraech mit Surie. Beim ersten Mal gibt es den Auftrag, danach wird gehandelt.
void openTraderWindow() {

  if(mQuestState == QUEST_START) {
    mQuestState = QUEST_TALKED;
    openWindow(WIN_MESSAGE, mTrader01Name, mTraderIntroText);
    drawGoal(false);
    return;
  }

  openWindow(WIN_CHOICE, mTrader01Name, 
             mQuestState == QUEST_DONE ? mTraderThanksText : mTraderStartText);

  if(hasItem(ITEM_PHOTO)) {
    addWindowOption(OPT_GIVE_PHOTO);
  }

  for(byte i = 0; i < 4; i++) {                                        // Dinge zum Verkauf anbieten
    if(pgm_read_byte(&mTrader01Items[i]) == ITEM_CAMERA && !hasItem(ITEM_CAMERA)) {
      addWindowOption(OPT_BUY_CAMERA);
    }
  }

  if(hasItem(ITEM_CAMERA) && mQuestState == QUEST_DONE) {              // erst nach dem Auftrag, sonst fehlen Muenzen
    addWindowOption(OPT_SELL_CAMERA);
  }

  addWindowOption(OPT_BYE);
}

// ========================================================================================
// Kauft ein Item bei Surie.
void traderBuy(uint16_t itemId) {

  int16_t price = getItemBuyValue(itemId);

  if(mCoins < price) {
    openWindow(WIN_MESSAGE, mTrader01Name, mTraderNotEnough);
    return;
  }

  if(!addItem(itemId)) {
    openWindow(WIN_MESSAGE, mTrader01Name, mBackpackFullText);
    return;
  }

  addCoins(-price);
  openWindow(WIN_MESSAGE, mTrader01Name, mTraderBoughtText);
}

// ========================================================================================
// Verkauft ein Item an Surie.
void traderSell(uint16_t itemId) {

  if(removeItem(itemId)) {
    addCoins(getItemSellValue(itemId));
  }

  openWindow(WIN_MESSAGE, mTrader01Name, mTraderSoldText);
}

// ========================================================================================
// Surie bekommt das Foto. Danach wird das Abschluss Fenster gezeigt.
void traderTakePhoto() {

  removeItem(ITEM_PHOTO);
  mQuestState = QUEST_DONE;
  addCoins(PHOTO_REWARD);
  openWindow(WIN_MESSAGE, mTrader01Name, mTraderPhotoText);
  mWindowShowEndNext = true;
}

// ========================================================================================
// Die Figur steht vor der Kiste.
// ----------------------------------------------------------------------------------------
// index = Kachel Index der Kiste
void openChestWindow(byte index) {

  mChestIndex = index;

  if(isTileConsumed(index)) {
    openWindow(WIN_MESSAGE, mBox01Name, mBox01EmptyText);
    return;
  }

  openWindow(WIN_CHOICE, mBox01Name, mBox01Description);
  addWindowOption(OPT_OPEN_CHEST);
  addWindowOption(OPT_KEEP_CLOSED);
}

// ========================================================================================
// Oeffnet die Kiste und nimmt den Inhalt heraus.
void openChest() {

  consumeTile(mChestIndex);                                            // Kiste ist jetzt leer
  addCoins(CHEST_COINS);
  openWindow(WIN_MESSAGE, mBox01Name, mBox01FoundText);
}

// ========================================================================================
// Muenzen hinzufuegen (oder mit negativem Wert abziehen).
void addCoins(int16_t value) {

  mCoins += value;
  drawCoinsStatus(false);
  drawGoal(false);
}

// ========================================================================================
// rendert unten rechts den Coin Stand.
// redraw = zeichnet den stand ohne veränderung des Coin status neu.
void drawCoinsStatus(bool redraw) {

  if(mCoins != mLastStateCoins || redraw) {
    EsploraTFT.fillRect(51, HUD_POS_Y + 1, MAP_WIDTH - 51, 9, colorOf(1));
    drawIcon(51, HUD_POS_Y + 2, 7, 7, mCoinSpiteIcon, 1);
    drawNumber(61, HUD_POS_Y + 2, mCoins, colorOf(12));
    mLastStateCoins = mCoins;
  }
}
