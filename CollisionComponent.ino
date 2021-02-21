// Beinhaltet die Methoden fur die Kollisionserkennung
// Wird auch verwendet, um Objekte einsammeln zu koennen.
// Interaktionen mit einem Haendler/in oder Kiste werden hier ebenfalls getriggert.

// ########################################################
// #         Variablen fuer Kollisionserkennung           #
// ########################################################
// index felder die zu prüfen sind
// wird in Abhaengigkeit der Richtung verwendet
int8_t collisionTilesA[3] = { -1, 0, 1 };
int8_t collisionTilesB[3] = { -1, 0, 1 };

// #######################################
// #          Methoden                   #
// #######################################

// Prüfen, ob in diesem Bereich sich bewegt werden kann.
// - positionX -> zukuenftige position x
// - positionY -> zukuenftige position y
boolean CanEnterArea(int positionX, int positionY) {
  boolean resultColide = true;
  
  // umliegende Kacheln auf hindernis prüfen
  // wenn hoch oder runter
  if(directionX == 0) {

    for(uint8_t i = 0; i < 3; i++) {
      
      int tileX = (positionX + (collisionTilesA[i] * mapTileSize)) / mapTileSize;
      int tileY = -1;

      int tileYTemp = tileY;
      int positionYShift = 0;

      while(tileY == tileYTemp && tileY != 0) {
        if(directionY == -1) { tileY = (positionY + directionY + positionYShift) / mapTileSize; }
        else { tileY = (positionY + directionY + 16 + positionYShift) / mapTileSize; }

        positionYShift += mapTileSize * directionY;
      }

      resultColide = checkCollideNeighbor(positionX, positionY + directionY, tileX, tileY);

      if(!resultColide) {
        break;
      }
    }

    resultColide = checkCollideOther(resultColide, positionX, positionY + directionY);
  }

  // wenn links oder rechts
  if(directionY == 0) {

    for(uint8_t i = 0; i < 3; i++) {
      int tileX = positionX / mapTileSize;
      int tileY = (positionY + (collisionTilesA[i] * mapTileSize)) / mapTileSize;
      
      int tileXTemp = tileX;
      int positionXShift = 0;

      while(tileX == tileXTemp) {
        if(directionX == -1) {  tileX = (positionX + directionX + positionXShift) / mapTileSize; }
        else { tileX = (positionX + directionX + 10 + positionXShift) / mapTileSize; }
        
        positionXShift += mapTileSize * directionX;
      }
  
      resultColide = checkCollideNeighbor(positionX + directionX, positionY, tileX, tileY);
  
      if(!resultColide) {
        break;
      }
    }

    resultColide = checkCollideOther(resultColide, positionX + directionX, positionY);
  }

  return resultColide;
}

boolean checkCollideOther(boolean resultColide, int positionX, int positionY) {
  // anderes bewegbares objekt
  if(resultColide) {

    int overlap = 4;
    resultColide = checkCollide(positionX, positionY, mapFigurePositionX + (overlap / 2), mapFigurePositionY + (overlap), 10 - overlap, 16 - (overlap * 2));

    // zum testen Fenster oeffnen
    showWindow = !resultColide;
    menueNavigation = showWindow;
  }

  return resultColide;
}

// Laedt aus dem Flashspeicher die Kachelelemente ab und 
// prueft die Kollision mit neben anliegende Kacheln.
// Verhindert speziel den Fehhler zwischen zwei Kacheln, nur eine zu pruefen.
// - positionX > zukuenftige position x
// - positionY > zukuenftige position y
// - tileX     > x Kachel die von der zukuenftigen Position
// - tileY     > y Kachel die von der zukuenftigen Position
boolean checkCollideNeighbor(int positionX, int positionY, int tileX, int tileY) {

  boolean resultColide = true;

  int mapOffsetX = tileX * mapTileSize;
  int mapOffsetY = tileY * mapTileSize;

  int indexStart = (tileY * mapTileCountX) + tileX;
  // hole die content Nummer ab um die kollisionsart zu bestimmen
  byte bTile = pgm_read_byte_near(mapContent + indexStart);

  if(bTile == 1) {
    
    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mapTileSize, mapTileSize);
  }

  if(bTile == 2) {

    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mapTileSize, mapTileSize);

    // abruf des Objektes, 
    // dass zu der Karte gehoert an der Position.
    if(!resultColide) {
      if(setItemToBackpack(1)) {
        mapKeyIsGet = true;
      }
      // nicht blockieren
      resultColide = true;
    }
  }
  if(bTile == 3 && resultColide) {

    // abruf des Objektes, 
    // dass zu der Karte gehoert an der Position.
    // TODO: Abfrage ob objekt aufgenommen werden soll
    setItemToBackpack(2);
  }
  if(bTile == 4 && resultColide) {

    // abruf des Objektes, 
    // dass zu der Karte gehoert an der Position.
    // TODO: Abfrage ob objekt aufgenommen werden soll
    setItemToBackpack(3);
  }

  // Tuer pruefen und oeffnen
  if(bTile == 5) {

    resultColide = checkCollide(positionX, positionY, mapOffsetX, mapOffsetY, mapTileSize, mapTileSize);

    // Uebergabewert des Verwendungswecks > Tuer oeffnen.
    // kollision aufheben
    if(!resultColide) {

      // ID 1 ist der Schlüssel und entscheidet,
      // ob die Tuer sich oeffen laest.
      resultColide = getItemToUsed(1);
    }
  }

  return resultColide;
}

// Kachel Position mit zukuenftiger Position der Figur abgeleichen,
// durch ansetzten von Rechtecken und ob diese sich ueberschneiden.
// - positionX  > zukuenftige position x
// - positionY  > zukuenftige position y
// - mapOffsetX > Kachel Position X
// - mapOffsetY > Kachel Position Y
boolean checkCollide(byte positionX, byte positionY, byte mapOffsetX, byte mapOffsetY, byte tileSizeX, byte tileSizeY) {

return !(positionX < mapOffsetX + tileSizeX &&
       positionX + 10 > mapOffsetX &&
       positionY < mapOffsetY + tileSizeY &&
       positionY + 16 > mapOffsetY);
}