// ========================================================================================
// Description:       Beinhaltet die Methoden fur die Kollisionserkennung
//                    Wird auch verwendet, um Objekte einsammeln zu koennen.
//                    Interaktionen mit einem Haendler/in oder Kiste werden
//                    hier ebenfalls getriggert.
// ----------------------------------------------------------------------------------------
// Fuss Bereich:      Geprueft wird nur der untere Teil der Figur (die Fuesse).
//                    Dadurch kann der Kopf vor einer Wand stehen, wie bei
//                    Spielen mit Draufsicht ueblich.
// ========================================================================================

// ========================================================================================
// Variablen fuer Kollisionserkennung

#define FEET_OFFSET_X   1                                              // Fuss Bereich relativ zur Figur
#define FEET_OFFSET_Y   10
#define FEET_WIDTH      8
#define FEET_HEIGHT     6
#define NPC_BLOCK_OFFSET_Y 6                                           // Beginn des Bereiches, den Surie blockiert

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// kann die Figur diesen Bereich betreten
// ----------------------------------------------------------------------------------------
// positionX, positionY = Nächste Position, in dem sich die Figur bwegen soll.
// Rueckgabe            = MOVE_FREE, MOVE_BLOCKED_NPC, MOVE_BLOCKED_BORDER
//                        oder der Index der Kachel, die im Weg ist.
byte checkMove(int positionX, int positionY) {

  if(positionX < 0 || positionY < 0 ||                                 // Kartenrand
     positionX > MAP_WIDTH - FIGURE_WIDTH ||
     positionY > MAP_HEIGHT - FIGURE_HEIGHT) {
    return MOVE_BLOCKED_BORDER;
  }

  int feetLeft = positionX + FEET_OFFSET_X;
  int feetTop = positionY + FEET_OFFSET_Y;

  byte blocker = MOVE_FREE;
  for(int tileY = feetTop / MAP_TILE_SIZE;                              // alle Kacheln unter den Fuessen pruefen
      tileY <= (feetTop + FEET_HEIGHT - 1) / MAP_TILE_SIZE; tileY++) {
    for(int tileX = feetLeft / MAP_TILE_SIZE;
        tileX <= (feetLeft + FEET_WIDTH - 1) / MAP_TILE_SIZE; tileX++) {

      byte index = tileY * MAP_TILE_COUNT_X + tileX;
      byte tile = getTile(index);

      if(isTileSolid(tile)) {
        if(isTileInteractive(tile)) {                                   // Tuer oder Kiste haben Vorrang vor einer Wand
          return index;
        }
        blocker = index;
      }
    }
  }

  if(blocker != MOVE_FREE) {
    return blocker;
  }

  if(mNpcActive &&                                                      // anderes bewegbares objekt, der Bereich reicht
     checkCollide(feetLeft, feetTop, FEET_WIDTH, FEET_HEIGHT,          // etwas unter Surie, damit sich die Figuren
                  mNpcX, mNpcY + NPC_BLOCK_OFFSET_Y,                    // kaum ueberdecken
                  FIGURE_WIDTH, FIGURE_HEIGHT)) {
    return MOVE_BLOCKED_NPC;
  }

  return MOVE_FREE;
}

// ========================================================================================
// Kann die Kachel nicht betreten werden.
bool isTileSolid(byte tile) {

  switch(tile) {
    case(TILE_WALL):
    case(TILE_DOOR):
    case(TILE_CHEST):
    case(TILE_TREE):
    case(TILE_HOUSE_WALL):
    case(TILE_ROOF):
    case(TILE_HEDGE):
    case(TILE_HOUSE_DOOR):
    case(TILE_WATER):
    case(TILE_PALM):
    case(TILE_BOAT):
    case(TILE_BUS_SIGN):
    case(TILE_FACADE):
    case(TILE_FAR_ISLAND):
    case(TILE_TABLE):
    case(TILE_PICTURE_GARDEN):
    case(TILE_PICTURE_BRIDGE):
    case(TILE_PICTURE_ISLAND):
    case(TILE_PICTURE_CITY): { return true; }
    default: { return false; }
  }
}

// ========================================================================================
// Loest die Kachel beim Anstossen eine Aktion aus.
bool isTileInteractive(byte tile) {

  switch(tile) {
    case(TILE_DOOR):
    case(TILE_CHEST):
    case(TILE_HOUSE_DOOR):
    case(TILE_BOAT):
    case(TILE_BUS_SIGN):
    case(TILE_TABLE):
    case(TILE_PICTURE_GARDEN):
    case(TILE_PICTURE_BRIDGE):
    case(TILE_PICTURE_ISLAND):
    case(TILE_PICTURE_CITY): { return true; }
    default: { return false; }
  }
}

// ========================================================================================
// Prueft, ob die Fuesse der Figur eine Kachel betreten haben, die etwas ausloest
// (Schluessel, Muenze, Ausgang oder Fotopunkt).
// Als Position zaehlt die Mitte der Fuesse.
void checkTrigger() {

  int footX = mPosX + FEET_OFFSET_X + FEET_WIDTH / 2;
  int footY = mPosY + FEET_OFFSET_Y + FEET_HEIGHT / 2;
  byte index = (footY / MAP_TILE_SIZE) * MAP_TILE_COUNT_X + (footX / MAP_TILE_SIZE);

  if(index == mTriggerLatch) {                                          // nur beim Betreten ausloesen
    return;
  }

  mTriggerLatch = index;
  onEnterTile(index, getTile(index));
}

// ========================================================================================
// Ueberschneiden sich zwei Rechtecke.
// ----------------------------------------------------------------------------------------
// ax, ay, aw, ah = Rechteck A (Position, Breite, Hoehe)
// bx, by, bw, bh = Rechteck B (Position, Breite, Hoehe)
bool checkCollide(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {

  return ax < bx + bw && ax + aw > bx &&
         ay < by + bh && ay + ah > by;
}
