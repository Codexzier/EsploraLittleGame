// ========================================================================================
// Description:       Spielstand im EEPROM speichern und laden.
//                    Gespeichert wird automatisch bei jedem Kartenwechsel und nach
//                    jedem geschlossenen Fenster. eeprom_update_... schreibt nur Bytes,
//                    die sich geaendert haben, das schont den EEPROM.
// ----------------------------------------------------------------------------------------
// Aufbau:            Adresse 0     Kennung (SAVE_MAGIC), nur dann gibt es einen Spielstand
//                    ab Adresse 1  Karte, Position, Blickrichtung, Spielfortschritt,
//                                  Muenzen, Rucksack und verwendete Kacheln (ca. 90 Byte)
// ========================================================================================

// ========================================================================================
// Variablen

#define SAVE_ADDRESS   0                                               // Beginn des Spielstandes im EEPROM
#define SAVE_MAGIC     0x5A                                            // Kennung, bei Aenderung des Aufbaus erhoehen

uint16_t mSaveAddress = 0;                                             // aktuelle Schreib- / Leseadresse

// ========================================================================================
// Methoden
// ========================================================================================

// ========================================================================================
// Gibt es einen gespeicherten Spielstand.
bool hasSaveGame() {
  return eeprom_read_byte((const uint8_t*)SAVE_ADDRESS) == SAVE_MAGIC;
}

// ========================================================================================
// Speichert den aktuellen Spielstand.
void saveGame() {

  if(mScene != SCENE_MAP) {                                            // nicht waehrend einer Animation
    return;
  }

  eeprom_update_byte((uint8_t*)SAVE_ADDRESS, SAVE_MAGIC);
  transferGame(true);
}

// ========================================================================================
// Laedt den gespeicherten Spielstand und zeichnet alles neu.
void loadGame() {

  transferGame(false);

  mWindowType = WIN_NONE;
  mWindowFollowType = WIN_NONE;
  mMapNeedsReload = false;
  mIsWalking = false;
  figureStand();

  loadMap(mCurrentMap, mPosX, mPosY);
  drawHud();
}

// ========================================================================================
// Schreibt oder liest alle Werte des Spielstandes in fester Reihenfolge.
// ----------------------------------------------------------------------------------------
// write = true: speichern, false: laden
void transferGame(bool write) {

  mSaveAddress = SAVE_ADDRESS + 1;
  transferBlock(&mCurrentMap, sizeof(mCurrentMap), write);
  transferBlock(&mPosX, sizeof(mPosX), write);
  transferBlock(&mPosY, sizeof(mPosY), write);
  transferBlock(&mFacingX, sizeof(mFacingX), write);
  transferBlock(&mFacingY, sizeof(mFacingY), write);
  transferBlock(&mQuestState, sizeof(mQuestState), write);
  transferBlock(&mCoins, sizeof(mCoins), write);
  transferBlock(mBackPlaces, sizeof(mBackPlaces), write);
  transferBlock(mTileConsumed, sizeof(mTileConsumed), write);
}

// ========================================================================================
// Schreibt oder liest einen Speicherbereich an der aktuellen Adresse.
void transferBlock(void* data, byte size, bool write) {

  if(write) {
    eeprom_update_block(data, (void*)(uintptr_t)mSaveAddress, size);
  }
  else {
    eeprom_read_block(data, (const void*)(uintptr_t)mSaveAddress, size);
  }

  mSaveAddress += size;
}
