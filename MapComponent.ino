// ========================================================================================
// Description:       Rendert den Inhalt der Karte
//                    Die Figur kann sich auf diesen festgelegten Karte frei bewegen.
// ----------------------------------------------------------------------------------------
// Jede Kachel hat eine Textur. 8x8 Muster werden auf die 16x16 Kachel wiederholt.
// Durchsichtige Pixel (0) zeigen den Boden der Karte (Haus: Fliesen, Garten: Gras).
// Die Texturen werden mit tools/sprites.py aus einer Zeichen Grafik erzeugt.
// ========================================================================================

// ========================================================================================
// Karten Informationen

const PROGMEM byte mMapContent[MAP_COUNT][MAP_TILE_COUNT] = {
  {                                                                    // Karte 0: Haus
    1,1,1,1,1,1,1,1,1,1,                                               // 1 = Wand
    1,0,0,0,2,1,0,0,0,1,                                               // 2 = Item Schluessel
    1,0,0,0,0,1,0,6,0,1,                                               // 5 = Tuer
    1,0,0,0,0,1,0,0,0,8,                                               // 6 = Haendlerin Surie
    1,7,0,0,0,5,0,0,0,1,                                               // 7 = Kiste
    1,1,1,1,1,1,1,1,1,1,                                               // 8 = Ausgang zum Garten
  },
  {                                                                    // Karte 1: Garten
    16,16,16,16,16,16,16,16,16,16,                                     // 16 = Hecke
    16, 9,14,11, 9,13,13,13, 9,16,                                     // 9 = Gras, 11 = Baum, 13 = Dach, 14 = Muenze
    16, 9, 9, 9, 9,12,18,12,14,16,                                     // 12 = Hauswand, 18 = Haustuer
     8, 9, 9, 9,15,10,10, 9, 9,16,                                     // 8 = Ausgang zum Haus, 10 = Weg, 15 = Fotopunkt
    16,14,17, 9, 9, 9,10,17,14,16,                                     // 17 = Blumen
    16,16,16,16,16,16,16,16,16,16,
  }
};

const PROGMEM byte mMapGround[MAP_COUNT] = { TILE_FLOOR, TILE_GRASS };  // Boden je Karte

// ----------------------------------------------------------------------------------------
// Ausgaenge: Karte, Kachel Index, Ziel Karte, Ziel Position X, Ziel Position Y

#define MAP_EXIT_COUNT 2
const PROGMEM byte mMapExits[MAP_EXIT_COUNT][5] = {
  { MAP_HOUSE,  39, MAP_GARDEN, 19,  48 },                             // Haus rechts   -> Garten links
  { MAP_GARDEN, 30, MAP_HOUSE,  131, 48 },                             // Garten links  -> Haus rechts
};

// ========================================================================================
// Kachel Texturen 8x8 (werden wiederholt)

const PROGMEM byte mTileFloor[64] = {
  15,15,15,15,15,15,15,8,
  15,15,15,15,15,15,15,8,
  15,15,15,15,15,15,15,8,
  8,8,8,8,8,8,8,8,
  15,15,15,8,15,15,15,15,
  15,15,15,8,15,15,15,15,
  15,15,15,8,15,15,15,15,
  8,8,8,8,8,8,8,8
};

const PROGMEM byte mTileWall[64] = {
  10,10,10,10,10,10,10,9,
  11,10,10,10,10,10,10,9,
  10,10,10,10,10,10,10,9,
  9,9,9,9,9,9,9,9,
  10,10,10,9,10,10,10,10,
  10,10,10,9,11,10,10,10,
  10,10,10,9,10,10,10,10,
  9,9,9,9,9,9,9,9
};

const PROGMEM byte mTileGrass[64] = {
  6,6,6,6,6,6,6,6,
  6,6,7,6,6,6,8,8,
  6,6,6,6,6,6,6,6,
  6,6,6,6,8,6,6,6,
  6,6,6,6,7,6,6,6,
  6,8,6,6,6,6,6,6,
  6,6,6,6,6,7,6,6,
  6,6,6,6,6,6,6,6
};

const PROGMEM byte mTilePath[64] = {
  20,20,20,20,20,20,20,20,
  20,20,4,20,20,20,20,20,
  20,20,20,20,20,20,4,20,
  20,20,20,20,20,20,20,20,
  20,4,20,20,20,20,20,20,
  20,20,20,20,4,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,4
};

const PROGMEM byte mTileHedge[64] = {
  7,6,7,7,7,6,7,7,
  6,7,7,8,7,7,7,6,
  7,7,6,7,7,6,7,7,
  7,6,7,7,7,7,6,7,
  7,7,7,6,7,7,8,7,
  6,7,7,7,6,7,6,7,
  7,8,6,7,7,7,7,6,
  7,7,7,7,6,7,7,7
};

const PROGMEM byte mTileRoof[64] = {
  14,22,22,22,14,22,22,22,
  22,22,22,22,22,22,22,22,
  22,22,22,22,22,22,22,22,
  17,17,17,17,17,17,17,17,
  22,22,14,22,22,22,22,14,
  22,22,22,22,22,22,22,22,
  22,22,22,22,22,22,22,22,
  17,17,17,17,17,17,17,17
};

const PROGMEM byte mTileFlowers[64] = {
  0,0,0,0,0,0,0,0,
  0,0,16,0,0,0,0,0,
  0,16,12,16,0,0,0,0,
  0,0,16,0,0,24,0,0,
  0,0,7,0,24,12,24,0,
  0,0,0,0,0,24,0,0,
  0,0,0,0,0,7,0,0,
  0,0,0,0,0,0,0,0
};

// ========================================================================================
// Kachel Texturen 16x16

const PROGMEM byte mTileDoor[256] = {
  17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,12,12,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,12,1,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,12,1,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,12,12,4,13,17,
  17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,13,13,4,13,13,13,13,4,13,13,13,13,4,13,17,
  17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17
};

const PROGMEM byte mTileChest[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
  0,1,4,4,4,4,4,4,4,4,4,4,4,4,1,0,
  0,1,4,13,13,13,13,13,13,13,13,13,13,4,1,0,
  0,1,4,13,13,13,13,13,13,13,13,13,13,4,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,1,18,4,4,4,4,12,12,4,4,4,4,18,1,0,
  0,1,18,13,13,13,13,12,1,13,13,13,13,18,1,0,
  0,1,18,13,13,13,13,12,12,13,13,13,13,18,1,0,
  0,1,18,13,13,13,13,13,13,13,13,13,13,18,1,0,
  0,1,18,13,13,13,13,13,13,13,13,13,13,18,1,0,
  0,1,18,4,4,4,4,4,4,4,4,4,4,18,1,0,
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const PROGMEM byte mTileExit[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,22,22,22,22,22,22,22,22,22,22,22,22,0,0,
  0,0,22,12,12,12,12,12,12,12,12,12,12,22,0,0,
  0,0,22,12,22,22,22,22,22,22,22,22,12,22,0,0,
  0,0,22,12,22,12,12,12,12,12,12,22,12,22,0,0,
  0,0,22,12,22,22,22,22,22,22,22,22,12,22,0,0,
  0,0,22,12,22,12,12,12,12,12,12,22,12,22,0,0,
  0,0,22,12,22,22,22,22,22,22,22,22,12,22,0,0,
  0,0,22,12,22,12,12,12,12,12,12,22,12,22,0,0,
  0,0,22,12,22,22,22,22,22,22,22,22,12,22,0,0,
  0,0,22,12,22,12,12,12,12,12,12,22,12,22,0,0,
  0,0,22,12,22,22,22,22,22,22,22,22,12,22,0,0,
  0,0,22,12,12,12,12,12,12,12,12,12,12,22,0,0,
  0,0,22,22,22,22,22,22,22,22,22,22,22,22,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const PROGMEM byte mTileTree[256] = {
  0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
  0,0,0,1,1,6,6,6,8,8,6,1,1,0,0,0,
  0,0,1,6,6,6,6,8,8,8,6,6,7,1,0,0,
  0,1,6,6,7,6,6,6,8,8,6,6,6,7,1,0,
  0,1,6,6,6,6,6,6,6,6,6,7,6,7,1,0,
  1,6,6,7,6,6,6,6,7,6,6,6,6,6,7,1,
  1,6,6,6,6,6,7,6,6,6,6,6,7,6,7,1,
  1,7,6,6,6,6,6,6,6,6,7,6,6,6,7,1,
  0,1,7,6,7,6,6,6,7,6,6,6,7,7,1,0,
  0,1,7,7,6,6,7,6,6,6,7,7,7,7,1,0,
  0,0,1,1,7,7,7,7,7,7,7,7,1,1,0,0,
  0,0,0,0,1,1,1,17,3,1,1,1,0,0,0,0,
  0,0,0,0,0,0,1,17,3,1,0,0,0,0,0,0,
  0,0,0,0,0,0,1,17,3,1,0,0,0,0,0,0,
  0,0,0,0,0,1,17,3,3,17,1,0,0,0,0,0,
  0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0
};

const PROGMEM byte mTileHouseWall[256] = {
  18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,1,1,1,1,1,1,1,1,19,19,19,19,
  19,19,19,19,1,24,11,11,1,24,11,1,19,19,19,19,
  19,19,19,19,1,11,11,1,11,11,1,1,19,19,19,19,
  19,19,19,19,1,1,1,1,1,1,1,1,19,19,19,19,
  19,19,19,19,1,11,11,1,11,11,11,1,19,19,19,19,
  19,19,19,19,1,11,11,1,11,11,11,1,19,19,19,19,
  19,19,19,19,1,1,1,1,1,1,1,1,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21
};

const PROGMEM byte mTileHouseDoor[256] = {
  18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,1,1,1,1,1,1,1,1,19,19,19,19,
  19,19,19,1,23,23,23,23,23,23,23,23,1,19,19,19,
  19,19,19,1,23,17,17,23,23,17,17,23,1,19,19,19,
  19,19,19,1,23,17,17,23,23,17,17,23,1,19,19,19,
  19,19,19,1,23,23,23,23,23,23,23,23,1,19,19,19,
  19,19,19,1,23,23,23,23,23,23,23,23,1,19,19,19,
  19,19,19,1,23,23,23,23,23,23,12,23,1,19,19,19,
  19,19,19,1,23,17,17,23,23,17,12,23,1,19,19,19,
  19,19,19,1,23,17,17,23,23,17,17,23,1,19,19,19,
  19,19,19,1,23,17,17,23,23,17,17,23,1,19,19,19,
  19,19,19,1,23,23,23,23,23,23,23,23,1,19,19,19,
  19,19,19,1,23,23,23,23,23,23,23,23,1,19,19,19,
  18,18,18,1,1,1,1,1,1,1,1,1,1,18,18,18,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21
};

const PROGMEM byte mTilePhotoSpot[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,12,12,0,0,0,0,0,0,12,12,0,0,0,
  0,0,0,12,12,12,0,0,0,0,12,12,12,0,0,0,
  0,0,0,0,12,12,12,0,0,12,12,12,0,0,0,0,
  0,0,0,0,0,12,12,12,12,12,12,0,0,0,0,0,
  0,0,0,0,0,0,12,12,12,12,0,0,0,0,0,0,
  0,0,0,0,0,0,12,12,12,12,0,0,0,0,0,0,
  0,0,0,0,0,12,12,12,12,12,12,0,0,0,0,0,
  0,0,0,0,12,12,12,0,0,12,12,12,0,0,0,0,
  0,0,0,12,12,12,0,0,0,0,12,12,12,0,0,0,
  0,0,0,12,12,0,0,0,0,0,0,12,12,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Laedt eine Karte, setzt die Figur und zeichnet alles neu.
// ----------------------------------------------------------------------------------------
// mapId     = Karte
// positionX = Start Position X der Figur
// positionY = Start Position Y der Figur
void loadMap(byte mapId, int positionX, int positionY) {

  mCurrentMap = mapId;
  mPosX = positionX;
  mPosY = positionY;
  mBumpLatch = MOVE_FREE;

  mNpcActive = false;                                                  // Figuren auf der Karte suchen
  for(byte index = 0; index < MAP_TILE_COUNT; index++) {
    if(getTile(index) == TILE_NPC) {
      mNpcActive = true;
      mNpcX = (index % MAP_TILE_COUNT_X) * MAP_TILE_SIZE + (MAP_TILE_SIZE - FIGURE_WIDTH) / 2;
      mNpcY = (index / MAP_TILE_COUNT_X) * MAP_TILE_SIZE;
    }
  }

  int footX = mPosX + FIGURE_WIDTH / 2;                                // Start Kachel nicht ausloesen
  int footY = mPosY + FIGURE_HEIGHT - 3;
  mTriggerLatch = (footY / MAP_TILE_SIZE) * MAP_TILE_COUNT_X + (footX / MAP_TILE_SIZE);

  renderArea(0, 0, MAP_WIDTH, MAP_HEIGHT);                              // gesammte Karte zeichnen
}

// ========================================================================================
// Wechselt ueber einen Ausgang auf die naechste Karte.
// ----------------------------------------------------------------------------------------
// index = Kachel Index des Ausgangs
void useMapExit(byte index) {

  for(byte i = 0; i < MAP_EXIT_COUNT; i++) {
    if(pgm_read_byte(&mMapExits[i][0]) == mCurrentMap &&
       pgm_read_byte(&mMapExits[i][1]) == index) {

      loadMap(pgm_read_byte(&mMapExits[i][2]),
              pgm_read_byte(&mMapExits[i][3]),
              pgm_read_byte(&mMapExits[i][4]));
      return;
    }
  }
}

// ========================================================================================
// Liefert die Kachel an dem Index der aktuellen Karte.
// Aufgesammelte Dinge und geoeffnete Tueren werden zum Boden.
// ----------------------------------------------------------------------------------------
// index = Kachel Index (Zeile * 10 + Spalte)
byte getTile(byte index) {

  if(index >= MAP_TILE_COUNT) {
    return TILE_WALL;
  }

  byte tile = pgm_read_byte(&mMapContent[mCurrentMap][index]);

  if(isTileConsumed(index) &&
     (tile == TILE_KEY || tile == TILE_DOOR || tile == TILE_COIN)) {
    return pgm_read_byte(&mMapGround[mCurrentMap]);
  }

  return tile;
}

// ========================================================================================
// Wurde die Kachel bereits verwendet (aufgesammelt, geoeffnet, geleert).
bool isTileConsumed(byte index) {
  return (mTileConsumed[mCurrentMap][index >> 3] & (1 << (index & 7))) != 0;
}

// ========================================================================================
// Markiert die Kachel als verwendet und zeichnet sie neu.
void consumeTile(byte index) {
  mTileConsumed[mCurrentMap][index >> 3] |= (1 << (index & 7));
  renderArea((index % MAP_TILE_COUNT_X) * MAP_TILE_SIZE,
             (index / MAP_TILE_COUNT_X) * MAP_TILE_SIZE,
             MAP_TILE_SIZE, MAP_TILE_SIZE);
}

// ========================================================================================
// Liefert die Farbnummer der Karte an einer Bildschirm Position.
// ----------------------------------------------------------------------------------------
// x, y = Bildschirm Position innerhalb der Karte
byte getTilePixel(int x, int y) {

  byte tile = getTile((y / MAP_TILE_SIZE) * MAP_TILE_COUNT_X + (x / MAP_TILE_SIZE));
  byte px = x & 15;                                                    // Position innerhalb der Kachel
  byte py = y & 15;
  byte color = getTileTexturePixel(tile, px, py);

  if(color == 0) {                                                     // durchsichtig: Boden der Karte
    color = getTileTexturePixel(pgm_read_byte(&mMapGround[mCurrentMap]), px, py);
  }

  return color;
}

// ========================================================================================
// Liefert die Farbnummer einer Kachel Textur.
// ----------------------------------------------------------------------------------------
// tile   = Kachel Typ
// px, py = Position innerhalb der Kachel (0 bis 15)
byte getTileTexturePixel(byte tile, byte px, byte py) {

  byte index8 = (py & 7) * 8 + (px & 7);                               // Index fuer 8x8 Muster
  byte index16 = py * 16 + px;                                         // Index fuer 16x16 Kacheln

  switch(tile) {
    case(TILE_WALL):       { return pgm_read_byte(mTileWall + index8); }
    case(TILE_GRASS):      { return pgm_read_byte(mTileGrass + index8); }
    case(TILE_PATH):       { return pgm_read_byte(mTilePath + index8); }
    case(TILE_HEDGE):      { return pgm_read_byte(mTileHedge + index8); }
    case(TILE_ROOF):       { return pgm_read_byte(mTileRoof + index8); }
    case(TILE_FLOWERS):    { return pgm_read_byte(mTileFlowers + index8); }
    case(TILE_KEY):        { return pgm_read_byte(mItemKey01Icon + index16); }
    case(TILE_DOOR):       { return pgm_read_byte(mTileDoor + index16); }
    case(TILE_CHEST):      { return pgm_read_byte(mTileChest + index16); }
    case(TILE_EXIT):       { return pgm_read_byte(mTileExit + index16); }
    case(TILE_TREE):       { return pgm_read_byte(mTileTree + index16); }
    case(TILE_HOUSE_WALL): { return pgm_read_byte(mTileHouseWall + index16); }
    case(TILE_HOUSE_DOOR): { return pgm_read_byte(mTileHouseDoor + index16); }
    case(TILE_PHOTO_SPOT): { return pgm_read_byte(mTilePhotoSpot + index16); }
    case(TILE_COIN): {                                                 // Muenze 7x7 in der Mitte
      if(px >= 4 && px < 11 && py >= 4 && py < 11) {
        return pgm_read_byte(mCoinSpiteIcon + (py - 4) * 7 + (px - 4));
      }
      return 0;
    }
    case(TILE_FLOOR):
    case(TILE_NPC): { return pgm_read_byte(mTileFloor + index8); }     // Boden, Surie wird extra gezeichnet
    default: { return 0; }
  }
}
