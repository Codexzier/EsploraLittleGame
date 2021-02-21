// ========================================================================================
// Description:       Die Haendler und Kisten Funktionen werden hier unter gebracht.
//                    Ein Handler kann verschiedene Rollen haben.
//                    - Verkaeufer
// ========================================================================================

// ========================================================================================
// Functionsvariablen, temp Variablen zum zwischen laden.
char mTraderName[1];
char mTraderdescription[1];
byte mTraderItems[4];

// ========================================================================================
// Coins

// Muenzen im Besitz
int16_t mCoins = 25;
int16_t mLastStateCoins = 0;

// ========================================================================================
// Common Text

const PROGMEM char mTraderStartText[] = "Hallo, was darf ich ihnen verkaufen?";    // Begruessungstext (Sollte immer verschieden sein.)
const PROGMEM char mTraderNotEnough[] = "Du hast nicht genug Muenzen.";            // Wenn zu wenig Muenzen zum Kaufen da sind
const PROGMEM char mTraderYouWantToBuy[] = "Kaufen?";                              // Frage zum Kauf

// ========================================================================================
// Common Sprite

const PROGMEM byte mTraderSpriteFrontMen[160] = {                                  // Bild vom Haendler / Die Farbe des Shirts, kann veraendert werden.
  0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,100,1,0,1,100,100,100,100,101,100,100,100,1,1,100,100,101,2,101,101,100,100,1,1,100,5,5,2,2,5,5,100,1,1,101,101,1,2,2,1,101,100,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,0,0,0,1,102,102,3,3,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,2,1,102,102,102,102,1,2,1,0,1,1,104,105,105,104,1,1,0,0,0,1,104,105,105,104,1,0,0,0,0,1,104,105,105,104,1,0,0,0,0,1,9,10,10,9,1,0,0,0,0,0,1,1,1,1,0,0,0 
};
const PROGMEM byte mTraderSpriteFrontWomen[160] = {                                // Bild vom Haendlerin / Die Farbe des Shirts, kann veraendert werden.
  0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,101,1,0,1,101,100,100,100,101,100,100,100,1,1,100,100,101,20,101,101,100,100,1,1,100,5,5,20,20,5,5,100,1,1,100,101,1,20,20,1,101,100,1,1,100,20,1,20,20,1,20,100,1,1,100,2,20,20,20,20,2,100,1,0,1,102,102,20,20,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,20,105,103,103,103,103,105,20,1,0,1,105,104,104,104,104,105,1,0,0,1,105,104,104,104,104,105,1,0,1,105,105,104,104,104,104,105,105,1,0,1,105,9,10,10,9,105,1,0,0,0,1,1,1,1,1,1,0,0 
};
const PROGMEM byte mBoxSpriteFront[100] = { 
  1,1,1,1,1,1,1,1,1,1,1,106,106,1,1,1,1,106,106,1,1,106,106,1,12,12,1,106,106,1,1,106,106,1,12,12,1,106,106,1,1,1,1,1,1,1,1,1,1,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,1,1,1,1,1,1,1,1,1 
};
const PROGMEM byte mCoinSpiteIcon[49] = { 
  0,14,12,12,12,5,0,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,0,14,12,12,12,5,0 
};

// ========================================================================================
// TRADER
// ----------------------------------------------------------------------------------------

// '0' bedeutet immer nicht belegt.
// ========================================================================================
// ID 1
// Name des Handlers
const PROGMEM char mTrader01Name[5] = "Surie";
// Kurze Beschreibung
const PROGMEM char mTrader01Description[11] = "Verkaeferin";
// Dinge zum verkauf
const PROGMEM byte mTrader01Items[4] = { 2, 0, 0, 0 }; // 2 = Kamera
// 0 = Taschenplaetze werden wie angegeben befullt. 
// Stellen werden Stellenweise in Bit herausgenommen
byte mTrader01ItemsClear = 0; 

// ========================================================================================
// Box
// ----------------------------------------------------------------------------------------

// ========================================================================================
// '0' bedeutet immer nicht belegt.

// ========================================================================================
// ID 1
// Name des Handlers
const PROGMEM char mBox01Name[11] = "Meine Kiste";
// Kurze Beschreibung
const PROGMEM char mBox01Description[25] = "Dinge die man so braucht.";
// Dinge zum verkauf
const PROGMEM byte mBox01Items[4] = { 3, 0, 0, 0 }; // 3 = Foto

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// ----------------------------------------------------------------------------------------
// arrayContent[]   = 
// traderItemsClear =
void memCopyItems(byte arrayContent[], byte traderItemsClear) {

  if(traderItemsClear == 128) { 
    traderItemsClear-= 128;
    mTraderItems[0] = 0;
  }
  else {
    mTraderItems[0] = pgm_read_byte_near(arrayContent + 0);
  }

  if(traderItemsClear >= 64) { 
    traderItemsClear-= 64;
    mTraderItems[1] = 0;
  }
  else {
    mTraderItems[1] = pgm_read_byte_near(arrayContent + 1);
  }

  if(traderItemsClear >= 32) { 
    traderItemsClear-= 32;
    mTraderItems[2] = 0;
  }
  else {
    mTraderItems[2] = pgm_read_byte_near(arrayContent + 2);
  }

  if(traderItemsClear >= 16) { 
    traderItemsClear-= 16;
    mTraderItems[3] = 0;
  }
  else {
    mTraderItems[3] = pgm_read_byte_near(arrayContent + 3);
  }
}

// ========================================================================================
// Zeichnet den Handler/in mit dem passenden sprite
// ----------------------------------------------------------------------------------------
// traderId
// positionX =
// positionY =
void drawTrader(int16_t traderId, int16_t positionX, int16_t positionY) {

  if(!mMapFigureRerender) {
    return;
  }

  // zurueck setzen
  mMapFigureRerender = false;
  
  switch(traderId) {
    case(1): {
      // Farbe des Haenderls/in
      mSpriteHairColor1 =  0xEEEC; mSpriteHairColor2 = 0xE662;  // hell Braun 1, hell braun 2
      mSpriteShirtColor1 = 0xD69A; mSpriteShirtColor2 = 0xB596;  // hell grau, grau
      mSpritePantsColor1 = 0x0418; mSpritePantsColor2 = 0x0312;  // Blau 1, blau
      memCopy(mTraderSpriteFrontWomen);                         // sprite einer Weiblichen figur
      memCopyItems(mTrader01Items, mTrader01ItemsClear);         // Taschen Inhalt
      break;
    }
    case(2): {
      // Farbe des Haenderls/in
      mSpriteHairColor1 = 0xD615;  mSpriteHairColor2 = 0xBD30;  // hell Braun 1, hell braun 2
      mSpriteShirtColor1 = 0xD69A; mSpriteShirtColor2 = 0xB596; // hell grau, grau
      mSpritePantsColor1 = 0x0418; mSpritePantsColor2 = 0x0312; // Blau 1, blau
      memCopy(mTraderSpriteFrontMen);                          // sprite einer maenlichen figur
      // Taschen Inhalt
      break;
    }
    default: {
      break;
    }
  }
  
  drawTile(positionX, positionY, 10, 16, mTempArray, false); 
}

// ========================================================================================
// Zeichnet die Box
void drawBox(int16_t boxId, int16_t positionX, int16_t positionY) {
  switch(boxId) {
    case(1): {
      mBoxColor = 0xDCFE;
      break;
    }
    default: { break; }
  }

  memCopy(mBoxSpriteFront); 
  drawTile(positionX, positionY, 10, 10, mTempArray, false);
}

// ========================================================================================
// rendert oben links die Coin Stand.
// redraw = zeichnet den stand ohne veränderung des Coin status neu.
void drawCoinsStatus(bool redraw) {
  if(mCoins != mLastStateCoins || redraw) {
    EsploraTFT.fillRect(2, 2, 30, 9, mapNumberToColor(1));
    memCopy(mCoinSpiteIcon);
    drawTile(3, 3, 7, 7, mTempArray, false);
    writeValue(12, 3, mCoins, false);
    mLastStateCoins = mCoins;
  }
}
