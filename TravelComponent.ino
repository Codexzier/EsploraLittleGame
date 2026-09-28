// ========================================================================================
// Description:       Reisen mit dem Schiff und dem Bus.
//                    Kapitaen und Busfahrer verkaufen Tickets. Die Fahrt wird als kleine
//                    Animation gezeigt:
//                    - Seekarte: Kueste, Nachbarinsel, gepunktete Route und ein Schiff
//                    - Bus:      Strasse von der Seite, der Bus faehrt durch das Bild
//                    Inseln und Bus werden aus Kreisen und Rechtecken berechnet,
//                    dadurch brauchen sie kaum Flash Speicher.
// ========================================================================================

// ========================================================================================
// Variablen

#define ANIM_FRAME_DELAY   25                                          // ms je Animationsbild
#define SHIP_WIDTH         12
#define SHIP_HEIGHT        10
#define SHIP_STEPS         60                                          // Bilder fuer die Ueberfahrt
#define BUS_WIDTH          48
#define BUS_HEIGHT         28
#define BUS_POS_Y          42
#define BUS_SPEED          3                                           // Pixel je Bild
#define BUS_SIGN_X         96                                          // Haltestellen Schild
#define BUS_STOP_RIGHT     40                                          // Halt bei Fahrt nach rechts (Front vor dem Schild)
#define BUS_STOP_LEFT      104                                         // Halt bei Fahrt nach links

int mShipX = 0;                                                        // Position des Schiffs auf der Seekarte
int mShipY = 0;
bool mShipMirror = false;                                              // Schiff faehrt nach links
int mBusX = 0;                                                         // Position des Busses
bool mBusFacingRight = true;                                           // Bus faehrt nach rechts

// ----------------------------------------------------------------------------------------
// Seekarte: Kreise der Landflaechen (Mitte X, Mitte Y, Radius)

#define SEA_LAND_COUNT 6
const PROGMEM byte mSeaLand[SEA_LAND_COUNT][3] = {
  {   0, 48, 40 },                                                     // Kueste mit Hafen
  {  18, 10, 22 },
  {  22, 92, 24 },
  { 118, 40, 14 },                                                     // Nachbarinsel
  { 129, 47, 11 },
  { 146, 80,  6 },                                                     // kleine Insel (Fotomotiv)
};

#define SHIP_START_X 38                                                // Hafen
#define SHIP_START_Y 56
#define SHIP_END_X   97                                                // Steg an der Nachbarinsel
#define SHIP_END_Y   44

// ----------------------------------------------------------------------------------------
// Texte

const PROGMEM char mCaptainOfferText[] = "Ahoi! Eine Rundfahrt zur Nachbarinsel kostet 60 Muenzen.";
const PROGMEM char mCaptainBoardText[] = "Ahoi! Alle an Bord? Wir fahren zur Nachbarinsel.";
const PROGMEM char mCaptainReturnText[] = "Zurueck zum Hafen?";
const PROGMEM char mDriverOfferText[] = "Mit dem Bus in die Stadt? Ein Ticket kostet 40 Muenzen.";
const PROGMEM char mDriverBoardText[] = "Einsteigen bitte! Naechster Halt: Stadt.";
const PROGMEM char mDriverReturnText[] = "Zurueck zur Haltestelle am Hafen?";
const PROGMEM char mSeaTitle[] = "Seekarte";
const PROGMEM char mSeaHarborLabel[] = "Hafen";
const PROGMEM char mSeaIslandLabel[] = "Insel";
const PROGMEM char mBusTitle[] = "Linie 1";

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Gespraech mit dem Kapitaen.
void openCaptainWindow() {

  if(mCurrentMap == MAP_ISLAND) {                                      // Rueckfahrt, das Ticket gilt fuer beide Wege
    openWindow(WIN_CHOICE, mTrader02Name, mCaptainReturnText);
    addWindowOption(OPT_TRAVEL);
    addWindowOption(OPT_NOT_NOW);
    return;
  }

  if(!hasItem(ITEM_BOAT_TICKET)) {
    openWindow(WIN_CHOICE, mTrader02Name, mCaptainOfferText);
    addWindowOption(OPT_BUY_BOAT_TICKET);
    addWindowOption(OPT_BYE);
    return;
  }

  openWindow(WIN_CHOICE, mTrader02Name, mCaptainBoardText);
  addWindowOption(OPT_TRAVEL);
  addWindowOption(OPT_NOT_NOW);
}

// ========================================================================================
// Gespraech mit dem Busfahrer.
void openDriverWindow() {

  if(mCurrentMap == MAP_CITY) {                                        // Rueckfahrt
    openWindow(WIN_CHOICE, mTrader03Name, mDriverReturnText);
    addWindowOption(OPT_TRAVEL);
    addWindowOption(OPT_NOT_NOW);
    return;
  }

  if(!hasItem(ITEM_BUS_TICKET)) {
    openWindow(WIN_CHOICE, mTrader03Name, mDriverOfferText);
    addWindowOption(OPT_BUY_BUS_TICKET);
    addWindowOption(OPT_BYE);
    return;
  }

  openWindow(WIN_CHOICE, mTrader03Name, mDriverBoardText);
  addWindowOption(OPT_TRAVEL);
  addWindowOption(OPT_NOT_NOW);
}

// ========================================================================================
// Faehrt mit der Figur auf der aktuellen Karte los (Schiff oder Bus).
void travelWithNpc() {

  mWindowType = WIN_NONE;                                              // Fenster wird von der Animation ueberdeckt
  mWindowOptionCount = 0;
  mFacingX = 0;
  mFacingY = 1;
  figureStand();

  switch(mCurrentMap) {
    case(MAP_HARBOR):   { playSeaTrip(false); loadMap(MAP_ISLAND, 51, 48); break; }
    case(MAP_ISLAND):   { playSeaTrip(true);  loadMap(MAP_HARBOR, 83, 32); break; }
    case(MAP_BUS_STOP): { playBusRide(true);  loadMap(MAP_CITY, 51, 48); break; }
    case(MAP_CITY):     { playBusRide(false); loadMap(MAP_BUS_STOP, 99, 48); break; }
    default:            { break; }
  }
}

// ========================================================================================
// Animation: Das Schiff faehrt auf der Seekarte zwischen Hafen und Nachbarinsel.
// ----------------------------------------------------------------------------------------
// backToHarbor = Rueckfahrt (Schiff faehrt nach links)
void playSeaTrip(bool backToHarbor) {

  int startX = backToHarbor ? SHIP_END_X : SHIP_START_X;
  int startY = backToHarbor ? SHIP_END_Y : SHIP_START_Y;
  int endX = backToHarbor ? SHIP_START_X : SHIP_END_X;
  int endY = backToHarbor ? SHIP_START_Y : SHIP_END_Y;

  mScene = SCENE_SEA;
  mShipMirror = backToHarbor;
  mShipX = startX;
  mShipY = startY;
  renderArea(0, 0, MAP_WIDTH, MAP_HEIGHT);

  for(byte i = 1; i < SHIP_STEPS; i += 3) {                            // gepunktete Route
    int x = SHIP_START_X + (SHIP_END_X - SHIP_START_X) * i / SHIP_STEPS + SHIP_WIDTH / 2;
    int y = SHIP_START_Y + (SHIP_END_Y - SHIP_START_Y) * i / SHIP_STEPS + SHIP_HEIGHT;
    EsploraTFT.drawPixel(x, y, colorOf(24));
  }

  drawTextP(56, 3, mSeaTitle, colorOf(24));                             // Beschriftung
  drawTextP(4, 76, mSeaHarborLabel, colorOf(24));
  drawTextP(108, 60, mSeaIslandLabel, colorOf(24));
  delay(400);

  for(int step = 1; step <= SHIP_STEPS; step++) {
    int oldX = mShipX;
    int oldY = mShipY;
    mShipX = startX + (endX - startX) * step / SHIP_STEPS;
    mShipY = startY + (endY - startY) * step / SHIP_STEPS + ((step >> 2) & 1);  // leichtes Schaukeln

    renderArea(min(oldX, mShipX), min(oldY, mShipY),
               SHIP_WIDTH + abs(mShipX - oldX), SHIP_HEIGHT + abs(mShipY - oldY));
    delay(ANIM_FRAME_DELAY);
  }

  delay(400);
  mScene = SCENE_MAP;
}

// ========================================================================================
// Animation: Der Bus faehrt von der Seite gesehen durch das Bild.
// Er haelt kurz an der Haltestelle und faehrt dann weiter.
// ----------------------------------------------------------------------------------------
// toCity = Fahrt in die Stadt (nach rechts), sonst zurueck (nach links)
void playBusRide(bool toCity) {

  mScene = SCENE_BUS;
  mBusFacingRight = toCity;
  mBusX = toCity ? -BUS_WIDTH : MAP_WIDTH;
  renderArea(0, 0, MAP_WIDTH, MAP_HEIGHT);
  drawTextP(4, 4, mBusTitle, colorOf(9));

  int stopX = toCity ? BUS_STOP_RIGHT : BUS_STOP_LEFT;                 // Halt an der Haltestelle
  int endX = toCity ? MAP_WIDTH : -BUS_WIDTH;

  moveBus(stopX);
  delay(600);                                                          // Figur steigt ein
  moveBus(endX);
  delay(200);
  mScene = SCENE_MAP;
}

// ========================================================================================
// Bewegt den Bus bis zur Ziel Position.
void moveBus(int targetX) {

  while(mBusX != targetX) {
    int oldX = mBusX;
    int distance = targetX - mBusX;
    int speed = min(abs(distance), BUS_SPEED);
    mBusX += distance > 0 ? speed : -speed;

    renderArea(min(oldX, mBusX), BUS_POS_Y, BUS_WIDTH + abs(mBusX - oldX), BUS_HEIGHT);
    delay(ANIM_FRAME_DELAY);
  }
}

// ========================================================================================
// Liefert die Farbnummer der Animation an einer Bildschirm Position.
byte getScenePixel(int x, int y) {

  if(mScene == SCENE_SEA) {
    return getSeaPixel(x, y);
  }

  return getBusScenePixel(x, y);
}

// ========================================================================================
// Seekarte: Schiff, Land (Gras mit Sandstrand) oder Wasser.
byte getSeaPixel(int x, int y) {

  int localX = x - mShipX;
  int localY = y - mShipY;
  if(localX >= 0 && localX < SHIP_WIDTH && localY >= 0 && localY < SHIP_HEIGHT) {
    if(mShipMirror) {
      localX = SHIP_WIDTH - 1 - localX;
    }
    byte c = getPackedPixel(mShipSprite, localY * SHIP_WIDTH + localX);
    if(c != 0) {
      return c;
    }
  }

  byte land = 0;                                                       // 0 = Wasser, 1 = Strand, 2 = Gras
  for(byte i = 0; i < SEA_LAND_COUNT && land < 2; i++) {
    int dx = x - pgm_read_byte(&mSeaLand[i][0]);
    int dy = y - pgm_read_byte(&mSeaLand[i][1]);
    int r = pgm_read_byte(&mSeaLand[i][2]);
    if(abs(dx) > r + 3 || abs(dy) > r + 3) {                            // schnell aussortieren
      continue;
    }
    int distance = dx * dx + dy * dy;
    if(distance <= r * r) { land = 2; }
    else if(distance <= (r + 3) * (r + 3)) { land = 1; }
  }

  byte index8 = (y & 7) * 8 + (x & 7);
  switch(land) {
    case(2):  { return pgm_read_byte(mTileGrass + index8); }
    case(1):  { return 27; }                                           // Sand
    default:  { return pgm_read_byte(mTileWater + index8); }
  }
}

// ========================================================================================
// Strasse von der Seite: Himmel, Huegel, Haltestelle, Strasse und Bus.
byte getBusScenePixel(int x, int y) {

  int localX = x - mBusX;
  int localY = y - BUS_POS_Y;
  if(localX >= 0 && localX < BUS_WIDTH && localY >= 0 && localY < BUS_HEIGHT) {
    if(!mBusFacingRight) {
      localX = BUS_WIDTH - 1 - localX;
    }
    byte c = getBusPixel(localX, localY);
    if(c != 0) {
      return c;
    }
  }

  if(x >= BUS_SIGN_X && x <= BUS_SIGN_X + 1 && y >= 38 && y < 66) { return 21; }  // Mast der Haltestelle
  int sx = x - BUS_SIGN_X;
  int sy = y - 34;
  if(sx * sx + sy * sy <= 25) { return (abs(sx) <= 1 || sy == 0) ? 6 : 12; }

  if(y < 56) {                                                         // Himmel mit Huegeln
    for(int hill = 20; hill < MAP_WIDTH; hill += 55) {
      int dx = x - hill;
      int dy = y - 64;
      if(abs(dx) < 30 && dx * dx + dy * dy < 900) {
        return 7;
      }
    }
    return 29;
  }

  if(y < 64) { return 6; }                                             // Wiese
  if(y < 68) { return 18; }                                            // Gehweg
  if((y == 84 || y == 85) && ((x >> 3) & 1) == 0) { return 24; }      // Mittellinie
  return 28;                                                           // Strasse
}

// ========================================================================================
// Bus von der Seite (Front rechts).
// ----------------------------------------------------------------------------------------
// x, y = Position im Bus (0 bis 47, 0 bis 27)
byte getBusPixel(int x, int y) {

  for(byte wheel = 0; wheel < 2; wheel++) {                            // Raeder
    int dx = x - (wheel == 0 ? 10 : 37);
    int dy = y - 22;
    int distance = dx * dx + dy * dy;
    if(distance <= 4) { return 18; }
    if(distance <= 25) { return 1; }
  }

  if(y > 20) {
    return 0;
  }

  if(x == 0 || x == BUS_WIDTH - 1 || y == 0 || y == 20) { return 1; }  // Rahmen
  if(x >= 44 && y >= 14 && y <= 16) { return 24; }                    // Scheinwerfer
  if(y == 13 || y == 14) { return 13; }                                // Zierstreifen
  if(x >= 30 && x <= 35 && y >= 3) { return y < 10 ? 11 : 21; }      // Tuer
  if(y >= 3 && y <= 9) {
    if(x >= 40 && x <= 45) { return 11; }                              // Frontscheibe
    if(x >= 3 && x < 28 && (x - 3) % 9 < 7) { return 11; }            // Fenster
  }
  return 12;                                                           // gelbe Karosserie
}
