# Anforderungsbeschreibung: „Suries Fotos“

Ausbau des kleinen Adventure-Spiels für den Arduino Esplora mit TFT-Display (160×128). Grundlage sind die vorhandenen Spielmechaniken: Laufen, Kollision, Gegenstände aufheben, Rucksack, Tür mit Schlüssel, Händlerin, Kiste, Münzen und Dialogfenster.

Das Spiel besteht aus zwei Teilen:
- **Teil 1:** das erste Foto für Surie (Schritte 1–7)
- **Teil 2:** drei weitere Fotos, eine Schiffs- und eine Busfahrt, zum Schluss ein Kaffee bei Surie (Schritte 7–15)

## 1. Rahmenbedingungen

| ID | Anforderung |
|---|---|
| R-01 | Zielhardware ist der Arduino Esplora (ATmega32U4, 28 KB Flash für den Sketch, 2,5 KB SRAM) mit dem Arduino TFT-Display 1.8". |
| R-02 | Verwendet werden nur die Bibliotheken `SPI`, `TFT` und `Esplora`. |
| R-03 | Der Sketch belegt höchstens 95 % des Flash-Speichers und höchstens 50 % des SRAM für globale Variablen. *(Stand: 94 % Flash, 18 % SRAM)* |
| R-04 | Grafiken, Karten und Texte liegen im Flash (`PROGMEM`). Im SRAM gibt es keine Bildpuffer. |

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
| – | „Geschafft! [1] halten = Neu“ | Button 1 eine Sekunde halten | Neues Spiel |

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
| F-04 | Wird Button 1 (unten) eine Sekunde gehalten, startet das Spiel neu. Kurzes Drücken bewirkt nichts. |

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
| G-07 | 16×16-Bilder werden **gepackt** gespeichert: 16 Byte Farbtabelle plus 4 Bit je Pixel, also 144 statt 256 Byte. |
| G-08 | Inseln der Seekarte und der Bus werden aus Kreisen und Rechtecken berechnet und brauchen dadurch kaum Flash. |

## 6. Nicht-funktionale Anforderungen

| ID | Anforderung |
|---|---|
| N-01 | Jede Komponente liegt in einer eigenen Datei: Assets, Backpack, Collision, Figure, Map, Quest, Render, Trader, Travel, Window. Gemeinsame Konstanten und Zustände stehen in `EsploraLittleGame.ino`, weil die IDE die Dateien alphabetisch anhängt. |
| N-02 | Kommentarstil und Benennung (`m`-Präfix, deutsche Kommentare, Trennlinien) bleiben wie im bestehenden Code. |
| N-03 | Das Spiel lässt sich ohne Hardware testen: `tools/simulator` baut den Sketch für den PC, spielt ihn automatisch durch, prüft den Spielstand und speichert Bildschirmfotos, auch mitten in den Animationen. |

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
10. Die Abgabe des ersten Fotos gibt 150 Münzen. Surie wünscht sich drei weitere Fotos, das Spiel endet nicht.
11. Die Hecke im Garten ist offen, am Fluss entsteht das Foto der Brücke.
12. Der Kapitän verkauft das Bootsticket (60), die Seekarten-Animation läuft, die Figur kommt auf der Nachbarinsel an.
13. Auf der Insel entsteht das Foto der Insel. Die Rückfahrt klappt, und das Ticket bleibt gültig.
14. Der Busfahrer verkauft das Busticket (40), die Bus-Animation läuft, die Figur kommt am Stadtrand an.
15. Am Stadtrand entsteht das Foto der Stadt, danach geht es mit dem Bus zurück.
16. Surie nimmt die drei Fotos an und verlässt den Laden.
17. Die Haustür im Garten führt in Suries Stube. Die Fotos hängen an der Wand und zeigen beim Anstoßen ihre Beschreibung.
18. Nach dem Kaffee erscheint das Abschlussfenster.
19. Kurzes Drücken von Button 1 setzt nicht zurück, eine Sekunde Halten setzt alles zurück.

## 8. Mögliche nächste Ausbaustufen

- **Rucksack benutzen:** Mit Button 3 in den Rucksack wechseln, mit dem Joystick einen Platz wählen und die Beschreibung anzeigen. Die Texte (`mItem…Description`) sind schon vorhanden.
- **Rückmeldung über die Esplora-Hardware:** Mit `Esplora.tone()` ein Ton beim Aufheben oder als Schiffshorn, mit `Esplora.writeRGB()` ein Blitzlicht beim Foto.
- **Spielstand speichern** im EEPROM (Bitfelder, Rucksack, Münzen: unter 80 Byte).
- **Mehr Flash freimachen:** Auch die Figuren-Sprites gepackt speichern (spart ca. 600 Byte).
