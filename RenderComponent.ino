// Die Inhalte werden in Form von Kacheln gerendert.

// #######################################
// #          Variablen                  #
// #######################################

// Wird fuer die Zeichenausgabe von integer Werten verwendet.
char valuePrint[5];

// Zeichnet den INhalt eines Byte arrays
// - relationX  > Anfangsposition X
// - relationY  > Anfangsposition Y
// - tileWidth  > Pixel Breite
// - tileHeight > Pixel Hoehe
// - tilePic[]  > Byte Array mit dem Inhalt der zu rendernden Pixeln.
// - mirror     > Spiegelt das Bild Horizontal
void drawTile(int relationX, int relationY, byte tileWidth, byte tileHeight, byte tilePic[], boolean mirror) {
  int index = 0;
  for(int y = 0; y < tileHeight; y++) {
    for(int x = 0; x < tileWidth; x++) {

        int indexTarget = index;
        if(mirror) {
          indexTarget = index - x + (tileWidth - x) - 1;
        }

        byte colorNumber = tilePic[indexTarget];
        // Nur Farbe
        if(colorNumber != 0) {
          EsploraTFT.drawPixel(relationX+x, relationY+y, mapNumberToColor(colorNumber));
        }
        index++;
    }
  }
}

// Aenderbare Farben
int16_t spriteHairColor1 = 0xD615;
int16_t spriteHairColor2 = 0xBD30;
int16_t spriteShirtColor1 = 0xD69A;
int16_t spriteShirtColor2 = 0xB596;
int16_t spritePantsColor1 = 0x0418;
int16_t spritePantsColor2 = 0x0312;

int16_t boxColor = 0xDCFE;

// gibt den Farbweter zurück, 
// der hinter der Farbnummer abgelegt wurde.
uint16_t mapNumberToColor(byte c) {
  uint16_t result = ST7735_RED;

  switch(c)  {
    case(1): { result = ST7735_BLACK; break; }
    case(2): { result = 0xF590; break; } // haut
    case(3): { result = 0x81E1; break; } // braun
    case(4): { result = 0xC2C2; break; } // hell braun
    case(5): { result = 0x8300; break; } // braun gelb
    case(6): { result = 0x5406; break; } // gruen
    case(7): { result = 0x32A4; break; } // dunkel gruen
    case(8): { result = 0xAE91; break; } // hell gruen
    case(9): { result = 0x2146; break; } // dunkel grau blau
    case(10):{ result = 0x31E9; break; } // grau blau
    case(11):{ result = 0x84B6; break; } // hell blau
    case(12):{ result = 0xFFE0; break; } // gelb
    case(13):{ result = 0xFC08; break; } // orange
    case(14):{ result = 0xFA8A; break; } // hell rot
    case(15):{ result = 0xD759; break; } // hell gruen 2

    case(16):{ result = 0xF800; break; } // rot
    case(17):{ result = 0x8208; break; } // dunkel braun
    case(18):{ result = 0xC618; break; } // grau
    case(19):{ result = 0xF7BE; break; } // sehr hell grau
    case(20):{ result = 0xFE97; break; } // hell haut

    case(100):{ result = spriteHairColor1; break; } // Haare 1
    case(101):{ result = spriteHairColor2; break; } // Haare 2
    case(102):{ result = spriteShirtColor1; break; } // T-Shirt 1
    case(103):{ result = spriteShirtColor2; break; } // T-Shirt 2
    case(104):{ result = spritePantsColor1; break; } // Hose 1
    case(105):{ result = spritePantsColor2; break; } // Hose 2
    
    default: {
      result = 0;
      break;
    }
  }

  return result;
}

// Schreibt den Text in weis
void writeText(int x, int y, char text[]) {
  EsploraTFT.stroke(200, 200, 200);
  EsploraTFT.text(text, x, y);
}

// Schreibt den Wert in weis
void writeValue(int x, int y, int val, boolean clr) {
  if(clr) {
    EsploraTFT.stroke(0, 0, 0);
  }
  else {
    EsploraTFT.stroke(200, 200, 200);
  }
  String accResult = String(val);
  accResult.toCharArray(valuePrint, 5);
  EsploraTFT.text(valuePrint, x, y);
}