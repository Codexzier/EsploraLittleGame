# Anforderungsbeschreibung: „Suries Foto“

Ausbau des kleinen Adventure-Spiels für den Arduino Esplora mit TFT-Display (160×128). Grundlage sind die vorhandenen Spielmechaniken: Laufen, Kollision, Gegenstände aufheben, Rucksack, Tür mit Schlüssel, Händlerin, Kiste, Münzen und Dialogfenster.

## 1. Rahmenbedingungen

| ID | Anforderung |
|---|---|
| R-01 | Zielhardware ist der Arduino Esplora (ATmega32U4, 28 KB Flash für den Sketch, 2,5 KB SRAM) mit dem Arduino TFT-Display 1.8". |
| R-02 | Verwendet werden nur die Bibliotheken `SPI`, `TFT` und `Esplora`. |
| R-03 | Der Sketch belegt höchstens 90 % des Flash-Speichers und höchstens 50 % des SRAM für globale Variablen. *(Stand: 75 % Flash, 14 % SRAM)* |
| R-04 | Grafiken, Karten und Texte liegen im Flash (`PROGMEM`). Im SRAM gibt es keine Bildpuffer. |

## 2. Spielidee

Die Händlerin Surie wünscht sich ein Foto von „Baum und Haus im Sonnenschein“. Die Spielfigur findet den Schlüssel, öffnet die Tür und trifft Surie. Dann sammelt sie Münzen, kauft eine Kamera, fotografiert im Garten das Motiv und bringt Surie das Foto.

## 3. Spielablauf

| Schritt | Ziel (Anzeige unten rechts) | Auslöser | Ergebnis |
|---|---|---|---|
| 1 | „Finde den Schlüssel.“ | Kachel mit dem Schlüssel betreten | Schlüssel im Rucksack, Hinweisfenster |
| 2 | „Öffne die Tür.“ | Gegen die Tür laufen | Schlüssel wird verbraucht, die Tür ist dauerhaft offen |
| 3 | „Sprich mit Surie.“ | Gegen Surie laufen | Auftrag erhalten |
| 4 | „Sammle 200 Münzen für die Kamera.“ | Kiste öffnen, Münzen im Garten einsammeln | 25 (Start) + 75 (Kiste) + 4 × 25 (Garten) = 200 |
| 5 | „Kaufe die Kamera bei Surie.“ | Surie ansprechen, „Kaufe Kamera (200)“ | Kamera im Rucksack, 0 Münzen |
| 6 | „Fotografiere Baum und Haus (Garten).“ | Fotopunkt (gelbes X) betreten, „Foto machen“ | Foto im Rucksack |
| 7 | „Bring Surie das Foto.“ | Surie ansprechen, „Gib Surie das Foto“ | +150 Münzen, Abschlussfenster |
| – | „Geschafft! [1] halten = Neu“ | Button 1 eine Sekunde halten | Neues Spiel |

**Sackgassen vermeiden:** Die Kamera kann erst nach abgeschlossenem Auftrag verkauft werden. Sonst könnte man sie für 140 verkaufen und bekäme nie wieder 200 Münzen zusammen.

## 4. Funktionale Anforderungen

### Steuerung

| ID | Anforderung |
|---|---|
| F-01 | Der Joystick bewegt die Figur in vier Richtungen. Bei schrägem Ausschlag zählt die Achse mit dem größeren Ausschlag. Eine Totzone (`STICK_DEADBAND`) filtert das Rauschen. |
| F-02 | Die Figur läuft mit gleichmäßiger Geschwindigkeit (1 Pixel alle 16 ms), unabhängig davon, wie lange das Zeichnen dauert. |
| F-03 | Button 4 (rechts) bestätigt, Button 2 (links) schließt oder geht zurück. In Fenstern wechselt der Joystick hoch/runter die Auswahl mit Wiederholverzögerung. |
| F-04 | Wird Button 1 (unten) eine Sekunde gehalten, startet das Spiel neu. Kurzes Drücken bewirkt nichts. |

### Bewegung und Kollision

| ID | Anforderung |
|---|---|
| F-10 | Kollisionen werden über den Fuß-Bereich der Figur (8×6 Pixel) geprüft. Der Kopf darf optisch vor Wänden stehen. |
| F-11 | Blockierende Kacheln sind Wand, Tür (solange geschlossen), Kiste, Baum, Hauswand, Dach, Hecke und Haustür. |
| F-12 | Anstoßen an Tür, Kiste, Haustür oder Surie löst genau **eine** Aktion aus. Erst nach einer Bewegung kann erneut ausgelöst werden. |
| F-13 | Das Betreten von Schlüssel, Münze, Ausgang oder Fotopunkt löst genau **einmal** aus. Maßgeblich ist die Mitte der Füße. |

### Karten

| ID | Anforderung |
|---|---|
| F-20 | Es gibt zwei Karten mit je 10×6 Kacheln: **Haus** (zwei Räume, Kiste, Tür, Surie) und **Garten** (Baum, Haus, Weg, Blumen, 4 Münzen, Fotopunkt). |
| F-21 | Ausgänge (Fußmatte) verbinden die Karten. Die Figur erscheint auf der Zielkarte neben dem Gegen-Ausgang. |
| F-22 | Aufgesammelte Gegenstände, geöffnete Türen und die geleerte Kiste bleiben auch nach einem Kartenwechsel erhalten (Bitfeld `mTileConsumed` je Karte). |

### Gegenstände und Rucksack

| ID | Anforderung |
|---|---|
| F-30 | Der Rucksack hat 6 Plätze. Belegte Plätze zeigen das Icon mit gelbem Rahmen, freie Plätze sind hellgrau mit grauem Rahmen. |
| F-31 | Jeder Gegenstand kann nur einmal im Rucksack sein. Beim Entfernen wird der Platz sofort neu gezeichnet. |
| F-32 | Gegenstände: Schlüssel (öffnet die Tür, wird verbraucht), Kamera (Kauf 200, Verkauf 140), Foto „Sonne, Baum und Haus“ (Abgabe an Surie). |

### Figuren und Dialoge

| ID | Anforderung |
|---|---|
| F-40 | Beim ersten Gespräch erklärt Surie den Auftrag. Danach öffnet sie ein Handelsmenü, dessen Optionen vom Spielstand abhängen: Foto geben, Kamera kaufen, Kamera verkaufen (nur nach dem Auftrag), Tschüss. |
| F-41 | Bei zu wenig Münzen erscheint „Du hast nicht genug Münzen.“ |
| F-42 | Die Kiste fragt „Öffnen / Zu lassen“. Beim Öffnen gibt es 75 Münzen, danach ist sie leer. |
| F-43 | Das Dialogfenster hat einen Titel (gelb), einen Text mit Zeilenumbruch an Wortgrenzen (23 Zeichen je Zeile), bis zu 4 Auswahlpunkte und eine Fußzeile mit der Tastenbelegung. |
| F-44 | Nach dem Schließen wird die Karte unter dem Fenster neu gezeichnet, Figur und Surie inklusive. |

### Anzeige (HUD)

| ID | Anforderung |
|---|---|
| F-50 | Der untere Bereich (y 96–127) ist fest reserviert: Rucksack links, Münzstand mit Icon oben rechts, aktuelles Ziel (2 Zeilen à 18 Zeichen) unten rechts. |
| F-51 | Münzstand und Ziel werden nur bei einer Änderung neu gezeichnet. |

## 5. Grafik-Anforderungen

| ID | Anforderung |
|---|---|
| G-01 | **Kein Flackern:** Jeder Bildpunkt wird pro Aktualisierung genau einmal geschrieben (Composite-Rendering). Es wird nicht erst gelöscht und dann neu gezeichnet. |
| G-02 | **Tiefensortierung:** Die Figur, die weiter unten steht (größeres y), wird vor der anderen gezeichnet. |
| G-03 | Pro Bewegungsschritt wird nur das Rechteck aus alter und neuer Position übertragen (höchstens 11×17 Pixel). |
| G-04 | Kacheln haben Texturen: Wand, Boden, Gras, Weg, Hecke, Dach und Blumen als 8×8-Muster; Tür, Kiste, Ausgang, Baum, Hauswand, Haustür, Schlüssel, Münze und Fotopunkt als 16×16-Bilder. Transparente Pixel zeigen den Boden der Karte. |
| G-05 | Alle Farben stammen aus einer zentralen Palette (`mPalette`). Die Nummern 100–105 sind die veränderbaren Farben einer Figur (Haare, Shirt, Hose). |
| G-06 | Texturen werden in `tools/sprites.py` als lesbare Zeichengrafik gepflegt und daraus als Byte-Arrays erzeugt. |

## 6. Nicht-funktionale Anforderungen

| ID | Anforderung |
|---|---|
| N-01 | Jede Komponente liegt in einer eigenen Datei: Backpack, Collision, Figure, Map, Quest, Render, Trader, Window. Gemeinsame Konstanten und Zustände stehen in `EsploraLittleGame.ino`, weil die IDE die Dateien alphabetisch anhängt. |
| N-02 | Kommentarstil und Benennung (`m`-Präfix, deutsche Kommentare, Trennlinien) bleiben wie im bestehenden Code. |
| N-03 | Das Spiel lässt sich ohne Hardware testen: `tools/simulator` baut den Sketch für den PC, spielt ihn automatisch durch, prüft den Spielstand und speichert Bildschirmfotos. |

## 7. Abnahmekriterien

Alle Punkte prüft der Simulator (`python3 tools/simulator/build.py <TFT-Bibliothek>`):

1. Der Start im Haus zeigt Figur, Surie, Schlüssel, Kiste, Tür und Ausgang. Anzeige: 25 Münzen, Ziel „Finde den Schlüssel.“
2. Die Tür ohne Schlüssel bleibt zu und zeigt einen Hinweis.
3. Der Schlüssel wird genau einmal aufgenommen.
4. Die Kiste gibt einmal 75 Münzen und ist danach leer.
5. Die Tür öffnet sich mit dem Schlüssel, der Schlüssel verschwindet aus dem Rucksack.
6. Surie gibt den Auftrag. Beim Kaufversuch mit 100 Münzen erscheint „nicht genug Münzen“.
7. Der Kartenwechsel in den Garten und zurück funktioniert, die 4 Münzen ergeben 200.
8. Ohne Kamera zeigt der Fotopunkt nur einen Hinweis. Mit Kamera entsteht das Foto.
9. Die Auswahl im Menü lässt sich mit dem Joystick bewegen.
10. Die Abgabe des Fotos gibt 150 Münzen und zeigt das Abschlussfenster.
11. Kurzes Drücken von Button 1 setzt nicht zurück, eine Sekunde Halten setzt alles zurück.

## 8. Mögliche nächste Ausbaustufen

- **Rucksack benutzen:** Mit Button 3 in den Rucksack wechseln, mit dem Joystick einen Platz wählen und die Beschreibung anzeigen. Die Texte (`mItem…Description`) sind schon vorhanden.
- **Zweite Figur:** Das männliche Händler-Sprite `mTraderSpriteFrontMen` ist vorhanden, zum Beispiel ein Bewohner im Gartenhaus, der eine eigene Aufgabe hat.
- **Weitere Karten** über zusätzliche Einträge in `mMapContent` und `mMapExits`.
- **Rückmeldung über die Esplora-Hardware:** Mit `Esplora.tone()` ein Ton beim Aufheben, mit `Esplora.writeRGB()` ein Aufleuchten der LED beim Foto.
- **Spielstand speichern** im EEPROM (Bitfelder, Rucksack, Münzen: unter 30 Byte).
