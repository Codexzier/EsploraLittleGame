// ========================================================================================
// Description:       Beinhaltet die Methoden fur die Kollisionserkennung
//                    Wird auch verwendet, um Objekte einsammeln zu koennen.
//                    Interaktionen mit einem Haendler/in oder Kiste werden 
//                    hier ebenfalls getriggert.
// ========================================================================================

// ========================================================================================
// Variablen fuer Kollisionserkennung

int8_t mCollisionTilesA[3] = { -1, 0, 1 };                   // index felder die zu prüfen sind, wird in Abhaengigkeit der Richtung verwendet
int8_t mCollisionTilesB[3] = { -1, 0, 1 };                   // index felder die zu prüfen sind, wird in Abhaengigkeit der Richtung verwendet

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// kann die Figur diesen Bereich betreten
// ----------------------------------------------------------------------------------------
// positionX, positionY = Nächste Position, in dem sich die Figur bwegen soll.
boolean CanEnterArea(int positionX, int positionY) {

  // TODO: Beinhaltet auch die Abfragen zu Haendler/in, Kiste und Kartenwechsel.
  boolean resultColide = true;

  if(mDirectionX == 0) {                                     // umliegende Kacheln auf hindernis prüfen, wenn hoch oder runter

    for(uint8_t i = 0; i < 3; i++) {
      
      int tileX = (positionX + (mCollisionTilesA[i] * mMapTileSize)) / mMapTileSize;
      int tileY = -1;

      int tileYTemp = tileY;
      int positionYShift = 0;

      while(tileY == tileYTemp && tileY != 0) {
        if(mDirectionY == -1) { tileY = (positionY + mDirectionY + positionYShift) / mMapTileSize; }
        else { tileY = (positionY + mDirectionY + 16 + positionYShift) / mMapTileSize; }

        positionYShift += mMapTileSize * mDirectionY;
      }

      resultColide = checkCollideNeighbor(positionX, positionY + mDirectionY, tileX, tileY);

      if(!resultColide) {
        break;
      }
    }

    resultColide = checkCollideOther(resultColide, positionX, positionY + mDirectionY);
  }

  if(mDirectionY == 0) {                                     // wenn links oder rechts

    for(uint8_t i = 0; i < 3; i++) {
      int tileX = positionX / mMapTileSize;
      int tileY = (positionY + (mCollisionTilesA[i] * mMapTileSize)) / mMapTileSize;
      
      int tileXTemp = tileX;
      int positionXShift = 0;

      while(tileX == tileXTemp) {
        if(mDirectionX == -1) {  tileX = (positionX + mDirectionX + positionXShift) / mMapTileSize; }
        else { tileX = (positionX + mDirectionX + 10 + positionXShift) / mMapTileSize; }
        
        positionXShift += mMapTileSize * mDirectionX;
      }
  
      resultColide = checkCollideNeighbor(positionX + mDirectionX, positionY, tileX, tileY);
  
      if(!resultColide) {
        break;
      }
    }

    resultColide = checkCollideOther(resultColide, positionX + mDirectionX, positionY);
  }

  return resultColide;
}

// ========================================================================================
// Pruft ob an der Position die Figuar mit einer 
// stehenden Figur auf der Karte ueberschneidet
// ----------------------------------------------------------------------------------------
// resultColide = besteht eine ueberschneidung, 
//                dann wird diese mit der pruefung genauer geprueft.
// positionX    = aktuelle zu prufende Position X
// positionY    = aktuelle zu prufende Position Y
boolean checkCollideOther(boolean resultColide, int positionX, int positionY) {

  if(resultColide) {                                        // anderes bewegbares objekt

    int overlap = 4;
    resultColide = checkCollide(positionX, positionY, 
                                mMapFigurePositionX + (overlap / 2), 
                                mMapFigurePositionY + (overlap), 
                                10 - overlap, 16 - (overlap * 2));
    
    mShowWindow = !resultColide;                             // zum testen Fenster oeffnen
    mMenueNavigation = mShowWindow;
  }

  return resultColide;
}

// ========================================================================================
// Laedt aus dem Flashspeicher die Kachelelemente ab und 
// prueft die Kollision mit neben anliegende Kacheln.
// Verhindert speziel den Fehhler zwischen zwei Kacheln, nur eine zu pruefen.
// ----------------------------------------------------------------------------------------
// positionX  = zukuenftige position x
// positionY  = zukuenftige position y
// tileX      = x Kachel die von der zukuenftigen Position
// tileY      = y Kachel die von der zukuenftigen Position
boolean checkCollideNeighbor(int positionX, int positionY, int tileX, int tileY) {

  boolean resultColide = true;

  int mapOffsetX = tileX * mMapTileSize;
  int mapOffsetY = tileY * mMapTileSize;
  
  int indexStart = (tileY * mMapTileCountX) + tileX;  
  byte bTile = pgm_read_byte_near(mMapContent + indexStart); // hole die content Nummer ab um die kollisionsart zu bestimmen

  if(bTile == 1) {
    
    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mMapTileSize, mMapTileSize);
  }

// TODO: Werte werden hier noch uebersetzt.
  if(bTile == 2) {

    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mMapTileSize, mMapTileSize);
    
    if(!resultColide) {                                     // abruf des Objektes, 
                                                            // dass zu der Karte gehoert an der Position.
      // TODO: Abfrage ob objekt aufgenommen werden soll
      if(setItemToBackpack(1)) {
        mMapKeyIsGet = true;
      }

      resultColide = true;                                  // nicht blockieren
    }
  }
  if(bTile == 3 && resultColide) {

    // TODO: Abfrage ob objekt aufgenommen werden soll
    setItemToBackpack(2);                                   // abruf des Objektes, 
                                                            // dass zu der Karte gehoert an der Position.
  }
  
  if(bTile == 4 && resultColide) {

    // TODO: Abfrage ob objekt aufgenommen werden soll
    setItemToBackpack(3);                                   // abruf des Objektes, 
                                                            // dass zu der Karte gehoert an der Position.
  }
  
  if(bTile == 5) {                                          // Tuer pruefen und oeffnen

    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mMapTileSize, mMapTileSize);
    
    if(!resultColide) {                                     // Uebergabewert des Verwendungswecks > Tuer oeffnen.
                                                            // kollision aufheben
     
      resultColide = getItemToUsed(1);                      // ID 1 ist der Schlüssel und entscheidet,
                                                            // ob die Tuer sich oeffen laest.
    }
  }

  return resultColide;
}

// ========================================================================================
// Kachel Position mit zukuenftiger Position der Figur abgeleichen,
// durch ansetzten von Rechtecken und ob diese sich ueberschneiden.
// ----------------------------------------------------------------------------------------
// positionX      = zukuenftige position x
// positionY      = zukuenftige position y
// mapOffsetX     = Kachel Position X
// mapOffsetY     = Kachel Position Y
// tileSizeWidth  = Kachel Breite
// tileSizeHeight = Kaachel Hoehe
boolean checkCollide(byte positionX, byte positionY, byte mapOffsetX, byte mapOffsetY, byte tileSizeWidth, byte tileSizeHeight) {

  if(positionX < mapOffsetX + tileSizeWidth &&
       positionX + 10 > mapOffsetX &&
       positionY < mapOffsetY + tileSizeHeight &&
       positionY + 16 > mapOffsetY)
  {
    return false; 
  }
  
  return true;
}