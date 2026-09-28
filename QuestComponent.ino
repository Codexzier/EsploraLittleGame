// ========================================================================================
// Description:       Spielablauf "Suries Foto".
//                    Hier wird entschieden, was beim Anstossen oder Betreten einer
//                    Kachel passiert und welches Ziel gerade angezeigt wird.
// ----------------------------------------------------------------------------------------
// Ablauf:            1. Schluessel finden               (Haus, linker Raum)
//                    2. Tuer oeffnen                    (Schluessel wird verbraucht)
//                    3. Mit Surie sprechen              (Auftrag: Foto von Baum und Haus)
//                    4. 200 Muenzen sammeln             (Start 25, Kiste 75, Garten 4 x 25)
//                    5. Kamera bei Surie kaufen
//                    6. Am Fotopunkt im Garten ein Foto machen
//                    7. Foto bei Surie abgeben          (Belohnung 150 Muenzen)
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
const PROGMEM char mPhotoSpotTitle[] = "Fotopunkt";
const PROGMEM char mPhotoSpotText[] = "Baum und Haus im Sonnenschein - ein schoenes Motiv!";
const PROGMEM char mPhotoSpotNoCameraText[] = "Baum und Haus im Sonnenschein - ein schoenes Motiv! Haette ich nur eine Kamera...";
const PROGMEM char mPhotoTakenText[] = "Klick! Das Foto ist jetzt im Rucksack.";
const PROGMEM char mBackpackFullText[] = "Der Rucksack ist voll.";
const PROGMEM char mEndTitle[] = "Geschafft!";
const PROGMEM char mEndText[] = "Surie freut sich ueber ihr Foto. Danke fuers Spielen! Halte [1] fuer ein neues Spiel.";

// ----------------------------------------------------------------------------------------
// Ziele (zwei Zeilen mit je hoechstens 18 Zeichen)

#define GOAL_COUNT 8
const PROGMEM char mGoalTexts[GOAL_COUNT][2][19] = {
  { "Finde den",          "Schluessel."        },
  { "Oeffne die Tuer.",   ""                   },
  { "Sprich mit Surie.",  ""                   },
  { "Sammle 200 Muenzen", "fuer die Kamera."   },
  { "Kaufe die Kamera",   "bei Surie."         },
  { "Fotografiere Baum",  "und Haus (Garten)." },
  { "Bring Surie",        "das Foto."          },
  { "Geschafft!",         "[1] halten = Neu"   },
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
    openTraderWindow();
    return;
  }

  if(blocker == MOVE_BLOCKED_BORDER) {
    return;
  }

  switch(getTile(blocker)) {
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
      openWindow(WIN_MESSAGE, mHouseDoorTitle, mHouseDoorText);
      break;
    }
    default: { break; }
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
      if(hasItem(ITEM_PHOTO) || mQuestState == QUEST_DONE) {
        openWindow(WIN_MESSAGE, mPhotoSpotTitle, mPhotoSpotText);
      }
      else if(hasItem(ITEM_CAMERA)) {
        openWindow(WIN_CHOICE, mPhotoSpotTitle, mPhotoSpotText);
        addWindowOption(OPT_TAKE_PHOTO);
        addWindowOption(OPT_NOT_NOW);
      }
      else {
        openWindow(WIN_MESSAGE, mPhotoSpotTitle, mPhotoSpotNoCameraText);
      }
      break;
    }
    default: { break; }
  }
}

// ========================================================================================
// Eine Auswahl im Fenster wurde mit Button 4 bestaetigt.
// ----------------------------------------------------------------------------------------
// option = Id der Auswahl (OPT_...)
void onWindowChoice(byte option) {

  switch(option) {
    case(OPT_OPEN_CHEST):  { openChest(); break; }
    case(OPT_BUY_CAMERA):  { traderBuy(ITEM_CAMERA); break; }
    case(OPT_SELL_CAMERA): { traderSell(ITEM_CAMERA); break; }
    case(OPT_GIVE_PHOTO):  { traderTakePhoto(); break; }
    case(OPT_TAKE_PHOTO): {
      if(addItem(ITEM_PHOTO)) {
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
// Oeffnet das Abschluss Fenster.
void openEndWindow() {
  openWindow(WIN_END, mEndTitle, mEndText);
}

// ========================================================================================
// Ist die Tuer im Haus bereits offen.
bool isHouseDoorOpen() {
  return (mTileConsumed[MAP_HOUSE][DOOR_TILE_INDEX >> 3] & (1 << (DOOR_TILE_INDEX & 7))) != 0;
}

// ========================================================================================
// Ermittelt das aktuelle Ziel aus dem Spielstand.
byte getGoal() {

  if(mQuestState == QUEST_DONE)         { return 7; }
  if(hasItem(ITEM_PHOTO))               { return 6; }
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
