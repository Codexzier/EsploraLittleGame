// ========================================================================================
// Description:       Grundeinstellung des Rucksackes
// ========================================================================================

// ========================================================================================
// Functionsvariablen

#define BACKPACK_PLACES_COUNT 6
uint16_t mBackPlaces[6] = { 0, 0, 0, 0, 0, 0 };
byte mTempIcon[256];

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

const PROGMEM char mItemKey01[10] = "Schluessel";                        // Name 
const PROGMEM byte mItemKey01Icon[256] =  {                              // Icon / Bild
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,10,0,0,10,10,10,10,10,10,10,10,10,10,10,0,0,10,0,0,10,10,0,10,0,0,0,0,10,0,0,0,0,10,0,0,10,0,0,0,0,0,0,0,10,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
};
const PROGMEM char mItemKey01Description[] = "Oeffnet eine Box";         // Beschreibung
const PROGMEM uint16_t mItemKey01Usage = 1;                              // Verwendungszweck Id  => kombinierte funktions abruf fur position und verknuepfte Box mit der selben Id
                                                                        // Verkaufswert         = 0 (Kann nicht verkauft werden)
                                                                        // Kaufwert             = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)

// ========================================================================================
// ID 02 

const PROGMEM char mItemCamera[6] = "Kamera";                            // Name
const PROGMEM byte mItemCameraIcon[256] = {                              // Icon / Bild
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,9,9,9,9,9,9,1,0,0,0,0,0,1,1,1,1,9,19,19,19,19,9,1,1,1,1,0,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,19,19,9,1,1,9,9,9,9,1,11,11,11,11,1,9,19,19,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,1,11,11,11,11,11,11,1,9,9,9,1,1,9,9,9,9,1,11,11,11,11,1,9,9,9,9,1,1,9,9,9,9,1,1,11,11,1,1,9,9,9,9,1,1,9,9,9,9,9,9,1,1,9,9,9,9,9,9,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
};
const PROGMEM char mItemCameraDescription[20] = "Mach ein paar Fotos!";  // Beschreibung
const PROGMEM uint16_t mItemCameraUsage = 2;                             // Verwendungszweck
const PROGMEM uint16_t mItemCameraSellValue = 140;                       // Verkaufswert
const PROGMEM uint16_t mItemCameraBuyValue = 200;                        // Kaufwert

// ========================================================================================
// ID 03 

const PROGMEM char mItemPhoto01[7] = "Foto 01";                          // Name
const PROGMEM byte mItemPhoto01Icon[256] = {                             // Icon / Bild
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,18,18,18,19,19,19,19,19,12,12,12,19,1,1,19,18,18,18,18,18,19,18,19,12,12,12,12,12,1,1,19,19,18,18,19,19,18,19,19,19,12,12,12,19,1,1,19,19,19,19,19,19,19,19,19,19,19,12,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,19,19,19,19,19,19,19,19,19,19,19,19,1,1,19,19,6,19,19,19,19,19,19,19,17,19,19,19,1,1,19,6,6,6,19,19,19,19,19,17,17,17,19,19,1,1,19,6,6,6,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,11,17,3,17,19,1,1,19,19,3,19,19,19,19,19,17,17,17,3,17,19,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 
};
const PROGMEM char mItemPhoto01Description[20] = "Sonne, Baum und Haus"; // Beschreibung
const PROGMEM uint16_t mItemPhoto01Usage = 3;                            // Verwendungszweck
                                                                        // Verkaufswert = 0 (Kann nicht verkauft werden, Objekt wird für Aufgabe abgegeben)
                                                                        // Kaufwert     = 0 (Kann nicht erwaorben werden, Objekte wird gefunden oder vergeben)

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Kopiert das array aus dem flash in das temporaere array
// ----------------------------------------------------------------------------------------
// icon[] = byte array mit dem Bild dass aus dem Flash Speicher geladen werden soll.
void setItemIconToTemp(byte icon[]) {
  // TODO: Bild kann eigentlich wie die Karte direkt aus dem Speicher
  //        auf den Bildschirm geschrieben werden.
  
  for(int index = 0; index < 256; index++) {
    mTempIcon[index] = pgm_read_byte_near(icon + index);
  }
}

// ========================================================================================
// Pruefen ob das Item bereits vorhanden ist ein Item kann nur einmal vorhanden sein
// ----------------------------------------------------------------------------------------
// itemId = Id Nummer, dass in in einem Taschenplatz hinterlegt wurde.
boolean isItemInBackback(uint16_t itemId) {

  for(byte index = 0; index < 6; index++) {
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
bool setItemToBackpack(uint16_t itemId) {

  if(isItemInBackback(itemId)) {
    return false; 
  }

  byte place = 0;                                                       // id ablegen in ersten freien Taschenplatz
  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {
    if(mBackPlaces[index] == 0) {
      mBackPlaces[index] = itemId;
      place = index;
      break;
    }
  }
  
  byte relationPlaceX = 0;
  byte relationPlaceY = 0;
  setItemRelationPlace(place, &relationPlaceX, &relationPlaceY);

  boolean isArrayCopy = true;                                           // koennte vielleicht spaeter ausgelagert werden.
                                                                        // abruf des Icon zu dem Item
  switch(itemId) {
    case(1): { setItemIconToTemp(mItemKey01Icon);  break; }              // Schluessel
    case(2): { setItemIconToTemp(mItemCameraIcon); break; }              // Fotoapparat
    case(3): { setItemIconToTemp(mItemPhoto01Icon); break; }             // Foto
    default: { isArrayCopy = false; break; }                            // Nicht belegt, darf aber auch nciht eintreten
  }

  if(isArrayCopy) { 
    drawTile(relationPlaceX, relationPlaceY, 
             mMapTileSize, mMapTileSize, 
             mTempIcon, false); 
  }
  else { 
    EsploraTFT.fillRect(relationPlaceX, relationPlaceY, 
                        mMapTileSize, mMapTileSize, 
                        0xF800); 
  }
  
  EsploraTFT.drawRect(relationPlaceX, relationPlaceY,                   // einen Rahmen darueber zeichnen
                      mMapTileSize, mMapTileSize, 
                      mapNumberToColor(12));

  return true;
}

// ========================================================================================
void drawEmptyPlaces() {

  for(int index = 0; index < BACKPACK_PLACES_COUNT; index++) {
    if(mBackPlaces[index] == 0) {  
      byte relationPlaceX = 0;
      byte relationPlaceY = 0;
      setItemRelationPlace(index, &relationPlaceX, &relationPlaceY);
      EsploraTFT.fillRect(relationPlaceX, relationPlaceY, mMapTileSize, mMapTileSize, mapNumberToColor(19));
      EsploraTFT.drawRect(relationPlaceX, relationPlaceY, mMapTileSize, mMapTileSize, mapNumberToColor(18));
    }
  }
}

// ========================================================================================
// holt die anfangs Position des Taschenplatzes 
// das auf dem Bildschirm gerendert werden soll.
// ----------------------------------------------------------------------------------------
// place          = Taschenplatz
// relationPlaceX = Erste bis dritte beginnende Spalte.
// relationPlaceY = Erste oder zweite Reihe.
void setItemRelationPlace(byte place, byte* relationPlaceX, byte* relationPlaceY) {

  if(place == 0 || place == 1 || place == 2) {                          // erste Zeile
    *relationPlaceY = 96; 
  }
  
  else if(place == 3 || place == 4 || place == 5) {                     // zweite Zeile
    *relationPlaceY = 112; 
  }

  if(place == 0 || place == 3) {                                        // erste Spalte
    *relationPlaceX = 0; 
  }
  
  else if(place == 1 || place == 4) {                                   // zweite Spalte 
    *relationPlaceX = 16; 
  }
  
  else if(place == 2 || place == 5) {                                   // dritte Spalte
    *relationPlaceX = 32; 
  }
}

// ========================================================================================
// Ruft das Item per verwendeten ID auf der Karte
// ----------------------------------------------------------------------------------------
// mapUsageId = ist die Id nummer, die in der Karte eingesetz wurde.
//              mit der ID wird der Verwendungszweck abgerufen und mit den Items aus der Tasche abgeglichen
//              die ID ist Kartenspezifisch, daher werden auch die Karten Informationen benotigt.
bool getItemToUsed(int16_t mapUsageId) {

  // TODO: Karten Spezifische Eigenschaften.
  //        Die Werte muessen spaeter in einem temporaren speicher gehalten werden
  int16_t itemId = 0;
  
  // TODO: Spezifischer Karteninformation
  // hole itemId aus der Karteneigenschaft ab.
  if(mapUsageId == mMapBarrierUsageDoor01) {
    itemId = 1;                                                         // Id des zu verwendenden Schlussels
  }

  for(byte index = 0; index < BACKPACK_PLACES_COUNT; index++) {           // prüfe die Tasche, ob das Item vorhanden ist
                                                                        // und dann aus dem inventar nehmen
    // TODO: Spezifische Funktion. 
    //       Item einmalig verwenden
    if(itemId != 0 && mBackPlaces[index] == itemId) {
      
      // TODO Spezfische Karteninfo 
      // Verwendungszweck
      if(mapUsageId == mMapBarrierUsageDoor01) {
        mMapBarrierDoorIsOpen = true;
        mBackPlaces[index] = 0;                                          // aus dem Inventar entfernen
      }
    }
  }

  // TODO: Karten Spezifische Eigenschaften.
  //        Die Werte muessen spaeter in einem temporaren speicher gehalten werden
  if(mapUsageId == mMapBarrierUsageDoor01 && mMapBarrierDoorIsOpen == true) {
    return true;
  }

   return false;
}