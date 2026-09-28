// ========================================================================================
// Description:       Die Haendler und Kisten Funktionen werden hier unter gebracht.
//                    Ein Handler kann verschiedene Rollen haben.
//                    - Verkaeufer (Surie: Kamera, Kapitaen: Bootsticket, Busfahrer: Busticket)
//                    - Auftraggeber (Surie wuenscht sich Fotos)
// ========================================================================================

// ========================================================================================
// Common Text

const PROGMEM char mTraderIntroText[] = "Hallo, ich bin Surie! Bring mir ein Foto von Baum und Haus im Garten. Eine Kamera kostet 200 Muenzen.";
const PROGMEM char mTraderStartText[] = "Hallo, was darf ich dir verkaufen?";      // Begruessungstext
const PROGMEM char mTraderWaitingText[] = "Bruecke, Insel und Stadt - hast du die Fotos schon?";
const PROGMEM char mTraderNotEnough[] = "Du hast nicht genug Muenzen.";            // Wenn zu wenig Muenzen zum Kaufen da sind
const PROGMEM char mTraderBoughtText[] = "Danke! Viel Spass mit der Kamera.";
const PROGMEM char mTraderTicketText[] = "Hier ist dein Ticket. Gute Fahrt!";
const PROGMEM char mTraderSoldText[] = "Danke, ich nehme sie gerne zurueck.";
const PROGMEM char mTraderPhotoText[] = "Wunderschoen! Genau so wollte ich es. Hier sind 150 Muenzen fuer dich!";
const PROGMEM char mTraderMorePhotosText[] = "Ich wuensche mir noch drei Fotos: Bruecke, Insel und Stadt. Hinter dem Garten geht es weiter!";
const PROGMEM char mTraderAllPhotosText[] = "Oh, sind die schoen! Damit schmuecke ich die Waende in meinem Haus.";
const PROGMEM char mTraderInviteText[] = "Besuch mich doch in meinem Haus im Garten. Ich koche uns einen Kaffee!";
const PROGMEM char mTraderHomeText[] = "Schau, deine Fotos haengen an der Wand! Magst du einen Kaffee?";
const PROGMEM char mTraderCoffeeText[] = "Mmh, lecker! Ihr erzaehlt von der Bruecke, der Insel und der Stadt.";
const PROGMEM char mTraderAfterCoffeeText[] = "Schoen, dass du mich besucht hast!";

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
// Dinge zum verkauf: Kamera
// Farben: Haare 1, Haare 2, T-Shirt 1, T-Shirt 2, Hose 1, Hose 2 (Farbnummer 100 bis 105)
const PROGMEM uint16_t mTrader01Colors[6] = { 0xEEEC, 0xE662, 0xD69A, 0xB596, 0x0418, 0x0312 };

// ========================================================================================
// ID 2 Kapitaen (verkauft das Bootsticket, Figur: mTraderSpriteFrontMen)
const PROGMEM char mTrader02Name[] = "Kapitaen";
const PROGMEM uint16_t mTrader02Colors[6] = { 0xFFFF, 0xC618, 0x10A8, 0x0864, 0x2104, 0x18C3 };  // weisse Haare, Marine Blau

// ========================================================================================
// ID 3 Busfahrer (verkauft das Busticket, Figur: mTraderSpriteFrontMen)
const PROGMEM char mTrader03Name[] = "Busfahrer";
const PROGMEM uint16_t mTrader03Colors[6] = { 0x81E1, 0x6180, 0x84B6, 0x6B4D, 0x7BEF, 0x5ACB };  // braune Haare, hell blau, grau

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
// Liefert die Farbnummer der Figur auf der Karte an einer Bildschirm Position.
// Surie hat ein eigenes Sprite, Kapitaen und Busfahrer teilen sich eines.
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

  const byte* sprite = (mNpcType == NPC_SURIE) ? mTraderSpriteFrontWomen : mTraderSpriteFrontMen;
  return getPackedPixel(sprite, localY * FIGURE_WIDTH + localX);
}

// ========================================================================================
// Liefert die veraenderbaren Farben der Figur (Farbnummer 100 bis 105).
uint16_t getNpcColor(byte c) {

  if(c < 100 || c > 105) {
    return ST7735_RED;
  }

  switch(mNpcType) {
    case(NPC_CAPTAIN): { return pgm_read_word(&mTrader02Colors[c - 100]); }
    case(NPC_DRIVER):  { return pgm_read_word(&mTrader03Colors[c - 100]); }
    default:           { return pgm_read_word(&mTrader01Colors[c - 100]); }
  }
}

// ========================================================================================
// Name der Figur auf der aktuellen Karte (Titel im Fenster).
const char* getNpcName() {

  switch(mNpcType) {
    case(NPC_CAPTAIN): { return mTrader02Name; }
    case(NPC_DRIVER):  { return mTrader03Name; }
    default:           { return mTrader01Name; }
  }
}

// ========================================================================================
// Gespraech mit Surie. Beim ersten Mal gibt es den Auftrag, danach wird gehandelt.
// In ihrem Haus gibt es zum Schluss einen Kaffee.
void openTraderWindow() {

  if(mQuestState == QUEST_START) {
    mQuestState = QUEST_TALKED;
    openWindow(WIN_MESSAGE, mTrader01Name, mTraderIntroText);
    drawGoal(false);
    return;
  }

  if(mCurrentMap == MAP_LIVING_ROOM) {                                 // Besuch bei Surie zu Hause
    if(mQuestState == QUEST_COFFEE) {
      openWindow(WIN_MESSAGE, mTrader01Name, mTraderAfterCoffeeText);
      return;
    }

    openWindow(WIN_CHOICE, mTrader01Name, mTraderHomeText);
    addWindowOption(OPT_DRINK_COFFEE);
    addWindowOption(OPT_NOT_NOW);
    return;
  }

  openWindow(WIN_CHOICE, mTrader01Name,
             mQuestState == QUEST_MORE_PHOTOS ? mTraderWaitingText : mTraderStartText);

  if(hasItem(ITEM_PHOTO)) {
    addWindowOption(OPT_GIVE_PHOTO);
  }

  if(mQuestState == QUEST_MORE_PHOTOS && hasAllNewPhotos()) {
    addWindowOption(OPT_GIVE_PHOTOS);
  }

  if(!hasItem(ITEM_CAMERA)) {                                          // Kamera zum Verkauf anbieten
    addWindowOption(OPT_BUY_CAMERA);
  }

  if(hasItem(ITEM_CAMERA) && mQuestState >= QUEST_PHOTOS_GIVEN) {      // erst nach allen Fotos, sonst fehlen Muenzen
    addWindowOption(OPT_SELL_CAMERA);
  }

  addWindowOption(OPT_BYE);
}

// ========================================================================================
// Kauft ein Item bei der Figur auf der Karte (Kamera oder Ticket).
void traderBuy(uint16_t itemId) {

  int16_t price = getItemBuyValue(itemId);

  if(mCoins < price) {
    openWindow(WIN_MESSAGE, getNpcName(), mTraderNotEnough);
    return;
  }

  if(!addItem(itemId)) {
    openWindow(WIN_MESSAGE, getNpcName(), mBackpackFullText);
    return;
  }

  addCoins(-price);
  openWindow(WIN_MESSAGE, getNpcName(),
             itemId == ITEM_CAMERA ? mTraderBoughtText : mTraderTicketText);
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
// Surie bekommt das erste Foto und wuenscht sich drei weitere.
void traderTakePhoto() {

  removeItem(ITEM_PHOTO);
  mQuestState = QUEST_MORE_PHOTOS;                                     // Hecke im Garten oeffnet sich
  addCoins(PHOTO_REWARD);
  openWindow(WIN_MESSAGE, mTrader01Name, mTraderPhotoText);
  setWindowFollow(WIN_MESSAGE, mTrader01Name, mTraderMorePhotosText);
}

// ========================================================================================
// Surie bekommt die drei Fotos, geht nach Hause und haengt sie auf.
void traderTakeAllPhotos() {

  removeItem(ITEM_PHOTO_BRIDGE);
  removeItem(ITEM_PHOTO_ISLAND);
  removeItem(ITEM_PHOTO_CITY);
  mQuestState = QUEST_PHOTOS_GIVEN;
  mMapNeedsReload = true;                                              // Surie verlaesst den Laden

  openWindow(WIN_MESSAGE, mTrader01Name, mTraderAllPhotosText);
  setWindowFollow(WIN_MESSAGE, mTrader01Name, mTraderInviteText);
}

// ========================================================================================
// Kaffee mit Surie, danach ist das Spiel geschafft.
void traderDrinkCoffee() {

  mQuestState = QUEST_COFFEE;
  openWindow(WIN_MESSAGE, mTrader01Name, mTraderCoffeeText);
  setWindowFollow(WIN_END, mEndTitle, mEndText);
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
    drawPackedIcon(51, HUD_POS_Y + 2, 7, 7, mCoinSpiteIcon, 1);
    drawNumber(61, HUD_POS_Y + 2, mCoins, colorOf(12));
    mLastStateCoins = mCoins;
  }
}
