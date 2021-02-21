int animStep = 0;
void drawFigure(int directionX, int directionY, int relationX, int relationY) {

  if(gameTime % 4 > 0) {
    if(animStep > 2) {animStep = 0;}
    else {animStep++;}
  }

  if(directionX == 0 && directionY == 1) {
    switch(animStep){
      case(0): { drawFigurArray(relationX, relationY, spriteFigureFrontLeft, false, false); break; }
      case(1): { drawFigurArray(relationX, relationY, spriteFigureFrontMiddle, false, false); break; }
      case(2): { drawFigurArray(relationX, relationY, spriteFigureFrontLeft, true, false); break; }
      default: { drawFigurArray(relationX, relationY, spriteFigureFrontMiddle, false, false); break;  }
    }
  }
  else if(directionX == -1 && directionY == 0) {
    switch(animStep){
      case(0): { drawFigurArray(relationX, relationY, spriteFigureSideLeft, false, false);  break; }
      case(1): { drawFigurArray(relationX, relationY, spriteFigureSideMiddle, false, false); break; }
      case(2): { drawFigurArray(relationX, relationY, spriteFigureSideRight, false, false); break; }
      default: { drawFigurArray(relationX, relationY, spriteFigureSideMiddle, false, false); break; }
    }
  }
  else if(directionX == 0 && directionY == -1) {
    switch(animStep){
      case(0): { drawFigurArray(relationX, relationY, spriteFigureBackLeft, false, false);  break; }
      case(1): { drawFigurArray(relationX, relationY, spriteFigureBackMiddle, false, false);  break; }
      case(2): { drawFigurArray(relationX, relationY, spriteFigureBackLeft, true, false);  break; }
      default: { drawFigurArray(relationX, relationY, spriteFigureBackMiddle, false, false);  break; }
    }
  }
  else if(directionX == 1 && directionY == 0) {
    switch(animStep){
      case(0): { drawFigurArray(relationX, relationY, spriteFigureSideLeft, true, false); break; }
      case(1): { drawFigurArray(relationX, relationY, spriteFigureSideMiddle, true, false); break; }
      case(2): { drawFigurArray(relationX, relationY, spriteFigureSideRight, true, false);  break; }
      default: { drawFigurArray(relationX, relationY, spriteFigureSideMiddle, true, false); break; }
    }
  }
}

void drawFigurArray(int relationX, int relationY, byte figureArray[], boolean mirror, boolean clearColor) {
  int index = 0;
  for(int y = 0; y < 16; y++) {
    for(int x = 0; x < 10; x++) {
      if(clearColor) {

          EsploraTFT.drawPixel(relationX+x, relationY+y, mapNumberToColor(figureArray[1]));
      }
      else {

        int indexTarget = index;
        if(mirror) {
          indexTarget = index - x + (10 - x) - 1;
        }

        EsploraTFT.drawPixel(relationX+x, relationY+y, mapNumberToColor(figureArray[indexTarget]));
          
        index++;
      }
    }
  }
}

uint16_t mapNumberToColor(byte c) {
  uint16_t result = ST7735_RED;

  switch(c) {
    case(1):{ result = ST7735_BLACK; break; }
    case(2):{ result = 0xF590; break; } // haut
    case(3):{ result = 0x81E1; break; } // braun
    case(4):{ result = 0xC2C2; break; } // hell braun
    case(5):{ result = 0x8300; break; } // braun gelb
    case(6):{ result = 0x5406; break; } // gruen
    case(7):{ result = 0x32A4; break; } // dunkel gruen
    case(8):{ result = 0xAE91; break; } // hell gruen
    case(9):{ result = 0x2146; break; } // dunkel grau blau
    case(10):{ result = 0x31E9; break; } // grau blau
    case(11):{ result = 0x84B6; break; } // hell blau
    default: {
      result = 0;
      break;
    }
  }

  return result;
}