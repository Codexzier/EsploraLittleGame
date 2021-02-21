// Legt ein Fenster in den Vordergrund
bool lastStateShowWindow = false;
bool windowHasRendered = false;

// Zeichnet das Fenster mit einem Ensprechenden Text
// rightSide = soll das Fenster auf der Rechten Seite angezeigt werden (Standard Maessig wird es in der Mitte angezeigt)
void drawWindow(bool rightSide) {

  if(lastStateShowWindow != showWindow && !showWindow) {
    lastStateShowWindow = showWindow;
    drawStack(true);
  }

  lastStateShowWindow = showWindow;
  
  if(!showWindow) {
    windowHasRendered = false;
    return;
  }

  if(windowHasRendered) {
    return;
  }
  
  // Mitte des Bildschirm schreiben
  int sizeX = 100; int sizeY = 40;
  int winPosX = (EsploraTFT.width() / 2) - (sizeX / 2);
  int winPosY = (EsploraTFT.height() / 2) - (sizeY / 2);

  EsploraTFT.fillRect(winPosX, winPosY, sizeX, sizeY, mapNumberToColor(0));
  EsploraTFT.drawRect(winPosX, winPosY, sizeX, sizeY, mapNumberToColor(18));
  EsploraTFT.drawRect(winPosX + 2, winPosY + 2, sizeX - 4, sizeY - 4, mapNumberToColor(18));

  // Text schreiben
  writeText(winPosX + 5, winPosY + 5, "Hallo!");
  writeText(winPosX + 5, winPosY + 28, "Schliessen [2]");

   windowHasRendered = true;
}
