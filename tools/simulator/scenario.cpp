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

// wie walkToMap, schliesst aber Hinweis Fenster unterwegs (z.B. Fotopunkt mit fertigem Foto)
static void walkToMapThrough(int dx, byte mapId) {
  for(int i = 0; i < 5 && mCurrentMap != mapId; i++) {
    walkToMap(dx, mapId);
    if(mWindowType == WIN_MESSAGE) { press(SWITCH_4); }
  }
}

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

// Bildschirmfoto waehrend einer Animation (beim n-ten Aufruf von delay)
static int gDelayCount = 0;
static int gShotAtDelay = -1;
static const char* gShotName = NULL;

static void onDelay() {
  gDelayCount++;
  if(gDelayCount == gShotAtDelay) { shot(gShotName); }
}

static void shotDuringAnimation(const char* name, int atDelay) {
  gDelayCount = 0;
  gShotAtDelay = atDelay;
  gShotName = name;
  gOnDelay = onDelay;
}

// ----------------------------------------------------------------------------------------
// Ablauf

int main(int argc, char** argv) {

  if(argc > 1) { gOutDir = argv[1]; }
  memset(gEeprom, 0xFF, sizeof(gEeprom));                              // leerer EEPROM wie ab Werk
  stick(0, 0);
  setup();
  step(10);
  check(mWindowType == WIN_TITLE && mWindowOptionCount == 1, "Startbildschirm ohne Spielstand: nur Neues Spiel");
  shot("00_title");
  press(SWITCH_2);
  check(mWindowType == WIN_TITLE, "Button 2 schliesst den Startbildschirm nicht");
  press(SWITCH_4);
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
  check(gLedFlashCount == 1, "Blitzlicht der RGB LED beim Foto");
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
  check(mQuestState == QUEST_MORE_PHOTOS && mCoins == PHOTO_REWARD, "Foto abgegeben, 150 Muenzen");
  press(SWITCH_4);
  step(5);
  check(mWindowType == WIN_MESSAGE, "Surie wuenscht sich drei weitere Fotos");
  shot("14_more_photos");
  press(SWITCH_4);
  step(5);
  check(mWindowType == WIN_NONE, "kein Spielende nach dem ersten Foto");

  printf("Schritt: Bruecke\n");
  walkY(48);
  walkToMap(1, MAP_GARDEN);
  walkToMapThrough(1, MAP_RIVER);
  check(mCurrentMap == MAP_RIVER, "Hecke im Garten ist offen, Fluss erreicht");
  shot("15a_river_map");
  walkX(51);
  walkY(32);
  step(5);
  check(mWindowType == WIN_CHOICE, "Fotopunkt an der Bruecke");
  shot("15_river");
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_PHOTO_BRIDGE), "Foto der Bruecke im Rucksack");
  press(SWITCH_4);

  printf("Schritt: Hafen und Bootsticket\n");
  walkY(48);
  walkToMap(1, MAP_HARBOR);
  check(mCurrentMap == MAP_HARBOR, "Hafen erreicht");
  shot("16a_harbor_map");
  bump(1, 0);
  step(5);
  check(mWindowType == WIN_CHOICE && mWindowOptions[0] == OPT_BUY_BOAT_TICKET, "Kapitaen bietet Ticket an");
  shot("16_harbor_captain");
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_BOAT_TICKET) && mCoins == PHOTO_REWARD - BOAT_TICKET_PRICE, "Bootsticket gekauft");
  press(SWITCH_4);
  walkX(mPosX - 10);
  bump(1, 0);
  step(5);
  check(mWindowType == WIN_CHOICE && mWindowOptions[0] == OPT_TRAVEL, "Kapitaen fragt nach Abfahrt");
  shotDuringAnimation("17_sea_trip", 35);
  press(SWITCH_4);
  gOnDelay = NULL;
  step(5);
  check(mCurrentMap == MAP_ISLAND && mScene == SCENE_MAP, "Ankunft auf der Nachbarinsel");
  check(Esplora.toneCount >= 3 && Esplora.toneFrequency == 0, "Schiffshorn gespielt und wieder aus");
  shot("18_island");

  printf("Schritt: Aus- und Einschalten, Spielstand laden\n");
  int savedCoins = mCoins;
  int savedX = mPosX;
  int savedY = mPosY;
  memset(mBackPlaces, 0, sizeof(mBackPlaces));                         // SRAM geht beim Ausschalten verloren
  memset(mTileConsumed, 0, sizeof(mTileConsumed));
  mQuestState = QUEST_START;
  mCoins = 0;
  setup();
  step(10);
  check(mWindowType == WIN_TITLE && mWindowOptionCount == 2 && mWindowChoice == 1,
        "Startbildschirm mit Spielstand: Spiel laden ist vorausgewaehlt");
  shot("18b_title_load");
  press(SWITCH_4);
  step(10);
  check(mCurrentMap == MAP_ISLAND && mPosX == savedX && mPosY == savedY && mCoins == savedCoins &&
        mQuestState == QUEST_MORE_PHOTOS && hasItem(ITEM_PHOTO_BRIDGE) && hasItem(ITEM_BOAT_TICKET) &&
        isHouseDoorOpen(), "Spielstand geladen: Karte, Position, Muenzen, Rucksack, Fortschritt");

  printf("Schritt: Insel\n");
  walkY(32);
  walkX(67);
  step(5);
  check(mWindowType == WIN_CHOICE, "Fotopunkt auf der Insel");
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_PHOTO_ISLAND), "Foto der Insel im Rucksack");
  press(SWITCH_4);
  walkY(48);
  bump(-1, 0);
  step(5);
  check(mWindowType == WIN_CHOICE && mWindowOptions[0] == OPT_TRAVEL, "Kapitaen bietet Rueckfahrt an");
  press(SWITCH_4);
  step(5);
  check(mCurrentMap == MAP_HARBOR && hasItem(ITEM_BOAT_TICKET), "zurueck im Hafen, Ticket gilt weiter");

  printf("Schritt: Bushaltestelle\n");
  walkY(16);
  walkX(131);
  walkY(32);
  walkToMap(1, MAP_BUS_STOP);
  check(mCurrentMap == MAP_BUS_STOP, "Bushaltestelle erreicht");
  shot("19a_bus_stop_map");
  walkX(99);
  walkY(48);
  bump(-1, 0);
  step(5);
  check(mWindowType == WIN_CHOICE && mWindowOptions[0] == OPT_BUY_BUS_TICKET, "Busfahrer bietet Ticket an");
  shot("19_bus_stop");
  press(SWITCH_4);
  step(5);
  check(hasItem(ITEM_BUS_TICKET) && mCoins == PHOTO_REWARD - BOAT_TICKET_PRICE - BUS_TICKET_PRICE,
        "Busticket gekauft");
  press(SWITCH_4);
  walkX(mPosX + 8);
  bump(-1, 0);
  step(5);
  shotDuringAnimation("20_bus_ride", 31);
  press(SWITCH_4);
  gOnDelay = NULL;
  step(5);
  check(mCurrentMap == MAP_CITY, "Ankunft am Stadtrand");
  shot("21a_city_map");

  printf("Schritt: Stadt\n");
  walkY(32);
  walkX(83);
  step(5);
  check(mWindowType == WIN_CHOICE, "Fotopunkt am Stadtrand");
  shot("21_city");
  press(SWITCH_4);
  step(5);
  check(hasAllNewPhotos(), "alle drei Fotos im Rucksack");
  press(SWITCH_4);
  walkY(48);
  bump(-1, 0);
  step(5);
  press(SWITCH_4);
  step(5);
  check(mCurrentMap == MAP_BUS_STOP, "mit dem Bus zurueck");

  printf("Schritt: Zurueck zu Surie\n");
  walkY(32);
  walkToMap(-1, MAP_HARBOR);
  walkY(16);
  walkX(67);
  walkY(32);
  walkToMap(-1, MAP_RIVER);
  walkToMap(-1, MAP_GARDEN);
  walkToMapThrough(-1, MAP_HOUSE);
  check(mCurrentMap == MAP_HOUSE, "zurueck im Haus");
  walkX(115);
  bump(0, -1);
  step(5);
  check(mWindowOptionCount == 2 && mWindowOptions[0] == OPT_GIVE_PHOTOS, "Option: Fotos geben");
  press(SWITCH_4);
  step(5);
  check(mQuestState == QUEST_PHOTOS_GIVEN && !hasItem(ITEM_PHOTO_CITY), "Fotos abgegeben");
  shot("22_photos_given");
  press(SWITCH_4);
  step(5);
  check(mWindowType == WIN_MESSAGE, "Surie laedt zum Kaffee ein");
  press(SWITCH_4);
  step(5);
  check(!mNpcActive, "Surie hat den Laden verlassen");

  printf("Schritt: Kaffee bei Surie\n");
  walkY(48);
  walkToMap(1, MAP_GARDEN);
  walk(1, 0, []() { return mPosX == 99; });
  if(mWindowType == WIN_MESSAGE) { press(SWITCH_4); }
  walkX(99);
  bump(0, -1);
  step(5);
  check(mCurrentMap == MAP_LIVING_ROOM, "Suries Haus betreten");
  bump(0, -1);
  step(5);
  check(mWindowType == WIN_MESSAGE, "Foto an der Wand ansehen");
  press(SWITCH_4);
  walkY(64);
  walkX(83);
  bump(0, -1);
  step(5);
  shot("23_living_room");
  check(mWindowType == WIN_CHOICE && mWindowOptions[0] == OPT_DRINK_COFFEE, "Surie bietet Kaffee an");
  press(SWITCH_4);
  step(5);
  press(SWITCH_4);
  step(5);
  check(mWindowType == WIN_END && mQuestState == QUEST_COFFEE, "Abschluss Fenster nach dem Kaffee");
  shot("24_end");
  press(SWITCH_4);
  step(5);
  shot("25_after_end");

  printf("Schritt: Neustart\n");
  hold(SWITCH_1, 400);
  check(mWindowType == WIN_NONE, "kurzes Druecken fuehrt nicht zum Startbildschirm");
  hold(SWITCH_1, 1200);
  step(5);
  check(mWindowType == WIN_TITLE && mWindowChoice == 1, "Button 1 halten: Startbildschirm");
  stick(0, -1);
  step(10);
  stick(0, 0);
  step(10);
  check(mWindowChoice == 0, "Neues Spiel ausgewaehlt");
  press(SWITCH_4);
  step(5);
  check(mQuestState == QUEST_START && mCoins == START_COINS && !hasItem(ITEM_KEY) &&
        mCurrentMap == MAP_HOUSE && !isHouseDoorOpen(), "Neustart setzt alles zurueck");
  shot("26_reset");

  printf("Statistik: %lu Pixel gesendet, %lu Adressfenster gesetzt\n",
         EsploraTFT.pixelWrites, EsploraTFT.addrWindowCount);
  printf(gFailures == 0 ? "ALLE PRUEFUNGEN OK\n" : "%d PRUEFUNG(EN) FEHLGESCHLAGEN\n", gFailures);
  return gFailures == 0 ? 0 : 1;
}
