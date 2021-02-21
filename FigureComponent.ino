// wird verwendet, um zu bestimmen, 
// welchers Sprite Bild beim Rendern verwendet werden soll.
int animStep = 0;

// Animationssequenz. Ist um ein paar Bytes kleiner, 
// als wenn man die function in case 1 nochmal in default einträgt.
byte animSequenz[4] = { 0, 1, 2, 1 };

// Zeichnet die Sprite Figur mit Bewegungsanimation
// - directionX -> Ausrichtung für Links und rechts
// - directionY -> Ausrichtung für hoch und runter
// - relationX  -> Horizontale Start Render Position
// - relationY  -> Vertikale Start Render Position
void drawFigure(int directionX, int directionY, int relationX, int relationY) {

  if(gameTime % 4 > 0) {
    if(animStep > 2) {animStep = 0;}
    else {animStep++;}
  }

  if(directionX == 0 && directionY == 1) {
    switch(animSequenz[animStep]){
      case(0): { memCopy(spriteFigureFrontLeft); drawTile(relationX, relationY, 10, 16, tempArray, false); break; }
      case(1): { memCopy(spriteFigureFrontMiddle); drawTile(relationX, relationY, 10, 16, tempArray, false); break; }
      case(2): { memCopy(spriteFigureFrontLeft); drawTile(relationX, relationY, 10, 16, tempArray, true); break; }
      default: {  break; }
    }
  }
  else if(directionX == -1 && directionY == 0) {
    switch(animSequenz[animStep]){
      case(0): { memCopy(spriteFigureSideLeft); drawTile(relationX, relationY, 10, 16, tempArray, false);  break; }
      case(1): { memCopy(spriteFigureSideMiddle); drawTile(relationX, relationY, 10, 16, tempArray, false); break; }
      case(2): { memCopy(spriteFigureSideRight); drawTile(relationX, relationY, 10, 16, tempArray, false); break; }
      default: { break; }
    }
  }
  else if(directionX == 0 && directionY == -1) {
    switch(animSequenz[animStep]){
      case(0): { memCopy(spriteFigureBackLeft); drawTile(relationX, relationY, 10, 16, tempArray, false);  break; }
      case(1): { memCopy(spriteFigureBackMiddle); drawTile(relationX, relationY, 10, 16, tempArray, false);  break; }
      case(2): { memCopy(spriteFigureBackLeft); drawTile(relationX, relationY, 10, 16, tempArray, true);  break; }
      default: {  break; }
    }
  }
  else if(directionX == 1 && directionY == 0) {
    switch(animSequenz[animStep]){
      case(0): { memCopy(spriteFigureSideLeft); drawTile(relationX, relationY, 10, 16, tempArray, true); break; }
      case(1): { memCopy(spriteFigureSideMiddle); drawTile(relationX, relationY, 10, 16, tempArray, true); break; }
      case(2): { memCopy(spriteFigureSideRight); drawTile(relationX, relationY, 10, 16, tempArray, true);  break; }
      default: { break; }
    }
  }
}

