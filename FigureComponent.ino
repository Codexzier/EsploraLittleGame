// ========================================================================================
// Description:       Animation der zu steuernden Figur.
// ========================================================================================

// ========================================================================================
// Variablen

int mAnimStep = 0;                           // wird verwendet, um zu bestimmen, 
                                            // welchers Sprite Bild beim Rendern verwendet werden soll.

byte mAnimSequenz[4] = { 0, 1, 2, 1 };       // Animationssequenz. Ist um ein paar Bytes kleiner, 
                                            // als wenn man die function in case 1 nochmal in default einträgt.

// ========================================================================================
// Figur Sprites

byte mTempArray[160];                                                  // Zwischenspeicher für das laden des akutellen byte array das ein sprite abbildet.

const PROGMEM byte mSpriteFigureFrontLeft[160] = {
  0,0,1,1,1,1,1,1,0,0,0,1,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,1,1,3,3,4,4,2,4,3,3,1,1,3,5,5,2,2,5,5,3,1,1,3,4,1,2,2,1,4,3,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,1,0,0,1,8,8,3,3,8,8,6,1,1,8,8,8,8,8,8,6,2,1,1,2,1,8,8,8,8,1,1,0,0,1,1,10,10,7,7,1,0,0,0,1,9,10,1,7,6,1,0,0,0,0,1,1,1,7,6,1,0,0,0,0,0,0,1,9,11,1,0,0,0,0,0,0,0,1,1,0,0,0
};

const PROGMEM byte mSpriteFigureFrontMiddle[160] = {
  0,0,1,1,1,1,1,1,0,0,0,1,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,1,1,3,3,4,2,4,4,3,3,1,1,3,5,5,2,2,5,5,3,1,1,3,4,1,2,2,1,4,3,1,0,1,2,1,2,2,1,2,1,0,0,0,1,2,2,2,2,1,0,0,0,1,8,8,3,3,8,8,1,0,1,8,6,8,8,8,8,6,8,1,1,2,1,8,8,8,8,1,2,1,0,1,1,7,7,7,7,1,1,0,0,0,1,6,7,7,6,1,0,0,0,0,1,6,7,7,6,1,0,0,0,0,1,9,10,10,9,1,0,0,0,0,0,1,1,1,1,0,0,0
};
  
const PROGMEM byte mSpriteFigureSideLeft[160] = {
  0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,0,1,3,8,8,8,1,0,0,0,0,1,8,8,8,6,1,1,0,0,1,2,8,8,8,7,1,1,0,0,1,7,7,1,7,7,1,0,0,0,0,1,1,10,6,6,1,0,0,0,1,10,10,1,6,10,11,1,0,0,1,1,1,1,1,10,9,1,0,0,0,0,0,0,0,1,1,0,0
};

const PROGMEM byte mSpriteFigureSideMiddle[160] = {
  0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,0,1,1,3,8,8,1,0,0,0,0,0,1,8,8,6,1,0,0,0,0,0,1,8,8,6,1,0,0,0,0,0,1,2,7,1,1,0,0,0,0,0,1,1,10,1,1,0,0,0,0,0,1,10,10,10,1,0,0,0,0,0,1,9,9,1,1,0,0,0,0,0,1,1,1,1,1,0,0
};

const PROGMEM byte mSpriteFigureSideRight[160] = {
  0,0,1,1,1,1,1,0,0,0,0,1,3,3,3,3,3,1,0,0,1,3,3,3,3,3,3,5,1,0,1,4,4,3,3,3,3,3,3,1,0,1,5,2,3,3,3,3,3,1,0,1,2,1,2,10,3,3,3,1,0,1,2,1,2,2,10,3,1,0,0,1,2,2,2,2,2,4,1,0,0,1,1,3,8,8,8,1,0,0,1,2,1,8,8,8,6,1,1,0,1,1,1,8,8,1,7,6,1,0,0,0,1,7,1,1,7,2,1,0,0,0,1,1,10,6,1,1,1,0,0,1,11,10,10,1,9,9,1,0,0,1,9,9,9,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0
};

const PROGMEM byte mSpriteFigureBackLeft[160] = {
  0,0,1,1,1,1,1,1,0,0,0,1,3,4,4,3,3,4,1,0,1,4,3,3,3,3,3,3,4,1,1,3,3,3,3,3,3,3,3,1,1,4,3,3,3,3,10,3,4,1,1,5,4,3,3,3,3,3,10,1,0,1,10,10,3,3,3,10,1,0,0,1,1,1,1,1,1,1,0,0,1,8,8,8,8,8,8,6,1,0,1,2,1,6,8,8,6,8,8,1,0,1,1,8,8,8,8,1,2,1,0,0,1,6,7,7,7,1,1,0,0,0,1,6,6,1,6,6,1,0,0,0,1,7,7,1,1,1,0,0,0,0,1,9,9,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0
};

const PROGMEM byte mSpriteFigureBackMiddle[160] = {
  0,0,1,1,1,1,1,1,0,0,0,1,3,4,4,3,3,4,1,0,1,4,3,3,3,3,3,3,4,1,1,3,3,3,3,3,3,3,3,1,1,4,3,3,3,3,10,3,4,1,1,5,4,3,3,3,3,3,10,1,0,1,10,10,3,3,3,10,1,0,0,0,1,1,1,1,1,1,0,0,0,1,8,8,8,8,8,6,1,0,1,8,6,6,8,8,6,6,8,1,1,2,1,8,8,8,8,1,2,1,0,1,1,7,6,6,7,1,1,0,0,0,1,6,7,7,6,1,0,0,0,0,1,9,9,9,9,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0
};


// ========================================================================================
// Zeichnet die Sprite Figur mit Bewegungsanimation
// ----------------------------------------------------------------------------------------
// directionX = Ausrichtung für Links und rechts
// directionY = Ausrichtung für hoch und runter
// relationX  = Horizontale Start Render Position
// relationY  = Vertikale Start Render Position
void drawFigure(int directionX, int directionY, int relationX, int relationY) {

  if(mGameTime % 4 > 0) {
    
    if(mAnimStep > 2) { 
      mAnimStep = 0;
    }
    else { 
      mAnimStep++; 
    }
  }

  if(directionX == 0 && directionY == 1) {
    switch(mAnimSequenz[mAnimStep]){
      case(0): { memCopy(mSpriteFigureFrontLeft); drawTile(relationX, relationY, 10, 16, mTempArray, false); break; }
      case(1): { memCopy(mSpriteFigureFrontMiddle); drawTile(relationX, relationY, 10, 16, mTempArray, false); break; }
      case(2): { memCopy(mSpriteFigureFrontLeft); drawTile(relationX, relationY, 10, 16, mTempArray, true); break; }
      default: {  break; }
    }
  }
  else if(directionX == -1 && directionY == 0) {
    switch(mAnimSequenz[mAnimStep]){
      case(0): { memCopy(mSpriteFigureSideLeft); drawTile(relationX, relationY, 10, 16, mTempArray, false);  break; }
      case(1): { memCopy(mSpriteFigureSideMiddle); drawTile(relationX, relationY, 10, 16, mTempArray, false); break; }
      case(2): { memCopy(mSpriteFigureSideRight); drawTile(relationX, relationY, 10, 16, mTempArray, false); break; }
      default: { break; }
    }
  }
  else if(directionX == 0 && directionY == -1) {
    switch(mAnimSequenz[mAnimStep]){
      case(0): { memCopy(mSpriteFigureBackLeft); drawTile(relationX, relationY, 10, 16, mTempArray, false);  break; }
      case(1): { memCopy(mSpriteFigureBackMiddle); drawTile(relationX, relationY, 10, 16, mTempArray, false);  break; }
      case(2): { memCopy(mSpriteFigureBackLeft); drawTile(relationX, relationY, 10, 16, mTempArray, true);  break; }
      default: {  break; }
    }
  }
  else if(directionX == 1 && directionY == 0) {
    switch(mAnimSequenz[mAnimStep]){
      case(0): { memCopy(mSpriteFigureSideLeft); drawTile(relationX, relationY, 10, 16, mTempArray, true); break; }
      case(1): { memCopy(mSpriteFigureSideMiddle); drawTile(relationX, relationY, 10, 16, mTempArray, true); break; }
      case(2): { memCopy(mSpriteFigureSideRight); drawTile(relationX, relationY, 10, 16, mTempArray, true);  break; }
      default: { break; }
    }
  }
}

// ========================================================================================
// Kopiert den Array Inhalt vom Flashspeicher in den SRAM
// Wird am meisten für die Spiel Figur verwendet.
// ----------------------------------------------------------------------------------------
// arrayContent[] = die zu kopierende byte Kette.
void memCopy(byte arrayContent[]) {                                    
  for(byte index = 0; index < 160; index++) {
    mTempArray[index] = pgm_read_byte_near(arrayContent + index);
  }
}