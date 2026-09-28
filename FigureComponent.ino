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
// Sprites je Blickrichtung (unten, oben, seitlich) und Animationsbild (links, mitte, rechts).
// Das rechte Bild ist bei unten / oben das gespiegelte linke Bild.
// Die Sprites stehen in AssetsData.ino.

const byte* const PROGMEM mFigureSprites[3][3] = {
  { mSpriteFigureFrontLeft, mSpriteFigureFrontMiddle, mSpriteFigureFrontLeft },
  { mSpriteFigureBackLeft,  mSpriteFigureBackMiddle,  mSpriteFigureBackLeft  },
  { mSpriteFigureSideLeft,  mSpriteFigureSideMiddle,  mSpriteFigureSideRight },
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
// und direkt aus dem Flash Speicher gelesen (gepackt, siehe AssetsData.ino).
// ----------------------------------------------------------------------------------------
// x, y     = Bildschirm Position
// Rueckgabe = Farbnummer, 0 = transparent / nicht getroffen
byte getFigurePixel(int x, int y) {

  int localX = x - mPosX;
  int localY = y - mPosY;

  if(localX < 0 || localX >= FIGURE_WIDTH || localY < 0 || localY >= FIGURE_HEIGHT) {
    return 0;
  }

  byte frame = mAnimSequenz[mAnimStep];
  byte row = (mFacingY == 1) ? 0 : ((mFacingY == -1) ? 1 : 2);          // unten, oben, seitlich
  const byte* sprite = (const byte*)pgm_read_ptr(&mFigureSprites[row][frame]);
  bool mirror = (row == 2) ? (mFacingX == 1) : (frame == 2);           // nach rechts gespiegelt

  if(mirror) {
    localX = FIGURE_WIDTH - 1 - localX;
  }

  return getPackedPixel(sprite, localY * FIGURE_WIDTH + localX);
}
