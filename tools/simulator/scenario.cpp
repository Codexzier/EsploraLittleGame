// ========================================================================================
// Description:       Testablauf fuer den PC Simulator.
//                    Spielt das Spiel einmal komplett durch, prueft den Spielstand
//                    nach jedem Schritt und speichert Bildschirmfotos.
// ========================================================================================

#include <functional>
#include <string>

static int gFailures = 0;
static const char* gOutDir = ".";

// ----------------------------------------------------------------------------------------
// Hilfsfunktionen

static void step(int count = 1) {
  for(int i = 0; i < count; i++) {
    gSimMillis += 2;
    loop();
  }
}

static void stick(int dx, int dy) {
  Esplora.joystickX = dx == 1 ? -300 : (dx == -1 ? 300 : mOffsetX);   // X Achse ist gespiegelt
  Esplora.joystickY = dy == -1 ? -300 : (dy == 1 ? 300 : mOffsetY);
}

static void release() {
  stick(0, 0);
  step(20);
}

static void press(byte sw) {
  Esplora.buttons[sw] = true;
  step(5);
  Esplora.buttons[sw] = false;
  step(5);
}

static void hold(byte sw, int ms) {
  Esplora.buttons[sw] = true;
  step(ms / 2);
  Esplora.buttons[sw] = false;
  step(5);
}

// laeuft in eine Richtung, bis die Bedingung erfuellt ist oder ein Fenster aufgeht
static void walk(int dx, int dy, std::function<bool()> done) {
  stick(dx, dy);
  for(int i = 0; i < 20000 && !done() && mWindowType == WIN_NONE; i++) {
    step();
  }
  release();
}

static void walkX(int x) { walk(x > mPosX ? 1 : -1, 0, [x]() { return mPosX == x; }); }
static void walkY(int y) { walk(0, y > mPosY ? 1 : -1, [y]() { return mPosY == y; }); }
static void bump(int dx, int dy) { walk(dx, dy, []() { return false; }); }
static void walkToMap(int dx, byte mapId) { walk(dx, 0, [mapId]() { return mCurrentMap == mapId; }); }

static void check(bool ok, const char* what) {
  printf("  [%s] %s\n", ok ? " OK " : "FAIL", what);
  if(!ok) { gFailures++; }
}

static void shot(const char* name) {
  std::string path = std::string(gOutDir) + "/" + name + ".ppm";
  FILE* f = fopen(path.c_str(), "wb");
  fprintf(f, "P6\n160 128\n255\n");
  for(int y = 0; y < 128; y++) {
    for(int x = 0; x < 160; x++) {
      uint16_t c = EsploraTFT.pixels[y][x];
      unsigned char rgb[3] = { (unsigned char)(((c >> 11) & 0x1F) * 255 / 31),
                               (unsigned char)(((c >> 5) & 0x3F) * 255 / 63),
                               (unsigned char)((c & 0x1F) * 255 / 31) };
      fwrite(rgb, 1, 3, f);
    }
  }
  fclose(f);
  printf("  -> %s\n", name);
}

// ----------------------------------------------------------------------------------------
// Ablauf

int main(int argc, char** argv) {

  if(argc > 1) { gOutDir = argv[1]; }
  stick(0, 0);
  setup();
  step(10);
  shot("01_start");
  check(mCurrentMap == MAP_HOUSE && mCoins == START_COINS, "Start im Haus mit 25 Muenzen");

  printf("Schritt: Messung Zeichenaufwand\n");
  unsigned long spiBefore = EsploraTFT.spiBytes();
  walkX(mPosX + 20);
  printf("  20 Pixel Schritte: ca. %lu SPI Bytes (%lu je Schritt)\n",
         EsploraTFT.spiBytes() - spiBefore, (EsploraTFT.spiBytes() - spiBefore) / 20);
  walkX(16);

  printf("Schritt: Tuer ohne Schluessel\n");
  walkY(48);
  walkX(60);
  walkY(60);
  bump(1, 0);
  step(5);
  shot("02_door_locked");
  check(mWindowType == WIN_MESSAGE, "Tuer ist verschlossen");
  press(SWITCH_2);

  printf("Schritt: Schluessel\n");
  walkY(16);
  walkX(64);
  step(5);
  shot("03_key_found");
  check(hasItem(ITEM_KEY), "Schluessel im Rucksack");
  press(SWITCH_4);
  check(mWindowType == WIN_NONE, "Fenster geschlossen");
  walkX(40);
  walkX(64);
  check(mWindowType == WIN_NONE && hasItem(ITEM_KEY), "Schluessel nicht doppelt aufgehoben");

  printf("Schritt: Kiste\n");
  walkY(48);
  walkX(19);
  bump(0, 1);
  step(5);
  shot("04_chest");
  check(mWindowType == WIN_CHOICE, "Kiste fragt nach Oeffnen");
  press(SWITCH_4);
  step(5);
  check(mCoins == START_COINS + CHEST_COINS, "75 Muenzen aus der Kiste");
  press(SWITCH_4);
  bump(0, -1);
  bump(0, 1);
  step(5);
  check(mWindowType == WIN_MESSAGE, "Kiste ist leer");
  press(SWITCH_4);

  printf("Schritt: Tuer oeffnen\n");
  walkX(60);
  walkY(60);
  bump(1, 0);
  step(5);
  check(!hasItem(ITEM_KEY) && isHouseDoorOpen(), "Tuer offen, Schluessel verbraucht");
  shot("05_door_open");
  press(SWITCH_4);
  walkX(100);
  check(mPosX == 100, "durch die Tuer gegangen");

  printf("Schritt: Surie\n");
  walkX(115);
  bump(0, -1);
  step(5);
  shot("06_surie_intro");
  check(mQuestState == QUEST_TALKED, "Auftrag erhalten");
  press(SWITCH_4);
  walkY(mPosY + 6);
  bump(0, -1);
  step(5);
  check(mWindowType == WIN_CHOICE && mWindowOptionCount == 2, "Handel: Kamera kaufen / Tschuess");
  shot("07_trader_menu");
  press(SWITCH_4);
  step(5);
  check(!hasItem(ITEM_CAMERA) && mCoins == 100, "nicht genug Muenzen");
  press(SWITCH_4);

  printf("Schritt: Garten\n");
  walkY(48);
  walkToMap(1, MAP_GARDEN);
  check(mCurrentMap == MAP_GARDEN, "Kartenwechsel in den Garten");
  shot("08_garden");
  walkY(12);
  walkX(32);
  walkY(56);
  walkX(19);
  walkX(131);
  walkY(28);
  check(mCoins == 200, "vier Muenzen im Garten gesammelt");
  walkY(44);
  walkX(67);
  step(5);
  shot("09_photo_spot_no_camera");
  check(mWindowType == WIN_MESSAGE, "Fotopunkt ohne Kamera");
  press(SWITCH_4);

  printf("Schritt: Kamera kaufen\n");
  walkY(48);
  walkToMap(-1, MAP_HOUSE);
  check(mCurrentMap == MAP_HOUSE, "zurueck im Haus");
  walkX(115);
  bump(0, -1);
  step(5);
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_CAMERA) && mCoins == 0, "Kamera gekauft");
  shot("10_camera_bought");
  press(SWITCH_4);

  printf("Schritt: Foto machen\n");
  walkY(48);
  walkToMap(1, MAP_GARDEN);
  walkX(67);
  step(5);
  check(mWindowType == WIN_CHOICE, "Fotopunkt fragt nach Foto");
  shot("11_photo_spot");
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_PHOTO), "Foto im Rucksack");
  press(SWITCH_4);
  shot("12_garden_with_photo");

  printf("Schritt: Foto abgeben\n");
  walkToMap(-1, MAP_HOUSE);
  walkX(115);
  bump(0, -1);
  step(5);
  check(mWindowOptionCount == 2 && mWindowOptions[0] == OPT_GIVE_PHOTO, "Option: Foto geben");
  stick(0, 1);
  step(10);
  stick(0, 0);
  step(10);
  check(mWindowChoice == 1, "Auswahl mit dem Joystick nach unten");
  shot("13_trader_choice_moved");
  stick(0, -1);
  step(10);
  stick(0, 0);
  step(10);
  check(mWindowChoice == 0, "Auswahl mit dem Joystick nach oben");
  press(SWITCH_4);
  step(5);
  check(mQuestState == QUEST_DONE && mCoins == PHOTO_REWARD, "Foto abgegeben, 150 Muenzen");
  press(SWITCH_4);
  step(5);
  check(mWindowType == WIN_END, "Abschluss Fenster");
  shot("14_end");
  press(SWITCH_4);
  step(5);
  shot("15_after_end");

  printf("Schritt: Neustart\n");
  hold(SWITCH_1, 400);
  check(mQuestState == QUEST_DONE, "kurzes Druecken setzt nicht zurueck");
  hold(SWITCH_1, 1200);
  step(5);
  check(mQuestState == QUEST_START && mCoins == START_COINS && !hasItem(ITEM_KEY) &&
        mCurrentMap == MAP_HOUSE && !isHouseDoorOpen(), "Neustart setzt alles zurueck");
  shot("16_reset");

  printf("Statistik: %lu Pixel gesendet, %lu Adressfenster gesetzt\n",
         EsploraTFT.pixelWrites, EsploraTFT.addrWindowCount);
  printf(gFailures == 0 ? "ALLE PRUEFUNGEN OK\n" : "%d PRUEFUNG(EN) FEHLGESCHLAGEN\n", gFailures);
  return gFailures == 0 ? 0 : 1;
}
