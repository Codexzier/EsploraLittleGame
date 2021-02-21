// Rendert den Inhalt der Karte
// Die Figur kann sich auf diesen festgelegten Karte frei bewegen.

// liest die Karteninformation ein und Zeichnet die Kacheln.
// - positionX > x Position der Figur
// - positionY > y Position der Figur
// - renderAll > wenn alle Kartenabschnitt neu gerendert werden sollen.
void renderMap(int positionX, int positionY, boolean renderAll) {

  // zum probieren wird zunächst ein Grid gerendert.
  byte index = 0;
  for(byte y = 0; y < mapTileCountY; y++) {
    for(byte x = 0; x < mapTileCountX; x++) {

      if(((positionX >= (int)(x * mapTileSize) - (int)mapTileSize && positionX <= (int)(x + 1) * (int)mapTileSize && 
          positionY >= (int)(y * mapTileSize) - (int)mapTileSize && positionY <= (int)(y + 1) * (int)mapTileSize)) || 
          renderAll) {
            byte bTile = pgm_read_byte_near(mapContent + index);

            // TODO: Kartenspezifische abhangigkeit, 
            //       Eigenschaften andern sich mit Kartenwechsel
            if(bTile == 2 && mapKeyIsGet) { bTile = 0; }
            if(bTile == 5 && mapBarrierDoorIsOpen) { bTile = 0; }
           
           renderMapTile(x, y, bTile);
      }
      index++;
    }
  }
}

// rendert die Kacheln Einfarbig.
// - x          > relation x Position
// - y          > relation x Position
// - mapSegment > Karten Eigenschaft
void renderMapTile(byte x, byte y, byte mapSegment) {
  
  byte mapTileColorNumber = 0;
  switch(mapSegment) {
    case(1): { mapTileColorNumber = 10; break; }
    case(2): { mapTileColorNumber = 12; break; }
    case(5): { mapTileColorNumber = 13; break; }
    default: { mapTileColorNumber = 15; break; }
  }

  EsploraTFT.fillRect(x * mapTileSize, y * mapTileSize, mapTileSize, mapTileSize, mapNumberToColor(mapTileColorNumber));
}