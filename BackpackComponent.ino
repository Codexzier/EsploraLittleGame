// ========================================================================================
// Description:       Grundeinstellung des Rucksackes
//                    Der Rucksack hat 6 Taschenplaetze (unten links auf dem Bildschirm).
// ========================================================================================

// ========================================================================================
// Objeckte
// Name (sollte sich auf moeglich wenig Zeichen beschraenken)
// Bild (byte array)
// Beschreibung (nur bedingt verwenden)
// Verwendungszweck (sollte nur eine ID sein)
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
const PROGMEM byte mItemKey01Icon[256] = {                              // Icon / Bild
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,0,0,1,12,12,12,12,1,0,
  0,1,1,1,1,1,1,1,1,12,1,0,0,1,12,1,
  1,12,12,12,12,12,12,12,12,12,1,0,0,1,12,1,
  0,1,1,1,1,12,1,12,1,12,1,0,0,1,12,1,
  0,0,0,0,1,12,1,12,1,1,12,12,12,12,1,0,
  0,0,0,0,1,1,1,1,1,0,1,1,1,1,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
const PROGMEM char mItemKey01Description[] = "Oeffnet eine Tuer";      // Beschreibung
const PROGMEM uint16_t mItemKey01Usage = 1;                              // Verwendungszweck Id  => kombinierte funktions abruf fur position und verknuepfte Tuer mit der selben Id
                                                                        // Verkaufswert         = 0 (Kann nicht verkauft werden)
                                                                        // Kaufwert             = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)

// ========================================================================================
// ID 02 

const PROGMEM char mItemCamera[] = "Kamera";                            // Name
const PROGMEM byte mItemCameraIcon[256] = {                              // Icon / Bild
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,9,9,9,9,9,9,1,0,0,0,0,0,1,1,1,1,9,19,19,19,19,9,1,1,1,1,0,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,19,19,9,1,1,9,9,9,9,1,11,11,11,11,1,9,19,19,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,9,1,11,11,11,11,1,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
};
const PROGMEM char mItemCameraDescription[] = "Mach ein paar Fotos!";  // Beschreibung
const PROGMEM uint16_t mItemCameraUsage = 2;                             // Verwendungszweck
const PROGMEM uint16_t mItemCameraSellValue = 140;                       // Verkaufswert
const PROGMEM uint16_t mItemCameraBuyValue = 200;                        // Kaufwert

// ========================================================================================
// ID 03 

const PROGMEM char mItemPhoto01[] = "Foto 01";                          // Name
const PROGMEM byte mItemPhoto01Icon[256] = {                             // Icon / Bild
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,18,18,18,19,19,19,19,19,12,12,12,19,1,1,19,18,18,18,18,18,19,18,19,12,12,12,12,12,1,1,19,19,18,18,19,19,18,19,19,19,12,12,12,19,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,6,19,19,19,19,19,19,19,17,19,19,19,1,1,19,6,6,6,19,19,19,19,19,17,17,17,19,19,1,1,19,6,6,6,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,17,17,3,17,19,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 
};
const PROGMEM char mItemPhoto01Description[] = "Sonne, Baum und Haus"; // Beschreibung
const PROGMEM uint16_t mItemPhoto01Usage = 3;                            // Verwendungszweck
                                                                        // Verkaufswert = 0 (Kann nicht verkauft werden, Objekt wird für Aufgabe abgegeben)
                                                                        // Kaufwert     = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)


// ========================================================================================
// Muenze (wird im Rucksack Bereich und auf der Karte verwendet)

const PROGMEM byte mCoinSpiteIcon[49] = { 
  0,14,12,12,12,5,0,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,14,12,12,12,12,12,5,0,14,12,12,12,5,0 
};

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
// Zeichnet einen Taschenplatz mit dem Icon des Items oder leer.
// ----------------------------------------------------------------------------------------
// place = Taschenplatz 0 bis 5
void drawBackpackPlace(byte place) {

  int x = (place % 3) * MAP_TILE_SIZE;                                  // drei Spalten
  int y = HUD_POS_Y + (place / 3) * MAP_TILE_SIZE;                      // zwei Zeilen

  const byte* icon = NULL;                                              // abruf des Icon zu dem Item
  switch(mBackPlaces[place]) {
    case(ITEM_KEY):    { icon = mItemKey01Icon;   break; }             // Schluessel
    case(ITEM_CAMERA): { icon = mItemCameraIcon;  break; }             // Fotoapparat
    case(ITEM_PHOTO):  { icon = mItemPhoto01Icon; break; }             // Foto
    default:           { break; }                                       // Nicht belegt
  }

  if(icon != NULL) {
    drawIcon(x, y, MAP_TILE_SIZE, MAP_TILE_SIZE, icon, 19);
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
  if(itemId == ITEM_CAMERA) { return pgm_read_word(&mItemCameraBuyValue); }
  return 0;
}

// ========================================================================================
// Verkaufswert eines Items (0 = kann nicht verkauft werden)
uint16_t getItemSellValue(uint16_t itemId) {
  if(itemId == ITEM_CAMERA) { return pgm_read_word(&mItemCameraSellValue); }
  return 0;
}
