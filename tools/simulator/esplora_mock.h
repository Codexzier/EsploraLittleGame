// ========================================================================================
// Description:       Nachbildung von Arduino, TFT und Esplora fuer den PC.
//                    Der Bildschirm wird in einen Speicher gezeichnet und kann als
//                    Bild gespeichert werden. Joystick und Buttons werden vom
//                    Testablauf gesetzt.
// ========================================================================================
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef uint8_t byte;
typedef bool boolean;

#define PROGMEM
#define pgm_read_byte(p) (*(const uint8_t*)(p))
#define pgm_read_byte_near(p) (*(const uint8_t*)(p))
#define pgm_read_word(p) (*(const uint16_t*)(p))
#define LOW 0
#define HIGH 1

template <typename A, typename B> static inline A min(A a, B b) { return a < b ? a : (A)b; }
template <typename A, typename B> static inline A max(A a, B b) { return a > b ? a : (A)b; }

static inline char* itoa(int value, char* buffer, int) { sprintf(buffer, "%d", value); return buffer; }

extern unsigned long gSimMillis;
extern void (*gOnDelay)();                                             // wird bei jedem delay() aufgerufen (Bildschirmfotos waehrend Animationen)
static inline unsigned long millis() { return gSimMillis; }
static inline void delay(unsigned long ms) { gSimMillis += ms; if(gOnDelay) { gOnDelay(); } }

// ----------------------------------------------------------------------------------------
// Display

#define INITR_BLACKTAB 0x2
#define ST7735_RED 0xF800

extern const unsigned char font[];                                    // glcdfont.c der TFT Bibliothek

class MockTFT {
public:
  uint16_t pixels[128][160];
  int winX0, winY0, winX1, winY1, curX, curY;
  unsigned long pushCount;                                             // Anzahl gesendeter Pixel (Statistik)
  unsigned long addrWindowCount;                                       // Anzahl gesetzter Adressfenster (Statistik)
  unsigned long pixelWrites;                                           // Anzahl geschriebener Pixel (Statistik)

  MockTFT() { memset(pixels, 0, sizeof(pixels)); pushCount = 0; addrWindowCount = 0; pixelWrites = 0; }

  // grobe Schaetzung der SPI Bytes: je Adressfenster 11 Bytes, je Pixel 2 Bytes
  unsigned long spiBytes() { return addrWindowCount * 11 + pixelWrites * 2; }
  void begin() {}
  void initR(uint8_t) {}
  void setRotation(uint8_t) {}
  int16_t width() { return 160; }
  int16_t height() { return 128; }

  void background(uint8_t r, uint8_t g, uint8_t b) {
    fillRect(0, 0, 160, 128, ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
  }

  void drawPixel(int16_t x, int16_t y, uint16_t c) {
    addrWindowCount++;
    pixelWrites++;
    if(x >= 0 && x < 160 && y >= 0 && y < 128) { pixels[y][x] = c; }
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c) {
    addrWindowCount++;
    for(int j = y; j < y + h; j++) {
      for(int i = x; i < x + w; i++) {
        pixelWrites++;
        if(i >= 0 && i < 160 && j >= 0 && j < 128) { pixels[j][i] = c; }
      }
    }
  }

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c) {
    fillRect(x, y, w, 1, c); fillRect(x, y + h - 1, w, 1, c);
    fillRect(x, y, 1, h, c); fillRect(x + w - 1, y, 1, h, c);
  }

  void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    addrWindowCount++;
    winX0 = x0; winY0 = y0; winX1 = x1; winY1 = y1; curX = x0; curY = y0;
  }

  void pushColor(uint16_t c) {
    pushCount++;
    pixelWrites++;
    if(curX >= 0 && curX < 160 && curY >= 0 && curY < 128) { pixels[curY][curX] = c; }
    curX++;
    if(curX > winX1) { curX = winX0; curY++; }
  }

  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t) {
    for(int8_t i = 0; i < 6; i++) {                                    // wie Adafruit_GFX::drawChar
      uint8_t line = (i == 5) ? 0 : font[(c * 5) + i];
      for(int8_t j = 0; j < 8; j++) {
        if(line & 0x1) { drawPixel(x + i, y + j, color); }
        else if(bg != color) { drawPixel(x + i, y + j, bg); }
        line >>= 1;
      }
    }
  }
};

extern MockTFT EsploraTFT;

// ----------------------------------------------------------------------------------------
// Esplora

const byte SWITCH_1 = 1;
const byte SWITCH_2 = 2;
const byte SWITCH_3 = 3;
const byte SWITCH_4 = 4;

class MockEsplora {
public:
  int joystickX = 0;                                                   // Rohwert wie vom Esplora
  int joystickY = 0;
  bool buttons[5] = { false, false, false, false, false };             // true = gedrueckt

  int readJoystickX() { return joystickX; }
  int readJoystickY() { return joystickY; }
  int readButton(byte sw) { return buttons[sw] ? LOW : HIGH; }
};

extern MockEsplora Esplora;
