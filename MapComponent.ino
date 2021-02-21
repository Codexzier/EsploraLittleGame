// Rendert den Inhalt der Karte
// Die Figur kann sich auf diesen festgelegten Karte frei bewegen.

// Karten größe muss noch geprüft werden.
const PROGMEM byte mapContent[160] =  { 
  1,1,1,1,1,1,1,1,1,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,1,0,0,1,0,0,0,1,
  1,0,1,0,0,1,1,1,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,
};
// map size
// Groeße einer Kachel
byte mapTileSize = 16;
// Anzahl Kacheln auf der X Achse
byte mapCountX = 10;
// Anzahl Kacheln auf der Y Achse
byte mapCountY = 6;

// liest die Karteninformation ein und Zeichnet die Kacheln.
void renderMap(int positionX, int positionY, boolean renderAll) {

  // zum probieren wird zunächste ein Grid gerendert.
  byte index = 0;
  for(byte y = 0; y < mapCountY; y++) {
    for(byte x = 0; x < mapCountX; x++) {

      if(((positionX >= (int)(x * mapTileSize) - (int)mapTileSize && positionX <= (int)(x + 1) * (int)mapTileSize && 
          positionY >= (int)(y * mapTileSize) - (int)mapTileSize && positionY <= (int)(y + 1) * (int)mapTileSize)) || 
          renderAll) {
        renderMapTile(x, y, pgm_read_byte_near(mapContent + index));
      }
      index++;
    }
  }
}

// Einfache Kollisionsabfrage
boolean CanEnterArea(byte positionX, byte positionY) {

  // Kachel Kordinate abrufen
  byte tileX = positionX / 16;
  byte tileY = positionY / 16;

  // Index aus dem Array abfragen
  byte index = (tileY * mapCountX) + tileX;

  // ist das Feld begehbar
  if(pgm_read_byte_near(mapContent + index) == 0) {
    return true;
  }
  
  return false;
}

// rendert die Kacheln Einfarbig.
void renderMapTile(byte x, byte y, byte mapSegment) {
  
  byte mapTileColorNumber = 0;
  switch(mapSegment) {
    case(1): { mapTileColorNumber = 10; break; }
    default: { mapTileColorNumber = 15; break; }
  }

  EsploraTFT.fillRect(x * mapTileSize, y * mapTileSize, mapTileSize, mapTileSize, mapNumberToColor(mapTileColorNumber));
}