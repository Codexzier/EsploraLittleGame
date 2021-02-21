// Die Haendler und Kisten Funktionen werden hier unter gebracht.
// Ein Handler kann verschiedene Rollen haben.
// - Verkaeufer

// #######################################
// #          Coins                      #
// #######################################
// Muenzen im Besitz
int16_t coins = 25;
int16_t lastStateCoins = 0;

// #######################################
// #          Common Text                #
// #######################################
// Begruessungstext (Sollte immer verschieden sein.)
const PROGMEM char traderStartText[] = "Hallo, was darf ich ihnen verkaufen?";
// Wenn zu wenig Muenzen zum Kaufen da sind
const PROGMEM char traderNotEnough[] = "Du hast nicht genug Muenzen.";
// Frage zum Kauf
const PROGMEM char traderYouWantToBuy[] = "Kaufen?";

// #######################################
// #          Common Sprite              #
// #######################################
// Bild vom Handler / Die Farbe des Shirts, kann veraendert werden.
const PROGMEM byte traderSpriteFrontMen[160] = { 0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,100,1,0,1,100,100,100,100,101,100,100,100,1,1,100,100,101,2,101,101,100,100,1,1,100,5,5,2,2,5,5,100,1,1,101,101,1,2,2,1,101,100,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,0,0,0,1,102,102,3,3,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,2,1,102,102,102,102,1,2,1,0,1,1,104,105,105,104,1,1,0,0,0,1,104,105,105,104,1,0,0,0,0,1,104,105,105,104,1,0,0,0,0,1,9,10,10,9,1,0,0,0,0,0,1,1,1,1,0,0,0 };
const PROGMEM byte traderSpriteFrontWomen[160] = { 0,0,1,1,1,1,1,1,0,0,0,1,100,100,100,100,100,101,1,0,1,101,100,100,100,101,100,100,100,1,1,100,100,101,20,101,101,100,100,1,1,100,5,5,20,20,5,5,100,1,1,100,101,1,20,20,1,101,100,1,1,100,20,1,20,20,1,20,100,1,1,100,2,20,20,20,20,2,100,1,0,1,102,102,20,20,102,102,1,0,1,102,103,102,102,102,102,103,102,1,1,20,105,103,103,103,103,105,20,1,0,1,105,104,104,104,104,105,1,0,0,1,105,104,104,104,104,105,1,0,1,105,105,104,104,104,104,105,105,1,0,1,105,9,10,10,9,105,1,0,0,0,1,1,1,1,1,1,0,0 };
const PROGMEM byte boxSpriteFront[100] = { 1,1,1,1,1,1,1,1,1,1,1,106,106,1,1,1,1,106,106,1,1,106,106,1,12,12,1,106,106,1,1,106,106,1,12,12,1,106,106,1,1,1,1,1,1,1,1,1,1,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,106,106,106,106,106,106,106,106,1,1,1,1,1,1,1,1,1,1,1 };
const PROGMEM byte coinSpiteIcon[49] = { 0,14,12,12,12,5,0,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,0,14,12,12,12,5,0 };

// #######################################
// #          TRADER                     #
// #######################################
// temp Variablen zum zwischen laden.
char traderName[1];
char traderdescription[1];
byte traderItems[4];

// '0' bedeutet immer nicht belegt.
// #######################################
// ID 1
// Name des Handlers
const PROGMEM char trader01Name[5] = "Surie";
// Kurze Beschreibung
const PROGMEM char trader01Description[11] = "Verkaeferin";
// Dinge zum verkauf
const PROGMEM byte trader01Items[4] = { 2, 0, 0, 0 }; // 2 = Kamera
// 0 = Taschenplaetze werden wie angegeben befullt. 
// Stellen werden Stellenweise in Bit herausgenommen
byte trader01ItemsClear = 0; 

// #######################################
// #          Box                        #
// #######################################
// '0' bedeutet immer nicht belegt.
// #######################################
// ID 1
// Name des Handlers
const PROGMEM char box01Name[11] = "Meine Kiste";
// Kurze Beschreibung
const PROGMEM char box01Description[25] = "Dinge die man so braucht.";
// Dinge zum verkauf
const PROGMEM byte box01Items[4] = { 3, 0, 0, 0 }; // 3 = Foto

// #######################################
// #          Methoden                   #
// #######################################

void memCopyItems(byte arrayContent[], byte traderItemsClear) {

  if(traderItemsClear == 128) { 
    traderItemsClear-= 128;
    traderItems[0] = 0;
  }
  else {
    traderItems[0] = pgm_read_byte_near(arrayContent + 0);
  }

  if(traderItemsClear >= 64) { 
    traderItemsClear-= 64;
    traderItems[1] = 0;
  }
  else {
    traderItems[1] = pgm_read_byte_near(arrayContent + 1);
  }

  if(traderItemsClear >= 32) { 
    traderItemsClear-= 32;
    traderItems[2] = 0;
  }
  else {
    traderItems[2] = pgm_read_byte_near(arrayContent + 2);
  }

  if(traderItemsClear >= 16) { 
    traderItemsClear-= 16;
    traderItems[3] = 0;
  }
  else {
    traderItems[3] = pgm_read_byte_near(arrayContent + 3);
  }
}


// Zeichnet den Handler/in mit dem passenden sprite
void drawTrader(int16_t traderId, int16_t positionX, int16_t positionY) {

  if(!mapFigureRerender) {
    return;
  }

  // zurueck setzen
  mapFigureRerender = false;
  
  switch(traderId) {
    case(1): {
      // Farbe des Haenderls/in
      spriteHairColor1 =  0xEEEC;  spriteHairColor2 = 0xE662;  // hell Braun 1, hell braun 2
      spriteShirtColor1 = 0xD69A; spriteShirtColor2 = 0xB596;  // hell grau, grau
      spritePantsColor1 = 0x0418; spritePantsColor2 = 0x0312;  // Blau 1, blau
      memCopy(traderSpriteFrontWomen);                         // sprite einer Weiblichen figur
      memCopyItems(trader01Items, trader01ItemsClear);         // Taschen Inhalt
      break;
    }
    case(2): {
      // Farbe des Haenderls/in
      spriteHairColor1 = 0xD615;  spriteHairColor2 = 0xBD30;  // hell Braun 1, hell braun 2
      spriteShirtColor1 = 0xD69A; spriteShirtColor2 = 0xB596; // hell grau, grau
      spritePantsColor1 = 0x0418; spritePantsColor2 = 0x0312; // Blau 1, blau
      memCopy(traderSpriteFrontMen);                          // sprite einer maenlichen figur
      // Taschen Inhalt
      break;
    }
    default: {
      break;
    }
  }
  
  drawTile(positionX, positionY, 10, 16, tempArray, false); 
}

// Zeichnet die Box
void drawBox(int16_t boxId, int16_t positionX, int16_t positionY) {
  switch(boxId) {
    case(1): {
      boxColor = 0xDCFE;
      break;
    }
    default: { break; }
  }

  memCopy(boxSpriteFront); 
  drawTile(positionX, positionY, 10, 10, tempArray, false);
}

// rendert oben links die Coin Stand.
// redraw = zeichnet den stand ohne veränderung des Coin status neu.
void drawCoinsStatus(bool redraw) {
  if(coins != lastStateCoins || redraw) {
    EsploraTFT.fillRect(2, 2, 30, 9, mapNumberToColor(1));
    memCopy(coinSpiteIcon);
    drawTile(3, 3, 7, 7, tempArray, false);
    writeValue(12, 3, coins, false);
    lastStateCoins = coins;
  }
}
