// ========================================================================================
// Description:       Animation der zu steuernden Figur.
// ========================================================================================

// ========================================================================================
// Variablen

byte mAnimStep = 1;                                                    // wird verwendet, um zu bestimmen, 
                                                                      // welchers Sprite Bild beim Rendern verwendet werden soll.
byte mAnimCounter = 0;                                                 // zaehlt die Bewegungsschritte bis zum naechsten Bild
#define ANIM_STEPS_PER_FRAME 5                                         // Pixel Schritte je Animationsbild

byte mAnimSequenz[4] = { 0, 1, 2, 1 };                                 // Animationssequenz. Ist um ein paar Bytes kleiner, 
                                                                      // als wenn man die function in case 1 nochmal in default einträgt.

// ========================================================================================
// Figur Sprites

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
// Naechstes Bild der Laufanimation.
void figureAdvanceAnimation() {

  mAnimCounter++;
  if(mAnimCounter >= ANIM_STEPS_PER_FRAME) {
    mAnimCounter = 0;
    mAnimStep = (mAnimStep + 1) % 4;
  }
}

// ========================================================================================
// Figur steht (mittleres Bild).
void figureStand() {
  mAnimStep = 1;
  mAnimCounter = 0;
}

// ========================================================================================
// Liefert die Farbnummer der Figur an einer Bildschirm Position.
// Das Sprite wird passend zur Blickrichtung und zum Animationsschritt gewaehlt
// und direkt aus dem Flash Speicher gelesen (kein Zwischenspeicher im SRAM).
// ----------------------------------------------------------------------------------------
// x, y     = Bildschirm Position
// Rueckgabe = Farbnummer, 0 = transparent / nicht getroffen
byte getFigurePixel(int x, int y) {

  int localX = x - mPosX;
  int localY = y - mPosY;

  if(localX < 0 || localX >= FIGURE_WIDTH || localY < 0 || localY >= FIGURE_HEIGHT) {
    return 0;
  }

  const byte* sprite;
  bool mirror = false;
  byte frame = mAnimSequenz[mAnimStep];

  if(mFacingY == 1) {                                                  // nach unten
    switch(frame){
      case(0): { sprite = mSpriteFigureFrontLeft; break; }
      case(1): { sprite = mSpriteFigureFrontMiddle; break; }
      default: { sprite = mSpriteFigureFrontLeft; mirror = true; break; }
    }
  }
  else if(mFacingY == -1) {                                            // nach oben
    switch(frame){
      case(0): { sprite = mSpriteFigureBackLeft; break; }
      case(1): { sprite = mSpriteFigureBackMiddle; break; }
      default: { sprite = mSpriteFigureBackLeft; mirror = true; break; }
    }
  }
  else {                                                               // nach links, nach rechts gespiegelt
    switch(frame){
      case(0): { sprite = mSpriteFigureSideLeft; break; }
      case(1): { sprite = mSpriteFigureSideMiddle; break; }
      default: { sprite = mSpriteFigureSideRight; break; }
    }
    mirror = (mFacingX == 1);
  }

  if(mirror) {
    localX = FIGURE_WIDTH - 1 - localX;
  }

  return pgm_read_byte(sprite + localY * FIGURE_WIDTH + localX);
}
