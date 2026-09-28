// ========================================================================================
// Description:       Die Inhalte werden in Form von Kacheln gerendert.
// ----------------------------------------------------------------------------------------
// Vorgehen:          Ein Bereich wird Pixel fuer Pixel zusammengesetzt
//                    (Karte -> Surie -> Figur, nach Tiefe sortiert) und mit
//                    setAddrWindow + pushColor in einem Rutsch an das Display gesendet.
//                    - kein Flackern, da jeder Pixel nur einmal geschrieben wird
//                    - deutlich schneller als drawPixel, da das Adressfenster nur
//                      einmal je Bereich gesetzt wird
//                    - kein Zwischenspeicher im SRAM noetig, alles kommt aus dem Flash
// ========================================================================================

// ========================================================================================
// Farben (RGB565), Index = Farbnummer in den Sprites und Kacheln

const PROGMEM uint16_t mPalette[] = {
  0x0000,                                                              // 0  durchsichtig
  0x0000,                                                              // 1  schwarz
  0xF590,                                                              // 2  haut
  0x81E1,                                                              // 3  braun
  0xC2C2,                                                              // 4  hell braun
  0x8300,                                                              // 5  braun gelb
  0x5406,                                                              // 6  gruen
  0x32A4,                                                              // 7  dunkel gruen
  0xAE91,                                                              // 8  hell gruen
  0x2146,                                                              // 9  dunkel grau blau
  0x31E9,                                                              // 10 grau blau
  0x84B6,                                                              // 11 hell blau
  0xFFE0,                                                              // 12 gelb
  0xFC08,                                                              // 13 orange
  0xFA8A,                                                              // 14 hell rot
  0xD759,                                                              // 15 hell gruen 2
  0xF800,                                                              // 16 rot
  0x8208,                                                              // 17 dunkel braun
  0xC618,                                                              // 18 grau
  0xF7BE,                                                              // 19 sehr hell grau
  0xFE97,                                                              // 20 hell haut
  0x4208,                                                              // 21 dunkel grau
  0xA145,                                                              // 22 ziegel rot
  0x6180,                                                              // 23 holz dunkel
  0xFFFF,                                                              // 24 weiss
};

#define PALETTE_COUNT 25

char mValuePrint[7];                                                   // Wird fuer die Zeichenausgabe
                                                                       // von integer Werten verwendet.

// ========================================================================================
// gibt den Farbweter zurück,
// der hinter der Farbnummer abgelegt wurde.
// ----------------------------------------------------------------------------------------
// c = Farbnummer. Ab 100 sind es die veraenderbaren Farben einer Figur (Haare, Shirt, Hose).
uint16_t colorOf(byte c) {

  if(c >= 100) {
    return getNpcColor(c);
  }

  if(c >= PALETTE_COUNT) {
    return ST7735_RED;                                                 // unbekannte Farbe gut sichtbar
  }

  return pgm_read_word(&mPalette[c]);
}

// ========================================================================================
// Setzt die Farbe eines Pixels aus allen Ebenen zusammen.
// Wer weiter unten steht, wird davor gezeichnet.
// ----------------------------------------------------------------------------------------
// x, y = Bildschirm Position innerhalb der Karte
uint16_t composePixel(int x, int y) {

  byte c;

  if(mPosY >= mNpcY) {                                                 // Figur steht vor Surie
    c = getFigurePixel(x, y);
    if(c == 0) { c = getNpcPixel(x, y); }
  }
  else {                                                               // Surie steht vor der Figur
    c = getNpcPixel(x, y);
    if(c == 0) { c = getFigurePixel(x, y); }
  }

  if(c == 0) {
    c = getTilePixel(x, y);
  }

  return colorOf(c);
}

// ========================================================================================
// Zeichnet einen Bereich der Karte neu.
// ----------------------------------------------------------------------------------------
// x, y          = Anfangsposition
// width, height = Groesse des Bereiches
void renderArea(int x, int y, int width, int height) {

  if(x < 0) { width += x; x = 0; }                                     // auf die Karte begrenzen
  if(y < 0) { height += y; y = 0; }
  if(x + width > MAP_WIDTH) { width = MAP_WIDTH - x; }
  if(y + height > MAP_HEIGHT) { height = MAP_HEIGHT - y; }

  if(width <= 0 || height <= 0) {
    return;
  }

  EsploraTFT.setAddrWindow(x, y, x + width - 1, y + height - 1);

  for(int py = y; py < y + height; py++) {
    for(int px = x; px < x + width; px++) {
      EsploraTFT.pushColor(composePixel(px, py));
    }
  }
}

// ========================================================================================
// Zeichnet ein Bild direkt aus dem Flash Speicher.
// ----------------------------------------------------------------------------------------
// x, y            = Anfangsposition
// width, height   = Groesse des Bildes
// icon            = Byte Array im Flash Speicher
// backgroundColor = Farbnummer fuer durchsichtige Pixel
void drawIcon(int x, int y, byte width, byte height, const byte* icon, byte backgroundColor) {

  EsploraTFT.setAddrWindow(x, y, x + width - 1, y + height - 1);

  int count = width * height;
  for(int index = 0; index < count; index++) {
    byte c = pgm_read_byte(icon + index);
    EsploraTFT.pushColor(colorOf(c == 0 ? backgroundColor : c));
  }
}

// ========================================================================================
// Schreibt einen Text aus dem Flash Speicher (ohne Hintergrund).
// ----------------------------------------------------------------------------------------
// x, y  = Anfangsposition
// text  = Text im Flash Speicher
// color = Farbwert (RGB565)
void drawTextP(int x, int y, const char* text, uint16_t color) {

  char c = pgm_read_byte(text);
  while(c != 0) {
    EsploraTFT.drawChar(x, y, c, color, color, 1);                     // gleiche Farbe = ohne Hintergrund
    x += 6;
    text++;
    c = pgm_read_byte(text);
  }
}

// ========================================================================================
// Schreibt eine Zahl (ohne Hintergrund).
// ----------------------------------------------------------------------------------------
// x, y  = Anfangsposition
// value = Zahl
// color = Farbwert (RGB565)
void drawNumber(int x, int y, int value, uint16_t color) {

  itoa(value, mValuePrint, 10);
  for(byte i = 0; mValuePrint[i] != 0; i++) {
    EsploraTFT.drawChar(x, y, mValuePrint[i], color, color, 1);
    x += 6;
  }
}
