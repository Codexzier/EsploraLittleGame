// wird verwendet, um zu bestimmen, 
// welchers Sprite Bild beim Rendern verwendet werden soll.
int animStep = 0;

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
    switch(animStep){
      case(0): { drawTile(relationX, relationY, 10, 16, spriteFigureFrontLeft, false); break; }
      case(1): { drawTile(relationX, relationY, 10, 16, spriteFigureFrontMiddle, false); break; }
      case(2): { drawTile(relationX, relationY, 10, 16, spriteFigureFrontLeft, true); break; }
      default: {  drawTile(relationX, relationY, 10, 16, spriteFigureFrontMiddle, false); break; }
    }
  }
  else if(directionX == -1 && directionY == 0) {
    switch(animStep){
      case(0): { drawTile(relationX, relationY, 10, 16, spriteFigureSideLeft, false);  break; }
      case(1): { drawTile(relationX, relationY, 10, 16, spriteFigureSideMiddle, false); break; }
      case(2): { drawTile(relationX, relationY, 10, 16, spriteFigureSideRight, false); break; }
      default: { drawTile(relationX, relationY, 10, 16, spriteFigureSideMiddle, false); break; }
    }
  }
  else if(directionX == 0 && directionY == -1) {
    switch(animStep){
      case(0): { drawTile(relationX, relationY, 10, 16, spriteFigureBackLeft, false);  break; }
      case(1): { drawTile(relationX, relationY, 10, 16, spriteFigureBackMiddle, false);  break; }
      case(2): { drawTile(relationX, relationY, 10, 16, spriteFigureBackLeft, true);  break; }
      default: {  drawTile(relationX, relationY, 10, 16, spriteFigureBackMiddle, false);  break; }
    }
  }
  else if(directionX == 1 && directionY == 0) {
    switch(animStep){
      case(0): { drawTile(relationX, relationY, 10, 16, spriteFigureSideLeft, true); break; }
      case(1): { drawTile(relationX, relationY, 10, 16, spriteFigureSideMiddle, true); break; }
      case(2): { drawTile(relationX, relationY, 10, 16, spriteFigureSideRight, true);  break; }
      default: { drawTile(relationX, relationY, 10, 16, spriteFigureSideMiddle, true); break; }
    }
  }
}