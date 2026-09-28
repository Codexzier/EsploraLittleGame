// ========================================================================================
// Description:       Rendert den Inhalt der Karte
//                    Die Figur kann sich auf diesen festgelegten Karte frei bewegen.
// ----------------------------------------------------------------------------------------
// Jede Kachel hat eine Textur (AssetsData.ino). 8x8 Muster werden auf die 16x16 Kachel
// wiederholt. Durchsichtige Pixel (0) zeigen den Boden der Karte (Haus: Fliesen,
// Garten: Gras, Hafen: Sand ...), beim Boot und der fernen Insel das Wasser.
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
    16, 9, 9, 9, 9,12,18,12,14,16,                                     // 12 = Hauswand, 18 = Haustuer (Suries Haus)
     8, 9, 9, 9,15,10,10, 9, 9,35,                                     // 8 = Ausgang zum Haus, 10 = Weg, 15 = Fotopunkt
    16,14,17, 9, 9, 9,10,17,14,16,                                     // 17 = Blumen, 35 = Hecke, spaeter Ausgang zum Fluss
    16,16,16,16,16,16,16,16,16,16,
  },
  {                                                                    // Karte 2: Fluss mit Bruecke
    16,16,16,16,19,19,16,16,16,16,                                     // 19 = Wasser
    16, 9,17, 9,19,19, 9,11, 9,16,
    16, 9, 9,15,19,19, 9, 9, 9,16,                                     // 15 = Fotopunkt mit Blick auf die Bruecke
     8,10,10,10,20,20,10,10,10, 8,                                     // 20 = Bruecke
    16, 9,11, 9,19,19, 9,17, 9,16,
    16,16,16,16,19,19,16,16,16,16,
  },
  {                                                                    // Karte 3: Hafen
    16,16,16,16,16,16,16,16,16,16,
    16,21,21,22,21,21,21,21,21,16,                                     // 21 = Sand, 22 = Palme
     8,21,21,21,21,21,33,21,21, 8,                                     // 33 = Kapitaen
    19,19,19,19,20,19,19,19,19,19,                                     // 20 = Steg
    19,19,19,19,20,23,19,19,19,19,                                     // 23 = Boot
    19,19,19,19,19,19,19,19,19,19,
  },
  {                                                                    // Karte 4: Nachbarinsel
    19,19,19,19,19,19,19,19,19,19,
    19,21,22,21,21,19,19,19,19,19,
    19,21,21,21,15,19,19,19,27,19,                                     // 15 = Fotopunkt, 27 = Insel in der Ferne
    19,21,33,21,21,20,23,19,19,19,                                     // 33 = Kapitaen, 20 = Steg, 23 = Boot
    19,19,21,21,21,19,19,19,19,19,
    19,19,19,19,19,19,19,19,19,19,
  },
  {                                                                    // Karte 5: Bushaltestelle
    16,16,16,16,16,16,16,16,16,16,
    16, 9,11, 9,17, 9, 9,11, 9,16,
     8,10,10,10,10,10,10,10,10,16,
     9, 9, 9, 9,25,34, 9, 9, 9, 9,                                     // 25 = Haltestelle, 34 = Busfahrer
    24,24,24,24,24,24,24,24,24,24,                                     // 24 = Strasse
    24,24,24,24,24,24,24,24,24,24,
  },
  {                                                                    // Karte 6: Stadtrand
    13,13,13,13,13,13,13,13,13,13,                                     // 13 = Daecher
    26,26,26,26,26,26,26,26,26,26,                                     // 26 = Hauswaende der Stadt
    10,10,10,10,10,15,10,10,10,10,                                     // 15 = Fotopunkt
    10,25,34,10,10,10,10,10,10,10,                                     // 25 = Haltestelle, 34 = Busfahrer
    24,24,24,24,24,24,24,24,24,24,
    24,24,24,24,24,24,24,24,24,24,
  },
  {                                                                    // Karte 7: Suries Stube
     1, 1,29, 1,30, 1,31, 1,32, 1,                                     // 29 bis 32 = Fotos an der Wand
     1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0,28, 0, 6, 0, 0, 0, 1,                                     // 28 = Tisch mit Kaffee, 6 = Surie
     1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     1, 1, 1, 1, 8, 1, 1, 1, 1, 1,                                     // 8 = Ausgang zum Garten
  }
};

const PROGMEM byte mMapGround[MAP_COUNT] = {                           // Boden je Karte
  TILE_FLOOR, TILE_GRASS, TILE_GRASS, TILE_SAND, TILE_SAND, TILE_GRASS, TILE_PATH, TILE_FLOOR
};

// ----------------------------------------------------------------------------------------
// Ausgaenge: Karte, Kachel Index, Ziel Karte, Ziel Position X, Ziel Position Y

#define MAP_EXIT_COUNT 9
const PROGMEM byte mMapExits[MAP_EXIT_COUNT][5] = {
  { MAP_HOUSE,       39, MAP_GARDEN,   19,  48 },                      // Haus rechts      -> Garten links
  { MAP_GARDEN,      30, MAP_HOUSE,    131, 48 },                      // Garten links     -> Haus rechts
  { MAP_GARDEN,      39, MAP_RIVER,    19,  48 },                      // Garten rechts    -> Fluss links
  { MAP_RIVER,       30, MAP_GARDEN,   131, 48 },                      // Fluss links      -> Garten rechts
  { MAP_RIVER,       39, MAP_HARBOR,   19,  32 },                      // Fluss rechts     -> Hafen links
  { MAP_HARBOR,      20, MAP_RIVER,    131, 48 },                      // Hafen links      -> Fluss rechts
  { MAP_HARBOR,      29, MAP_BUS_STOP, 19,  32 },                      // Hafen rechts     -> Haltestelle links
  { MAP_BUS_STOP,    20, MAP_HARBOR,   131, 32 },                      // Haltestelle links -> Hafen rechts
  { MAP_LIVING_ROOM, 54, MAP_GARDEN,   99,  48 },                      // Stube unten      -> vor Suries Haus
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
  mScene = SCENE_MAP;

  mNpcActive = false;                                                  // Figuren auf der Karte suchen
  mNpcType = NPC_NONE;
  for(byte index = 0; index < MAP_TILE_COUNT; index++) {
    byte tile = getTile(index);
    if(tile == TILE_NPC || tile == TILE_NPC_CAPTAIN || tile == TILE_NPC_DRIVER) {
      mNpcActive = true;
      mNpcType = tile;
      mNpcX = (index % MAP_TILE_COUNT_X) * MAP_TILE_SIZE + (MAP_TILE_SIZE - FIGURE_WIDTH) / 2;
      mNpcY = (index / MAP_TILE_COUNT_X) * MAP_TILE_SIZE;
    }
  }

  int footX = mPosX + FIGURE_WIDTH / 2;                                // Start Kachel nicht ausloesen
  int footY = mPosY + FIGURE_HEIGHT - 3;
  mTriggerLatch = (footY / MAP_TILE_SIZE) * MAP_TILE_COUNT_X + (footX / MAP_TILE_SIZE);

  renderArea(0, 0, MAP_WIDTH, MAP_HEIGHT);                              // gesammte Karte zeichnen
  saveGame();                                                          // automatisch speichern
}

// ========================================================================================
// Laedt die aktuelle Karte neu, z.B. wenn eine Figur die Karte verlassen hat.
void reloadMap() {
  loadMap(mCurrentMap, mPosX, mPosY);
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
    return getGround();
  }

  if(tile == TILE_GATE) {                                              // Hecke oeffnet sich fuer den zweiten Auftrag
    return mQuestState >= QUEST_MORE_PHOTOS ? TILE_EXIT : TILE_HEDGE;
  }

  if(tile == TILE_NPC && mCurrentMap == MAP_HOUSE &&                   // Surie ist nach Hause gegangen
     mQuestState >= QUEST_PHOTOS_GIVEN) {
    return getGround();
  }

  return tile;
}

// ========================================================================================
// Boden der aktuellen Karte.
byte getGround() {
  return pgm_read_byte(&mMapGround[mCurrentMap]);
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

  if(color == 0) {                                                     // durchsichtig: darunter liegende Kachel
    byte under = (tile == TILE_BOAT || tile == TILE_FAR_ISLAND) ? TILE_WATER : getGround();
    color = getTileTexturePixel(under, px, py);
  }

  return color;
}

// ========================================================================================
// Texturen je Kachel Typ (Index = Kachel Typ, NULL = keine Textur, also Boden)
// und die Groesse: 8 = 8x8 Muster (wird wiederholt), 16 = 16x16 Bild

const byte* const PROGMEM mTileTextures[] = {
  mTileFloor,        mTileWall,          mItemKey01Icon,       NULL,                  // 0 bis 3
  NULL,              mTileDoor,          NULL,                 mTileChest,            // 4 bis 7
  mTileExit,         mTileGrass,         mTilePath,            mTileTree,             // 8 bis 11
  mTileHouseWall,    mTileRoof,          NULL,                 mTilePhotoSpot,        // 12 bis 15 (Muenze extra)
  mTileHedge,        mTileFlowers,       mTileHouseDoor,       mTileWater,            // 16 bis 19
  mTileBridge,       mTileSand,          mTilePalm,            mTileBoat,             // 20 bis 23
  mTileRoad,         mTileBusSign,       mTileFacade,          mTileFarIsland,        // 24 bis 27
  mTileTable,        mItemPhoto01Icon,   mItemPhotoBridgeIcon, mItemPhotoIslandIcon,  // 28 bis 31
  mItemPhotoCityIcon                                                                  // 32
};

const PROGMEM byte mTileTextureSize[] = {
  8, 8, 16, 0,   0, 16, 0, 16,   16, 8, 8, 16,   16, 8, 0, 16,
  8, 8, 16, 8,   8, 8, 16, 16,   8, 16, 8, 16,   16, 16, 16, 16,   16
};

#define TILE_TEXTURE_COUNT 33

// ========================================================================================
// Liefert die Farbnummer einer Kachel Textur.
// ----------------------------------------------------------------------------------------
// tile   = Kachel Typ
// px, py = Position innerhalb der Kachel (0 bis 15)
byte getTileTexturePixel(byte tile, byte px, byte py) {

  if(tile == TILE_COIN) {                                              // Muenze 7x7 in der Mitte
    if(px >= 4 && px < 11 && py >= 4 && py < 11) {
      return getPackedPixel(mCoinSpiteIcon, (py - 4) * 7 + (px - 4));
    }
    return 0;
  }

  if(tile >= TILE_TEXTURE_COUNT) {                                     // Figuren Startplaetze: Boden
    return 0;
  }

  const byte* texture = (const byte*)pgm_read_ptr(&mTileTextures[tile]);
  if(texture == NULL) {
    return 0;
  }

  if(pgm_read_byte(&mTileTextureSize[tile]) == 8) {                    // 8x8 Muster wiederholen
    return getPackedPixel(texture, (py & 7) * 8 + (px & 7));
  }

  return getPackedPixel(texture, py * 16 + px);
}
