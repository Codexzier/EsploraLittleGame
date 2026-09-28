// ========================================================================================
// Description:       Grundeinstellung des Rucksackes
//                    Der Rucksack hat 6 Taschenplaetze (unten links auf dem Bildschirm).
//                    Die Icons stehen in AssetsData.ino (erzeugt mit tools/sprites.py).
// ========================================================================================

// ========================================================================================
// Objeckte
// Name (sollte sich auf moeglich wenig Zeichen beschraenken)
// Bild (gepacktes byte array in AssetsData.ino)
// Beschreibung (nur bedingt verwenden)
// Verkaufswert (einige Dinge können gehandelt werden)
// Kaufwert (Haendler Preis)

// ========================================================================================
// ITEMS
// ----------------------------------------------------------------------------------------
// ID 0
// '0' bedeutet immer nicht belegt.
// ========================================================================================
// ID 01

const PROGMEM char mItemKey01[] = "Schluessel";                          // Name
const PROGMEM char mItemKey01Description[] = "Oeffnet eine Tuer";      // Beschreibung
                                                                        // Verkaufswert         = 0 (Kann nicht verkauft werden)
                                                                        // Kaufwert             = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)

// ========================================================================================
// ID 02

const PROGMEM char mItemCamera[] = "Kamera";                            // Name
const PROGMEM char mItemCameraDescription[] = "Mach ein paar Fotos!";  // Beschreibung
const PROGMEM uint16_t mItemCameraSellValue = 140;                       // Verkaufswert
const PROGMEM uint16_t mItemCameraBuyValue = 200;                        // Kaufwert

// ========================================================================================
// ID 03 bis 06 (Fotos, werden fuer Suries Auftrag abgegeben)

const PROGMEM char mItemPhoto01Description[] = "Sonne, Baum und Haus"; // Beschreibung
const PROGMEM char mItemPhotoBridgeDescription[] = "Die alte Bruecke ueber den Fluss";
const PROGMEM char mItemPhotoIslandDescription[] = "Die Insel mitten im Meer";
const PROGMEM char mItemPhotoCityDescription[] = "Die Stadt mit ihren vielen Haeusern";

// ========================================================================================
// ID 07 und 08 (Tickets, gelten fuer beliebig viele Fahrten)

const PROGMEM char mItemBoatTicket[] = "Bootsticket";
const PROGMEM char mItemBusTicket[] = "Busticket";

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Pruefen ob das Item bereits vorhanden ist ein Item kann nur einmal vorhanden sein
// ----------------------------------------------------------------------------------------
// itemId = Id Nummer, dass in in einem Taschenplatz hinterlegt wurde.
bool hasItem(uint16_t itemId) {

  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {
    if(mBackPlaces[index] == itemId) {
      return true;
    }
  }

  return false;
}

// ========================================================================================
// legt das Item in die Tasche ab und Zeichnet es in einen offen Taschenplatz
// ----------------------------------------------------------------------------------------
// itemId = Gegenstands Id Nummer. Damit wird das Icon Bild abgerufen
bool addItem(uint16_t itemId) {

  if(hasItem(itemId)) {
    return false;
  }

  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {        // id ablegen in ersten freien Taschenplatz
    if(mBackPlaces[index] == ITEM_NONE) {
      mBackPlaces[index] = itemId;
      drawBackpackPlace(index);
      return true;
    }
  }

  return false;                                                         // Rucksack ist voll
}

// ========================================================================================
// nimmt das Item aus dem Rucksack und zeichnet den Taschenplatz leer.
// ----------------------------------------------------------------------------------------
// itemId = Gegenstands Id Nummer
bool removeItem(uint16_t itemId) {

  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {
    if(mBackPlaces[index] == itemId) {
      mBackPlaces[index] = ITEM_NONE;
      drawBackpackPlace(index);
      return true;
    }
  }

  return false;
}

// ========================================================================================
// Zeichnet alle Taschenplaetze.
void drawBackpack() {

  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {
    drawBackpackPlace(index);
  }
}

// ========================================================================================
// Liefert das Icon zu einem Item (gepackt, 16x16).
const byte* getItemIcon(uint16_t itemId) {

  switch(itemId) {
    case(ITEM_KEY):          { return mItemKey01Icon; }
    case(ITEM_CAMERA):       { return mItemCameraIcon; }
    case(ITEM_PHOTO):        { return mItemPhoto01Icon; }
    case(ITEM_PHOTO_BRIDGE): { return mItemPhotoBridgeIcon; }
    case(ITEM_PHOTO_ISLAND): { return mItemPhotoIslandIcon; }
    case(ITEM_PHOTO_CITY):   { return mItemPhotoCityIcon; }
    case(ITEM_BOAT_TICKET):  { return mItemBoatTicketIcon; }
    case(ITEM_BUS_TICKET):   { return mItemBusTicketIcon; }
    default:                 { return NULL; }                          // Nicht belegt
  }
}

// ========================================================================================
// Zeichnet einen Taschenplatz mit dem Icon des Items oder leer.
// ----------------------------------------------------------------------------------------
// place = Taschenplatz 0 bis 5
void drawBackpackPlace(byte place) {

  int x = (place % 3) * MAP_TILE_SIZE;                                  // drei Spalten
  int y = HUD_POS_Y + (place / 3) * MAP_TILE_SIZE;                      // zwei Zeilen

  const byte* icon = getItemIcon(mBackPlaces[place]);

  if(icon != NULL) {
    drawPackedIcon(x, y, MAP_TILE_SIZE, MAP_TILE_SIZE, icon, 19);
  }
  else {
    EsploraTFT.fillRect(x, y, MAP_TILE_SIZE, MAP_TILE_SIZE, colorOf(19));
  }

  EsploraTFT.drawRect(x, y, MAP_TILE_SIZE, MAP_TILE_SIZE,               // einen Rahmen darueber zeichnen
                      colorOf(icon != NULL ? 12 : 18));
}

// ========================================================================================
// Kaufwert eines Items (0 = kann nicht gekauft werden)
uint16_t getItemBuyValue(uint16_t itemId) {

  switch(itemId) {
    case(ITEM_CAMERA):      { return pgm_read_word(&mItemCameraBuyValue); }
    case(ITEM_BOAT_TICKET): { return BOAT_TICKET_PRICE; }
    case(ITEM_BUS_TICKET):  { return BUS_TICKET_PRICE; }
    default:                { return 0; }
  }
}

// ========================================================================================
// Verkaufswert eines Items (0 = kann nicht verkauft werden)
uint16_t getItemSellValue(uint16_t itemId) {
  if(itemId == ITEM_CAMERA) { return pgm_read_word(&mItemCameraSellValue); }
  return 0;
}

// ========================================================================================
// Hat die Figur alle drei neuen Fotos (Bruecke, Insel, Stadt).
bool hasAllNewPhotos() {
  return hasItem(ITEM_PHOTO_BRIDGE) && hasItem(ITEM_PHOTO_ISLAND) && hasItem(ITEM_PHOTO_CITY);
}
