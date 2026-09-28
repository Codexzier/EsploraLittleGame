# Anforderungsbeschreibung: „Photo Quest“ (Suries Fotos)

Ausbau des kleinen Adventure-Spiels für den Arduino Esplora mit TFT-Display (160×128). Grundlage sind die vorhandenen Spielmechaniken: Laufen, Kollision, Gegenstände aufheben, Rucksack, Tür mit Schlüssel, Händlerin, Kiste, Münzen und Dialogfenster.

Das Spiel besteht aus zwei Teilen:
- **Teil 1:** das erste Foto für Surie (Schritte 1–7)
- **Teil 2:** drei weitere Fotos, eine Schiffs- und eine Busfahrt, zum Schluss ein Kaffee bei Surie (Schritte 7–15)

## 1. Rahmenbedingungen

| ID | Anforderung |
|---|---|
| R-01 | Zielhardware ist der Arduino Esplora (ATmega32U4, 28 KB Flash für den Sketch, 2,5 KB SRAM) mit dem Arduino TFT-Display 1.8". |
| R-02 | Verwendet werden nur die Bibliotheken `SPI`, `TFT` und `Esplora`. |
| R-03 | Der Sketch belegt höchstens 99 % des Flash-Speichers und höchstens 50 % des SRAM für globale Variablen. *(Stand: 98 % Flash, 18 % SRAM)* |
| R-04 | Grafiken, Karten und Texte liegen im Flash (`PROGMEM`). Im SRAM gibt es keine Bildpuffer. |
| R-05 | Der Spielstand liegt im EEPROM (1 KB, davon ca. 90 Byte belegt). |

## 2. Spielidee

**Teil 1:** Die Händlerin Surie wünscht sich ein Foto von „Baum und Haus im Sonnenschein“. Die Spielfigur findet den Schlüssel, öffnet die Tür und trifft Surie. Dann sammelt sie Münzen, kauft eine Kamera, fotografiert im Garten das Motiv und bringt Surie das Foto.

**Teil 2:** Surie freut sich so sehr, dass sie sich noch drei Fotos wünscht: eine Brücke, eine Insel und eine Stadt. Hinter dem Garten öffnet sich die Hecke:
- Der Weg führt über einen Fluss mit Brücke zum Hafen.
- Vom Hafen bringt ein Kapitän die Figur zur Nachbarinsel, von dort sieht man die Insel.
- Ein Bus fährt zum Stadtrand.

Mit allen Fotos kehrt die Figur zu Surie zurück. Surie hängt die Fotos in ihrem Haus auf und lädt zum Kaffee ein.

## 3. Spielablauf

| Schritt | Ziel (Anzeige unten rechts) | Auslöser | Ergebnis |
|---|---|---|---|
| 1 | „Finde den Schlüssel.“ | Kachel mit dem Schlüssel betreten | Schlüssel im Rucksack, Hinweisfenster |
| 2 | „Öffne die Tür.“ | Gegen die Tür laufen | Schlüssel wird verbraucht, die Tür ist dauerhaft offen |
| 3 | „Sprich mit Surie.“ | Gegen Surie laufen | Auftrag erhalten |
| 4 | „Sammle 200 Münzen für die Kamera.“ | Kiste öffnen, Münzen im Garten einsammeln | 25 (Start) + 75 (Kiste) + 4 × 25 (Garten) = 200 |
| 5 | „Kaufe die Kamera bei Surie.“ | Surie ansprechen, „Kaufe Kamera (200)“ | Kamera im Rucksack, 0 Münzen |
| 6 | „Fotografiere Baum und Haus (Garten).“ | Fotopunkt (gelbes X) betreten, „Foto machen“ | Foto im Rucksack |
| 7 | „Bring Surie das Foto.“ | Surie ansprechen, „Gib Surie das Foto“ | +150 Münzen. Surie wünscht sich drei weitere Fotos, die Hecke rechts im Garten wird zum Ausgang. |
| 8 | „Fotografiere die Brücke (Osten).“ | Fotopunkt am Fluss | Foto der Brücke |
| 9 | „Fotografiere die Insel im Meer.“ | Fotopunkt auf der Nachbarinsel | Foto der Insel |
| 10 | „Kaufe am Hafen ein Bootsticket.“ | Kapitän ansprechen, „Kaufe Ticket (60)“, danach „Losfahren“ | Rundfahrt: Seekarte mit fahrendem Schiff, Ankunft auf der Nachbarinsel. Das Ticket gilt für Hin- und Rückfahrt. |
| 11 | „Fotografiere die Stadt.“ | Fotopunkt am Stadtrand | Foto der Stadt |
| 12 | „Kaufe ein Ticket beim Busfahrer.“ | Busfahrer an der Haltestelle ansprechen, „Kaufe Ticket (40)“, danach „Losfahren“ | Bus fährt von der Seite gesehen durchs Bild, Ankunft am Stadtrand |
| 13 | (Stadtrand) | – | Häuserreihe mit Dächern deutet die Stadt an |
| 14 | „Bring Surie die drei Fotos.“ | Surie ansprechen, „Gib Surie die Fotos“ | Surie geht nach Hause und hängt die Fotos an die Wand |
| 15 | „Besuche Surie in ihrem Haus.“ | Gegen die Haustür im Garten laufen, in der Stube Surie ansprechen, „Kaffee trinken“ | Abschlussfenster |
| – | „Geschafft! [1] halten = Neu“ | Button 1 eine Sekunde halten | Startbildschirm |

**Münzen in Teil 2:** 150 (Belohnung) − 60 (Boot) − 40 (Bus) = 50 bleiben übrig.

**Sackgassen vermeiden:**
- Die Kamera kann erst verkauft werden, wenn alle Fotos abgegeben sind.
- Tickets gelten für beliebig viele Fahrten. So muss niemand ein zweites Ticket kaufen, falls ein Foto vergessen wurde.
- Der Weg zum Fluss öffnet sich erst nach dem ersten Foto. Vorher kann man die Münzen für die Kamera nicht für Tickets ausgeben.
- Rucksack am vollsten Punkt: Kamera, zwei Tickets und drei Fotos, genau 6 Plätze.

## 4. Funktionale Anforderungen

### Steuerung

| ID | Anforderung |
|---|---|
| F-01 | Der Joystick bewegt die Figur in vier Richtungen. Bei schrägem Ausschlag zählt die Achse mit dem größeren Ausschlag. Eine Totzone (`STICK_DEADBAND`) filtert das Rauschen. |
| F-02 | Die Figur läuft mit gleichmäßiger Geschwindigkeit (1 Pixel alle 16 ms), unabhängig davon, wie lange das Zeichnen dauert. |
| F-03 | Button 4 (rechts) bestätigt, Button 2 (links) schließt oder geht zurück. In Fenstern wechselt der Joystick hoch/runter die Auswahl mit Wiederholverzögerung. |
| F-04 | Wird Button 1 (unten) eine Sekunde gehalten, erscheint der Startbildschirm. Kurzes Drücken bewirkt nichts. |

### Bewegung und Kollision

| ID | Anforderung |
|---|---|
| F-10 | Kollisionen werden über den Fuß-Bereich der Figur (8×6 Pixel) geprüft. Der Kopf darf optisch vor Wänden stehen. |
| F-11 | Blockierende Kacheln: Wand, Tür (solange geschlossen), Kiste, Baum, Hauswand, Dach, Hecke, Haustür, Wasser, Palme, Boot, Haltestelle, Häuser der Stadt, ferne Insel, Tisch und Fotos an der Wand. |
| F-12 | Anstoßen an Tür, Kiste, Haustür, Boot, Haltestelle, Tisch, Foto an der Wand oder eine Figur löst genau **eine** Aktion aus. Erst nach einer Bewegung kann erneut ausgelöst werden. |
| F-13 | Das Betreten von Schlüssel, Münze, Ausgang oder Fotopunkt löst genau **einmal** aus. Maßgeblich ist die Mitte der Füße. |

### Karten

| ID | Anforderung |
|---|---|
| F-20 | Es gibt acht Karten mit je 10×6 Kacheln (siehe Weltkarte unten). |
| F-21 | Ausgänge (Fußmatte) verbinden die Karten. Die Figur erscheint auf der Zielkarte neben dem Gegen-Ausgang. |
| F-22 | Aufgesammelte Gegenstände, geöffnete Türen und die geleerte Kiste bleiben auch nach einem Kartenwechsel erhalten (Bitfeld `mTileConsumed` je Karte). |
| F-23 | Die Hecke rechts im Garten (`TILE_GATE`) wird nach dem ersten Foto zum Ausgang Richtung Fluss. |
| F-24 | Surie steht bis zur Abgabe der drei Fotos im Laden, danach in ihrer Stube. Die Haustür im Garten öffnet sich erst dann. |

```
                 Seekarte (Schiff)
    Nachbarinsel  <---------------->  Hafen  ---  Bushaltestelle
                                        |              |
                                      Fluss        Bus (Seitenansicht)
                                        |              |
    Haus (Laden) ---  Garten  ----------+          Stadtrand
                        |
                   Suries Stube
```

### Gegenstände und Rucksack

| ID | Anforderung |
|---|---|
| F-30 | Der Rucksack hat 6 Plätze. Belegte Plätze zeigen das Icon mit gelbem Rahmen, freie Plätze sind hellgrau mit grauem Rahmen. |
| F-31 | Jeder Gegenstand kann nur einmal im Rucksack sein. Beim Entfernen wird der Platz sofort neu gezeichnet. |
| F-32 | Gegenstände: Schlüssel (öffnet die Tür, wird verbraucht), Kamera (Kauf 200, Verkauf 140), vier Fotos (Baum und Haus, Brücke, Insel, Stadt), Bootsticket (60), Busticket (40). |
| F-33 | Welches Foto entsteht, hängt von der Karte des Fotopunktes ab. Ein Foto gibt es nur einmal. |

### Figuren und Dialoge

| ID | Anforderung |
|---|---|
| F-40 | Beim ersten Gespräch erklärt Surie den Auftrag. Danach öffnet sie ein Handelsmenü, dessen Optionen vom Spielstand abhängen: Foto geben, Fotos geben (nur mit allen drei), Kamera kaufen, Kamera verkaufen (erst am Ende), Tschüss. |
| F-41 | Bei zu wenig Münzen erscheint „Du hast nicht genug Münzen.“ |
| F-42 | Die Kiste fragt „Öffnen / Zu lassen“. Beim Öffnen gibt es 75 Münzen, danach ist sie leer. |
| F-43 | Das Dialogfenster hat einen Titel (gelb), einen Text mit Zeilenumbruch an Wortgrenzen (23 Zeichen je Zeile), bis zu 4 Auswahlpunkte und eine Fußzeile mit der Tastenbelegung. |
| F-44 | Nach dem Schließen wird die Karte unter dem Fenster neu gezeichnet, Figur und Surie inklusive. |
| F-45 | Längere Gespräche bestehen aus mehreren Fenstern hintereinander (`setWindowFollow`). |
| F-46 | Kapitän und Busfahrer nutzen dasselbe Sprite mit eigenen Farben (Haare, Shirt, Hose). Ohne Ticket bieten sie eins an, mit Ticket fragen sie „Losfahren?“. |
| F-47 | Das Anstoßen an Boot oder Haltestelle spricht den Kapitän bzw. den Busfahrer an. |
| F-48 | Die Fotos an Suries Wand zeigen beim Anstoßen ihre Beschreibung. |

### Reisen und Animationen

| ID | Anforderung |
|---|---|
| F-60 | **Seekarte:** Beim Losfahren zeigt die Karte die Küste mit dem Hafen, die Nachbarinsel und die kleine Insel, dazu eine gepunktete Route und die Beschriftungen „Seekarte“, „Hafen“, „Insel“. Das Schiff fährt schaukelnd die Route entlang, auf der Rückfahrt gespiegelt. |
| F-61 | **Bus:** Beim Losfahren zeigt die Szene Himmel, Hügel, Haltestelle und Straße von der Seite. Der Bus fährt ins Bild, hält an der Haltestelle und fährt weiter, auf der Rückfahrt in die andere Richtung. |
| F-62 | Während einer Animation bleibt der untere Bereich (Rucksack, Münzen, Ziel) sichtbar. Danach erscheint die Zielkarte. |

### Startbildschirm, Spielstand und Effekte

| ID | Anforderung |
|---|---|
| F-70 | Nach dem Einschalten erscheint der Startbildschirm mit dem Namen **„Photo Quest“** in doppelter Schriftgröße und einem Filmstreifen mit den vier Fotos. |
| F-71 | Auswahl „Neues Spiel“ und, nur wenn ein Spielstand gespeichert ist, „Spiel laden“. Mit Spielstand ist „Spiel laden“ vorausgewählt. Joystick hoch/runter wählt, Button 4 bestätigt. Button 2 schließt den Startbildschirm nicht. |
| F-72 | Der Spielstand wird automatisch im EEPROM gespeichert, bei jedem Kartenwechsel und nach jedem geschlossenen Fenster. Gespeichert werden Karte, Position, Blickrichtung, Fortschritt, Münzen, Rucksack und alle verwendeten Kacheln. `eeprom_update_…` schreibt nur geänderte Bytes und schont so den EEPROM. |
| F-73 | Eine Kennung (`SAVE_MAGIC`) an Adresse 0 zeigt, ob ein gültiger Spielstand vorhanden ist. Ändert sich der Aufbau, wird die Kennung erhöht, damit alte Spielstände ignoriert werden. |
| F-74 | **Blitzlicht:** Beim Fotografieren leuchtet die RGB-LED 80 ms weiß. Die drei LED-Pins werden direkt mit `digitalWrite` geschaltet, das spart den Flash-Speicher für `analogWrite`. |
| F-75 | **Schiffshorn:** Vor der Abfahrt ertönt über `Esplora.tone()` ein kurzer und ein langer tiefer Ton (165 Hz), bei der Ankunft ein kurzer. |

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
| G-03 | Pro Bewegungsschritt wird nur das Rechteck aus alter und neuer Position übertragen (höchstens 11×17 Pixel). Bei Animationen gilt das für das Schiff bzw. den Bus. |
| G-04 | Kacheln haben Texturen. 8×8-Muster: Wand, Boden, Gras, Weg, Hecke, Dach, Blumen, Wasser, Brücke, Sand, Straße, Häuserfront. 16×16-Bilder: Tür, Kiste, Ausgang, Baum, Hauswand, Haustür, Schlüssel, Fotopunkt, Palme, Boot, Haltestelle, ferne Insel, Tisch, Fotos. Transparente Pixel zeigen den Boden der Karte, beim Boot und der fernen Insel das Wasser. |
| G-05 | Alle Farben stammen aus einer zentralen Palette (`mPalette`, 31 Farben). Die Nummern 100–105 sind die veränderbaren Farben einer Figur (Haare, Shirt, Hose). |
| G-06 | Texturen werden in `tools/sprites.py` als lesbare Zeichengrafik gepflegt. Das Skript erzeugt daraus `AssetsData.ino`. |
| G-07 | Alle Bilder, auch 8×8-Muster und Figuren, werden **gepackt** gespeichert: 16 Byte Farbtabelle plus 4 Bit je Pixel. Ein 16×16-Bild braucht so 144 statt 256 Byte. |
| G-08 | Inseln der Seekarte und der Bus werden aus Kreisen und Rechtecken berechnet und brauchen dadurch kaum Flash. |

## 6. Nicht-funktionale Anforderungen

| ID | Anforderung |
|---|---|
| N-01 | Jede Komponente liegt in einer eigenen Datei: Assets, Backpack, Collision, Figure, Map, Quest, Render, Save, Title, Trader, Travel, Window. Gemeinsame Konstanten und Zustände stehen in `EsploraLittleGame.ino`, weil die IDE die Dateien alphabetisch anhängt. |
| N-02 | Kommentarstil und Benennung (`m`-Präfix, deutsche Kommentare, Trennlinien) bleiben wie im bestehenden Code. |
| N-03 | Das Spiel lässt sich ohne Hardware testen: `tools/simulator` baut den Sketch für den PC, spielt ihn automatisch durch, prüft den Spielstand und speichert Bildschirmfotos, auch mitten in den Animationen. |

## 7. Abnahmekriterien

Alle Punkte prüft der Simulator (`python3 tools/simulator/build.py <TFT-Bibliothek>`):

1. Nach dem Einschalten ohne Spielstand zeigt der Startbildschirm nur „Neues Spiel“. Button 2 schließt ihn nicht.
2. Der Start im Haus zeigt Figur, Surie, Schlüssel, Kiste, Tür und Ausgang. Anzeige: 25 Münzen, Ziel „Finde den Schlüssel.“
3. Die Tür ohne Schlüssel bleibt zu und zeigt einen Hinweis.
4. Der Schlüssel wird genau einmal aufgenommen.
5. Die Kiste gibt einmal 75 Münzen und ist danach leer.
6. Die Tür öffnet sich mit dem Schlüssel, der Schlüssel verschwindet aus dem Rucksack.
7. Surie gibt den Auftrag. Beim Kaufversuch mit 100 Münzen erscheint „nicht genug Münzen“.
8. Der Kartenwechsel in den Garten und zurück funktioniert, die 4 Münzen ergeben 200.
9. Ohne Kamera zeigt der Fotopunkt nur einen Hinweis. Mit Kamera entsteht das Foto, und die LED blitzt.
10. Die Auswahl im Menü lässt sich mit dem Joystick bewegen.
11. Die Abgabe des ersten Fotos gibt 150 Münzen. Surie wünscht sich drei weitere Fotos, das Spiel endet nicht.
12. Die Hecke im Garten ist offen, am Fluss entsteht das Foto der Brücke.
13. Der Kapitän verkauft das Bootsticket (60), das Schiffshorn ertönt, die Seekarten-Animation läuft, die Figur kommt auf der Nachbarinsel an.
14. Nach Aus- und Einschalten ist „Spiel laden“ vorausgewählt. Das Laden stellt Karte, Position, Münzen, Rucksack und Fortschritt wieder her.
15. Auf der Insel entsteht das Foto der Insel. Die Rückfahrt klappt, und das Ticket bleibt gültig.
16. Der Busfahrer verkauft das Busticket (40), die Bus-Animation läuft, die Figur kommt am Stadtrand an.
17. Am Stadtrand entsteht das Foto der Stadt, danach geht es mit dem Bus zurück.
18. Surie nimmt die drei Fotos an und verlässt den Laden.
19. Die Haustür im Garten führt in Suries Stube. Die Fotos hängen an der Wand und zeigen beim Anstoßen ihre Beschreibung.
20. Nach dem Kaffee erscheint das Abschlussfenster.
21. Kurzes Drücken von Button 1 bewirkt nichts. Eine Sekunde Halten führt zum Startbildschirm, dort startet „Neues Spiel“ von vorn.

## 8. Mögliche nächste Ausbaustufen

Der Flash-Speicher ist mit 98 % fast voll. Weitere Inhalte brauchen vorher Platz, zum Beispiel durch kürzere Texte oder eine Textkompression.

- **Rucksack benutzen:** Mit Button 3 in den Rucksack wechseln, mit dem Joystick einen Platz wählen und die Beschreibung anzeigen. Die Texte (`mItem…Description`) sind schon vorhanden.
- **Weitere Töne:** kurze Signale beim Aufheben oder für den Bus. `tone()` ist bereits eingebunden, jeder weitere Ton kostet nur wenige Bytes.
