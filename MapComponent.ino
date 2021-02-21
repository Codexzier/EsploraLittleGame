// Rendert den Inhalt der Karte
// Die Figur kann sich auf diesen festgelegten Karte frei bewegen.

// Karte
// 0 = begehbare flaeche
// 1 = Wand
byte mapContent[160] =  { 
  1,1,1,1,1,1,1,1,1,1,
  1,0,1,0,0,0,0,0,0,1,
  1,0,1,0,0,1,0,0,0,1,
  1,0,1,0,0,1,1,1,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,
};

// Groeße einer Kachel
byte mapTileSize = 16;
// Anzahl Kacheln auf der X Achse
byte mapCountX = 10;
// Anzahl Kacheln auf der Y Achse
byte mapCountY = 6;

// liest die Karteninformation ein und Zeichnet die Kacheln.
// positionX -> Horizontale Render Start Position
// positionY -> Vertikale Render Start Position
// renderAll -> sollen alle Kacheln neu geschrieben werden 
//              oder nur von der angegebenen Position mit den Änderungen.
void renderMap(int positionX, int positionY, boolean renderAll) {
  byte index = 0;
  for(byte y = 0; y < mapCountY; y++) {
    for(byte x = 0; x < mapCountX; x++) {

      if(((positionX >= (int)(x * mapTileSize) - (int)mapTileSize && positionX <= (int)(x + 1) * (int)mapTileSize && 
          positionY >= (int)(y * mapTileSize) - (int)mapTileSize && positionY <= (int)(y + 1) * (int)mapTileSize)) || 
          renderAll) {
        renderMapTile(x, y, mapContent[index]);
      }
      index++;
    }
  }
}

// Einfache Kollisionsabfrage
// positionX -> Horizontale Position
// positionY -> Vertikale Position
boolean canEnterArea(byte positionX, byte positionY) {

  // Kachel Kordinate abrufen
  byte tileX = positionX / 16;
  byte tileY = positionY / 16;

  // Index aus dem Array abfragen
  byte index = (tileY * mapCountX) + tileX;

  // ist das Feld begehbar
  if(mapContent[index] == 0) {
    return true;
  }
  
  return false;
}

// rendert die Kacheln Einfarbig.
// x = Horizontale Kachel Koordinate
// y = Vertikale Kachel Koordinate
void renderMapTile(byte x, byte y, byte mapSegment) {
  
  byte mapTileColorNumber = 0;
  
  // mapping farbe zuweisen
  switch(mapSegment) {
    case(1): { mapTileColorNumber = 10; break; }
    default: { mapTileColorNumber = 15; break; }
  }

  EsploraTFT.fillRect(x * mapTileSize, y * mapTileSize, mapTileSize, mapTileSize, mapNumberToColor(mapTileColorNumber));
}