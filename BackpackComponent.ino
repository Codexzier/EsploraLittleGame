// Grundeinstellung des Rucksackes
#define backpackPlacesCount 6
uint16_t backPlaces[backpackPlacesCount] = { 0, 0, 0, 0, 0, 0 };
byte tempIcon[256];

// Objeckte
// Name (sollte sich auf moeglich wenig Zeichen beschraenken)
// Bild (byte array)
// Beschreibung (nur bedingt verwenden)
// Verwendungszweck (sollte nur eine ID sein)
// Verkaufswert (einige Dinge können gehandelt werden)
// Kaufwert (Haendler Preis)

// #######################################
// #          ITEMS                      #
// #######################################
// ID 0 
// '0' bedeutet immer nicht belegt.
// #######################################
// ID 01
// Name 
const PROGMEM char itemKey01[10] = "Schluessel";
// Icon / Bild
const PROGMEM byte itemKey01Icon[256] =  { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,10,0,0,10,10,10,10,10,10,10,10,10,10,10,0,0,10,0,0,10,10,0,10,0,0,0,0,10,0,0,0,0,10,0,0,10,0,0,0,0,0,0,0,10,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
// Beschreibung
const PROGMEM char itemKey01Description[] = "Oeffnet eine Box";
// Verwendungszweck Id => kombinierte funktions abruf fur position und verknuepfte Box mit der selben Id
const PROGMEM uint16_t itemKey01Usage = 1;
// Verkaufswert = 0 (Kann nicht verkauft werden)
// Kaufwert     = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)

// #######################################
// ID 02 
// Name
const PROGMEM char itemCamera[6] = "Kamera";
// Icon / Bild
const PROGMEM byte itemCameraIcon[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,9,9,9,9,9,9,1,0,0,0,0,0,1,1,1,1,9,19,19,19,19,9,1,1,1,1,0,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,19,19,9,1,1,9,9,9,9,1,11,11,11,11,1,9,19,19,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,9,1,11,11,11,11,1,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
// Beschreibung
const PROGMEM char itemCameraDescription[20] = "Mach ein paar Fotos!";
// Verwendungszweck
const PROGMEM uint16_t itemCameraUsage = 2;

// #######################################
// ID 03 
// Name
const PROGMEM char itemPhoto01[7] = "Foto 01";
// Icon / Bild
const PROGMEM byte itemPhoto01Icon[256] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,18,18,18,19,19,19,19,19,12,12,12,19,1,1,19,18,18,18,18,18,19,18,19,12,12,12,12,12,1,1,19,19,18,18,19,19,18,19,19,19,12,12,12,19,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,6,19,19,19,19,19,19,19,17,19,19,19,1,1,19,6,6,6,19,19,19,19,19,17,17,17,19,19,1,1,19,6,6,6,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,17,17,3,17,19,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
// Beschreibunbg
const PROGMEM char itemPhoto01Description[20] = "Sonne, Baum und Haus";
// Verwendungszweck
const PROGMEM uint16_t itemPhoto01Usage = 3;


// #######################################
// #         ITEM Methods                #
// #######################################

// Kopiert das array aus dem flash in das temporaere array
void setItemIconToTemp(byte icon[]) {
  for(int index = 0; index < 256; index++) {
    tempIcon[index] = pgm_read_byte_near(icon + index);
  }
}

// pruefen ob das Item bereits vorhanden ist
// ein Item kann nur einmal vorhanden sein
bool isItemInBackback(uint16_t itemId) {

  for(byte index = 0; index < 6; index++) {
    if(backPlaces[index] == itemId) { return true; }
  }

  return false;
}

// legt das Item in die Tasche ab und Zeichnet es in einen offen Taschenplatz
bool setItemToBackpack(uint16_t itemId) {

  if(isItemInBackback(itemId)) { return false; }
  
  // id ablegen in ersten freien Taschenplatz
  byte place = 0;
  for(byte index = 0; index < backpackPlacesCount; index++) {
    if(backPlaces[index] == 0) {
      backPlaces[index] = itemId;
      place = index;
      break;
    }
  }
  
  byte relationPlaceX = 0;
  byte relationPlaceY = 0;
  setItemRelationPlace(place, &relationPlaceX, &relationPlaceY);

  // koennte vielleicht spaeter ausgelagert werden.
  // abruf des Icon zu dem Item
  boolean isArrayCopy = true;
  switch(itemId) {
    case(1): { setItemIconToTemp(itemKey01Icon);  break; } // Schluessel
    case(2): { setItemIconToTemp(itemCameraIcon); break; } // Fotoapparat
    case(3): { setItemIconToTemp(itemPhoto01Icon); break; } // Foto
    default: { isArrayCopy = false; break; } // Nicht belegt, darf aber auch nciht eintreten
  }

  if(isArrayCopy) { drawTile(relationPlaceX, relationPlaceY, mapTileSize, mapTileSize, tempIcon, false); }
  else { EsploraTFT.fillRect(relationPlaceX, relationPlaceY, mapTileSize, mapTileSize, 0xF800); }
  
  // einen Rahmen darueber zeichnen
  EsploraTFT.drawRect(relationPlaceX, relationPlaceY, mapTileSize, mapTileSize, mapNumberToColor(12));

  return true;
}

void drawEmptyPlaces() {

  for(int index = 0; index < backpackPlacesCount; index++) {
    if(backPlaces[index] == 0) {  
      byte relationPlaceX = 0;
      byte relationPlaceY = 0;
      setItemRelationPlace(index, &relationPlaceX, &relationPlaceY);
      EsploraTFT.fillRect(relationPlaceX, relationPlaceY, mapTileSize, mapTileSize, mapNumberToColor(19));
      EsploraTFT.drawRect(relationPlaceX, relationPlaceY, mapTileSize, mapTileSize, mapNumberToColor(18));
    }
  }
}

// holt die anfangs Position des Taschenplatzes das auf dem Bildschirm gerendert werden soll.
void setItemRelationPlace(byte place, byte* relationPlaceX, byte* relationPlaceY) {

  // erste Zeile
  if(place == 0 || place == 1 || place == 2) { *relationPlaceY = 96; }
  // zweite Zeile
  else if(place == 3 || place == 4 || place == 5) { *relationPlaceY = 112; }

  // erste Spalte
  if(place == 0 || place == 3) { *relationPlaceX = 0; }
  // zweite Spalte
  else if(place == 1 || place == 4) { *relationPlaceX = 16; }
  // dritte Spalte
  else if(place == 2 || place == 5) { *relationPlaceX = 32; }
}

// mapUsageId = ist die Id nummer, die in der Karte eingesetz wurde.
//              mit der ID wird der Verwendungszweck abgerufen und mit den Items aus der Tasche abgeglichen
//              die ID ist Kartenspezifisch, daher werden auch die Karten Informationen benotigt.
bool getItemToUsed(int16_t mapUsageId) {

  // TODO: Karten Spezifische Eigenschaften.
  //        Die Werte muessen spaeter in einem temporaren speicher gehalten werden
  int16_t itemId = 0;

  // hole itemId aus der Karteneigenschaft ab.
  if(mapUsageId == mapBarrierUsageDoor01) {
    // Id des zu verwendenden Schlussels
    itemId = 1;
  }
  
  // prüfe die Tasche, ob das Item vorhanden ist
  // und dann aus dem inventar nehmen
  for(byte index = 0; index < backpackPlacesCount; index++) {

    // Item einmalig verwenden
    if(itemId != 0 && backPlaces[index] == itemId) {

      // Verwendungszweck
      if(mapUsageId == mapBarrierUsageDoor01) {

        mapBarrierDoorIsOpen = true;
        // aus dem Inventar entfernen
        backPlaces[index] = 0;
      }
    }
  }
  
  // TODO: Karten Spezifische Eigenschaften.
  //        Die Werte muessen spaeter in einem temporaren speicher gehalten werden
  if(mapUsageId == mapBarrierUsageDoor01 && mapBarrierDoorIsOpen == true) {
    return true;
  }

  return false;
}