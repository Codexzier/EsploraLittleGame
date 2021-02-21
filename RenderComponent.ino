// Die Inhalte werden in Form von Kacheln gerendert.

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

// gibt den Farbweter zurück, 
// der hinter der Farbnummer abgelegt wurde.
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
    case(13):{ result = 0xFC08; break; } // orange
    case(14):{ result = 0xFA8A; break; } // hell rot
    case(15):{ result = 0xD759; break; } // hell gruen 2
    default: {
      result = 0;
      break;
    }
  }

  return result;
}