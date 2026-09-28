// ========================================================================================
// Description:       Spielablauf "Suries Fotos".
//                    Hier wird entschieden, was beim Anstossen oder Betreten einer
//                    Kachel passiert und welches Ziel gerade angezeigt wird.
// ----------------------------------------------------------------------------------------
// Ablauf Teil 1:     1. Schluessel finden               (Haus, linker Raum)
//                    2. Tuer oeffnen                    (Schluessel wird verbraucht)
//                    3. Mit Surie sprechen              (Auftrag: Foto von Baum und Haus)
//                    4. 200 Muenzen sammeln             (Start 25, Kiste 75, Garten 4 x 25)
//                    5. Kamera bei Surie kaufen
//                    6. Am Fotopunkt im Garten ein Foto machen
//                    7. Foto bei Surie abgeben          (150 Muenzen, neuer Auftrag: drei Fotos)
// Ablauf Teil 2:     8. Foto der Bruecke                (Fluss, hinter der Hecke im Garten)
//                    9. Foto der Insel                  (von der Nachbarinsel aus)
//                   10. Bootsticket beim Kapitaen       (60 Muenzen, Rundfahrt mit Seekarte)
//                   11. Foto der Stadt                  (Stadtrand)
//                   12. Busticket beim Busfahrer        (40 Muenzen, Busfahrt)
//                   13. Am Stadtrand fotografieren
//                   14. Fotos bei Surie abgeben         (sie haengt sie in ihrem Haus auf)
//                   15. Surie in ihrem Haus besuchen und Kaffee trinken
// ========================================================================================

// ========================================================================================
// Variablen

#define DOOR_TILE_INDEX 45                                             // Tuer im Haus (Zeile 4, Spalte 5)

byte mLastGoal = 255;                                                  // zuletzt angezeigtes Ziel

// ========================================================================================
// Texte

const PROGMEM char mDoorTitle[] = "Tuer";
const PROGMEM char mDoorLockedText[] = "Die Tuer ist verschlossen. Irgendwo muss ein Schluessel sein.";
const PROGMEM char mDoorOpenedText[] = "Der Schluessel passt! Die Tuer ist jetzt offen.";
const PROGMEM char mKeyFoundText[] = "Ein Schluessel! Damit laesst sich bestimmt eine Tuer oeffnen.";
const PROGMEM char mHouseDoorTitle[] = "Haustuer";
const PROGMEM char mHouseDoorText[] = "Es ist niemand zu Hause.";
const PROGMEM char mTableTitle[] = "Tisch";
const PROGMEM char mTableText[] = "Zwei Tassen Kaffee und ein Stueck Kuchen stehen bereit.";
const PROGMEM char mPictureTitle[] = "Foto an der Wand";
const PROGMEM char mPhotoSpotTitle[] = "Fotopunkt";
const PROGMEM char mPhotoSpotText[] = "Baum und Haus im Sonnenschein - ein schoenes Motiv!";
const PROGMEM char mPhotoSpotBridgeText[] = "Die alte Bruecke ueber den Fluss - ein schoenes Motiv!";
const PROGMEM char mPhotoSpotIslandText[] = "Da drueben liegt eine kleine Insel mitten im Meer - ein schoenes Motiv!";
const PROGMEM char mPhotoSpotCityText[] = "Die Stadt mit ihren vielen Haeusern - ein schoenes Motiv!";
const PROGMEM char mPhotoSpotNoCameraText[] = "Ein schoenes Motiv! Haette ich nur eine Kamera...";
const PROGMEM char mPhotoTakenText[] = "Klick! Das Foto ist jetzt im Rucksack.";
const PROGMEM char mBackpackFullText[] = "Der Rucksack ist voll.";
const PROGMEM char mEndTitle[] = "Geschafft!";
const PROGMEM char mEndText[] = "Die Fotos haengen an der Wand und ihr trinkt Kaffee. Danke fuers Spielen! [1] halten = Neues Spiel";

// ----------------------------------------------------------------------------------------
// Ziele (zwei Zeilen mit je hoechstens 18 Zeichen)

#define GOAL_COUNT 15
const PROGMEM char mGoalTexts[GOAL_COUNT][2][19] = {
  { "Finde den",          "Schluessel."        },                      // 0
  { "Oeffne die Tuer.",   ""                   },                      // 1
  { "Sprich mit Surie.",  ""                   },                      // 2
  { "Sammle 200 Muenzen", "fuer die Kamera."   },                      // 3
  { "Kaufe die Kamera",   "bei Surie."         },                      // 4
  { "Fotografiere Baum",  "und Haus (Garten)." },                      // 5
  { "Bring Surie",        "das Foto."          },                      // 6
  { "Fotografiere die",   "Bruecke (Osten)."   },                      // 7
  { "Kaufe am Hafen",     "ein Bootsticket."   },                      // 8
  { "Fotografiere die",   "Insel im Meer."     },                      // 9
  { "Kaufe ein Ticket",   "beim Busfahrer."    },                      // 10
  { "Fotografiere die",   "Stadt."             },                      // 11
  { "Bring Surie",        "die drei Fotos."    },                      // 12
  { "Besuche Surie in",   "ihrem Haus."        },                      // 13
  { "Geschafft!",         "[1] halten = Neu"   },                      // 14
};

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Die Figur stoesst gegen etwas, das den Weg versperrt.
// ----------------------------------------------------------------------------------------
// blocker = Kachel Index oder MOVE_BLOCKED_NPC / MOVE_BLOCKED_BORDER
void onBump(byte blocker) {

  if(blocker == MOVE_BLOCKED_NPC) {
    talkToNpc();
    return;
  }

  if(blocker == MOVE_BLOCKED_BORDER) {
    return;
  }

  byte tile = getTile(blocker);
  switch(tile) {
    case(TILE_DOOR): {
      if(removeItem(ITEM_KEY)) {                                       // Schluessel wird verbraucht
        consumeTile(blocker);                                          // Tuer ist offen
        openWindow(WIN_MESSAGE, mDoorTitle, mDoorOpenedText);
        drawGoal(false);
      }
      else {
        openWindow(WIN_MESSAGE, mDoorTitle, mDoorLockedText);
      }
      break;
    }
    case(TILE_CHEST): {
      openChestWindow(blocker);
      break;
    }
    case(TILE_HOUSE_DOOR): {
      if(mQuestState >= QUEST_PHOTOS_GIVEN) {                          // Surie ist zu Hause
        mFacingX = 0;
        mFacingY = -1;
        loadMap(MAP_LIVING_ROOM, 67, 64);
      }
      else {
        openWindow(WIN_MESSAGE, mHouseDoorTitle, mHouseDoorText);
      }
      break;
    }
    case(TILE_BOAT):                                                   // Boot und Schild gehoeren
    case(TILE_BUS_SIGN): {                                             // zur Figur auf der Karte
      talkToNpc();
      break;
    }
    case(TILE_TABLE): {
      openWindow(WIN_MESSAGE, mTableTitle, mTableText);
      break;
    }
    case(TILE_PICTURE_GARDEN): { openWindow(WIN_MESSAGE, mPictureTitle, mItemPhoto01Description); break; }
    case(TILE_PICTURE_BRIDGE): { openWindow(WIN_MESSAGE, mPictureTitle, mItemPhotoBridgeDescription); break; }
    case(TILE_PICTURE_ISLAND): { openWindow(WIN_MESSAGE, mPictureTitle, mItemPhotoIslandDescription); break; }
    case(TILE_PICTURE_CITY):   { openWindow(WIN_MESSAGE, mPictureTitle, mItemPhotoCityDescription); break; }
    default: { break; }
  }
}

// ========================================================================================
// Spricht die Figur auf der aktuellen Karte an.
void talkToNpc() {

  switch(mNpcType) {
    case(NPC_SURIE):   { openTraderWindow(); break; }
    case(NPC_CAPTAIN): { openCaptainWindow(); break; }
    case(NPC_DRIVER):  { openDriverWindow(); break; }
    default:           { break; }
  }
}

// ========================================================================================
// Die Figur hat eine neue Kachel betreten.
// ----------------------------------------------------------------------------------------
// index = Kachel Index
// tile  = Kachel Typ
void onEnterTile(byte index, byte tile) {

  switch(tile) {
    case(TILE_KEY): {
      if(addItem(ITEM_KEY)) {
        consumeTile(index);
        openWindow(WIN_MESSAGE, mItemKey01, mKeyFoundText);
        drawGoal(false);
      }
      else {
        openWindow(WIN_MESSAGE, mItemKey01, mBackpackFullText);
      }
      break;
    }
    case(TILE_COIN): {
      consumeTile(index);
      addCoins(FLOOR_COINS);
      break;
    }
    case(TILE_EXIT): {
      useMapExit(index);
      break;
    }
    case(TILE_PHOTO_SPOT): {
      openPhotoSpotWindow();
      break;
    }
    default: { break; }
  }
}

// ========================================================================================
// Welches Foto kann auf der aktuellen Karte gemacht werden.
uint16_t getPhotoOfMap() {

  switch(mCurrentMap) {
    case(MAP_RIVER):  { return ITEM_PHOTO_BRIDGE; }
    case(MAP_ISLAND): { return ITEM_PHOTO_ISLAND; }
    case(MAP_CITY):   { return ITEM_PHOTO_CITY; }
    default:          { return ITEM_PHOTO; }
  }
}

// ========================================================================================
// Die Figur steht auf einem Fotopunkt.
void openPhotoSpotWindow() {

  uint16_t photo = getPhotoOfMap();
  const char* text;
  switch(photo) {
    case(ITEM_PHOTO_BRIDGE): { text = mPhotoSpotBridgeText; break; }
    case(ITEM_PHOTO_ISLAND): { text = mPhotoSpotIslandText; break; }
    case(ITEM_PHOTO_CITY):   { text = mPhotoSpotCityText; break; }
    default:                 { text = mPhotoSpotText; break; }
  }

  bool delivered = (photo == ITEM_PHOTO) ? mQuestState >= QUEST_MORE_PHOTOS
                                         : mQuestState >= QUEST_PHOTOS_GIVEN;

  if(hasItem(photo) || delivered) {                                    // Foto gibt es schon
    openWindow(WIN_MESSAGE, mPhotoSpotTitle, text);
  }
  else if(hasItem(ITEM_CAMERA)) {
    openWindow(WIN_CHOICE, mPhotoSpotTitle, text);
    addWindowOption(OPT_TAKE_PHOTO);
    addWindowOption(OPT_NOT_NOW);
  }
  else {
    openWindow(WIN_MESSAGE, mPhotoSpotTitle, mPhotoSpotNoCameraText);
  }
}

// ========================================================================================
// Eine Auswahl im Fenster wurde mit Button 4 bestaetigt.
// ----------------------------------------------------------------------------------------
// option = Id der Auswahl (OPT_...)
void onWindowChoice(byte option) {

  switch(option) {
    case(OPT_OPEN_CHEST):      { openChest(); break; }
    case(OPT_BUY_CAMERA):      { traderBuy(ITEM_CAMERA); break; }
    case(OPT_SELL_CAMERA):     { traderSell(ITEM_CAMERA); break; }
    case(OPT_GIVE_PHOTO):      { traderTakePhoto(); break; }
    case(OPT_GIVE_PHOTOS):     { traderTakeAllPhotos(); break; }
    case(OPT_BUY_BOAT_TICKET): { traderBuy(ITEM_BOAT_TICKET); break; }
    case(OPT_BUY_BUS_TICKET):  { traderBuy(ITEM_BUS_TICKET); break; }
    case(OPT_TRAVEL):          { travelWithNpc(); break; }
    case(OPT_DRINK_COFFEE):    { traderDrinkCoffee(); break; }
    case(OPT_TAKE_PHOTO): {
      if(addItem(getPhotoOfMap())) {
        openWindow(WIN_MESSAGE, mPhotoSpotTitle, mPhotoTakenText);
      }
      else {
        openWindow(WIN_MESSAGE, mPhotoSpotTitle, mBackpackFullText);
      }
      break;
    }
    default: {                                                         // Tschuess, Nicht jetzt, Zu lassen
      closeWindow();
      break;
    }
  }

  drawGoal(false);
}

// ========================================================================================
// Ist die Tuer im Haus bereits offen.
bool isHouseDoorOpen() {
  return (mTileConsumed[MAP_HOUSE][DOOR_TILE_INDEX >> 3] & (1 << (DOOR_TILE_INDEX & 7))) != 0;
}

// ========================================================================================
// Ermittelt das aktuelle Ziel aus dem Spielstand.
byte getGoal() {

  if(mQuestState == QUEST_COFFEE)       { return 14; }
  if(mQuestState == QUEST_PHOTOS_GIVEN) { return 13; }

  if(mQuestState == QUEST_MORE_PHOTOS) {                               // Teil 2: drei Fotos
    if(hasAllNewPhotos())               { return 12; }
    if(!hasItem(ITEM_PHOTO_BRIDGE))     { return 7; }
    if(!hasItem(ITEM_PHOTO_ISLAND))     { return hasItem(ITEM_BOAT_TICKET) ? 9 : 8; }
    return hasItem(ITEM_BUS_TICKET) ? 11 : 10;
  }

  if(hasItem(ITEM_PHOTO))               { return 6; }                  // Teil 1: erstes Foto
  if(hasItem(ITEM_CAMERA))              { return 5; }
  if(!isHouseDoorOpen())                { return hasItem(ITEM_KEY) ? 1 : 0; }
  if(mQuestState == QUEST_START)        { return 2; }
  if(mCoins >= (int16_t)getItemBuyValue(ITEM_CAMERA)) { return 4; }
  return 3;
}

// ========================================================================================
// Zeichnet das aktuelle Ziel unten rechts.
// ----------------------------------------------------------------------------------------
// redraw = zeichnet das Ziel auch ohne Aenderung neu.
void drawGoal(bool redraw) {

  byte goal = getGoal();
  if(goal == mLastGoal && !redraw) {
    return;
  }
  mLastGoal = goal;

  EsploraTFT.fillRect(51, HUD_POS_Y + 12, MAP_WIDTH - 51, 20, colorOf(1));
  drawTextP(51, HUD_POS_Y + 13, mGoalTexts[goal][0], colorOf(19));
  drawTextP(51, HUD_POS_Y + 22, mGoalTexts[goal][1], colorOf(19));
}

// ========================================================================================
// Zeichnet den unteren Bereich: Rucksack, Muenzen und Ziel.
void drawHud() {

  EsploraTFT.fillRect(0, HUD_POS_Y, MAP_WIDTH, EsploraTFT.height() - HUD_POS_Y, colorOf(1));
  drawBackpack();
  drawCoinsStatus(true);
  drawGoal(true);
}
