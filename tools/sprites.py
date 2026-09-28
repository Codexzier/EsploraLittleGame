#!/usr/bin/env python3
# ========================================================================================
# Description:       Hilfsskript um Kacheln / Sprites als lesbare Zeichen-Grafik zu pflegen.
#                    Erzeugt die Datei AssetsData.ino mit allen PROGMEM Arrays.
#
# Formate:           8x8 Muster        1 Byte je Pixel (Farbnummer)
#                    alle anderen      gepackt: 16 Byte Farbtabelle + 4 Bit je Pixel
#                                      (spart fast die Haelfte Flash Speicher)
#
# Aufruf:            python3 tools/sprites.py
# ========================================================================================

import os

# Zeichen -> Farbnummer (siehe mPalette in RenderComponent.ino)
PALETTE_CHARS = {
    '.': 0,   # transparent
    'K': 1,   # schwarz
    's': 2,   # haut
    'b': 3,   # braun
    'B': 4,   # hell braun
    'y': 5,   # braun gelb
    'g': 6,   # gruen
    'G': 7,   # dunkel gruen
    'l': 8,   # hell gruen
    'n': 9,   # dunkel grau blau
    'N': 10,  # grau blau
    'c': 11,  # hell blau
    'Y': 12,  # gelb
    'o': 13,  # orange
    'r': 14,  # hell rot
    'm': 15,  # hell gruen 2
    'R': 16,  # rot
    'd': 17,  # dunkel braun
    'e': 18,  # grau
    'w': 19,  # sehr hell grau
    'h': 20,  # hell haut
    'D': 21,  # dunkel grau
    'z': 22,  # ziegel rot
    'W': 23,  # holz dunkel
    'X': 24,  # weiss
    'v': 25,  # wasser blau
    'V': 26,  # wasser dunkel
    'S': 27,  # sand
    'A': 28,  # asphalt
    'q': 29,  # himmel blau
    'p': 30,  # sand dunkel
    '1': 100, # Figur: Haare 1
    '2': 101, # Figur: Haare 2
    '3': 102, # Figur: T-Shirt 1
    '4': 103, # Figur: T-Shirt 2
    '5': 104, # Figur: Hose 1
    '6': 105, # Figur: Hose 2
}

SPRITES = {}

# ----------------------------------------------------------------------------------------
# 8x8 Muster (werden auf 16x16 Kacheln wiederholt)

SPRITES['mTileFloor'] = [
    "mmmmmmml",
    "mmmmmmml",
    "mmmmmmml",
    "llllllll",
    "mmmlmmmm",
    "mmmlmmmm",
    "mmmlmmmm",
    "llllllll",
]

SPRITES['mTileWall'] = [
    "NNNNNNNn",
    "cNNNNNNn",
    "NNNNNNNn",
    "nnnnnnnn",
    "NNNnNNNN",
    "NNNncNNN",
    "NNNnNNNN",
    "nnnnnnnn",
]

SPRITES['mTileGrass'] = [
    "gggggggg",
    "ggGgggll",
    "gggggggg",
    "gggglggg",
    "ggggGggg",
    "glgggggg",
    "gggggGgg",
    "gggggggg",
]

SPRITES['mTilePath'] = [
    "hhhhhhhh",
    "hhBhhhhh",
    "hhhhhhBh",
    "hhhhhhhh",
    "hBhhhhhh",
    "hhhhBhhh",
    "hhhhhhhh",
    "hhhhhhhB",
]

SPRITES['mTileHedge'] = [
    "GgGGGgGG",
    "gGGlGGGg",
    "GGgGGgGG",
    "GgGGGGgG",
    "GGGgGGlG",
    "gGGGgGgG",
    "GlgGGGGg",
    "GGGGgGGG",
]

SPRITES['mTileRoof'] = [
    "rzzzrzzz",
    "zzzzzzzz",
    "zzzzzzzz",
    "dddddddd",
    "zzrzzzzr",
    "zzzzzzzz",
    "zzzzzzzz",
    "dddddddd",
]

SPRITES['mTileFlowers'] = [
    "........",
    "..R.....",
    ".RYR....",
    "..R..X..",
    "..G.XYX.",
    ".....X..",
    ".....G..",
    "........",
]

# ----------------------------------------------------------------------------------------
# 16x16 Kacheln

SPRITES['mTileDoor'] = [
    "dddddddddddddddd",
    "dooBooooBooooBod",
    "dooBooooBooooBod",
    "dooBooooBooooBod",
    "dddddddddddddddd",
    "dooBooooBooooBod",
    "dooBooooBooooBod",
    "dooBooooBooYYBod",
    "dooBooooBooYKBod",
    "dooBooooBooYKBod",
    "dooBooooBooYYBod",
    "dddddddddddddddd",
    "dooBooooBooooBod",
    "dooBooooBooooBod",
    "dooBooooBooooBod",
    "dddddddddddddddd",
]

SPRITES['mTileChest'] = [
    "................",
    "................",
    "..KKKKKKKKKKKK..",
    ".KBBBBBBBBBBBBK.",
    ".KBooooooooooBK.",
    ".KBooooooooooBK.",
    ".KKKKKKKKKKKKKK.",
    ".KeBBBBYYBBBBeK.",
    ".KeooooYKooooeK.",
    ".KeooooYYooooeK.",
    ".KeooooooooooeK.",
    ".KeooooooooooeK.",
    ".KeBBBBBBBBBBeK.",
    ".KKKKKKKKKKKKKK.",
    "................",
    "................",
]

SPRITES['mTileExit'] = [
    "................",
    "................",
    "..zzzzzzzzzzzz..",
    "..zYYYYYYYYYYz..",
    "..zYzzzzzzzzYz..",
    "..zYzYYYYYYzYz..",
    "..zYzzzzzzzzYz..",
    "..zYzYYYYYYzYz..",
    "..zYzzzzzzzzYz..",
    "..zYzYYYYYYzYz..",
    "..zYzzzzzzzzYz..",
    "..zYzYYYYYYzYz..",
    "..zYzzzzzzzzYz..",
    "..zYYYYYYYYYYz..",
    "..zzzzzzzzzzzz..",
    "................",
]

SPRITES['mTileTree'] = [
    ".....KKKKKK.....",
    "...KKgggllgKK...",
    "..KgggglllggGK..",
    ".KggGgggllgggGK.",
    ".KgggggggggGgGK.",
    "KggGggggGgggggGK",
    "KgggggGgggggGgGK",
    "KGggggggggGgggGK",
    ".KGgGgggGgggGGK.",
    ".KGGggGgggGGGGK.",
    "..KKGGGGGGGGKK..",
    "....KKKdbKKK....",
    "......KdbK......",
    "......KdbK......",
    ".....KdbbdK.....",
    ".....KKKKKK.....",
]

SPRITES['mTileHouseWall'] = [
    "eeeeeeeeeeeeeeee",
    "wwwwwwwwwwwwwwww",
    "wwwwKKKKKKKKwwww",
    "wwwwKXccKXcKwwww",
    "wwwwKccKccKKwwww",
    "wwwwKKKKKKKKwwww",
    "wwwwKccKcccKwwww",
    "wwwwKccKcccKwwww",
    "wwwwKKKKKKKKwwww",
    "wwwwwwwwwwwwwwww",
    "wwwwwwwwwwwwwwww",
    "wwwwwwwwwwwwwwww",
    "wwwwwwwwwwwwwwww",
    "wwwwwwwwwwwwwwww",
    "eeeeeeeeeeeeeeee",
    "DDDDDDDDDDDDDDDD",
]

SPRITES['mTileHouseDoor'] = [
    "eeeeeeeeeeeeeeee",
    "wwwwwwwwwwwwwwww",
    "wwwwKKKKKKKKwwww",
    "wwwKWWWWWWWWKwww",
    "wwwKWddWWddWKwww",
    "wwwKWddWWddWKwww",
    "wwwKWWWWWWWWKwww",
    "wwwKWWWWWWWWKwww",
    "wwwKWWWWWWYWKwww",
    "wwwKWddWWdYWKwww",
    "wwwKWddWWddWKwww",
    "wwwKWddWWddWKwww",
    "wwwKWWWWWWWWKwww",
    "wwwKWWWWWWWWKwww",
    "eeeKKKKKKKKKKeee",
    "DDDDDDDDDDDDDDDD",
]

SPRITES['mTilePhotoSpot'] = [
    "................",
    "................",
    "................",
    "...YY......YY...",
    "...YYY....YYY...",
    "....YYY..YYY....",
    ".....YYYYYY.....",
    "......YYYY......",
    "......YYYY......",
    ".....YYYYYY.....",
    "....YYY..YYY....",
    "...YYY....YYY...",
    "...YY......YY...",
    "................",
    "................",
    "................",
]

# Schluessel (Karte und Rucksack Icon)
SPRITES['mItemKey01Icon'] = [
    "................",
    "................",
    "................",
    "................",
    "..........KKKK..",
    ".........KYYYYK.",
    ".KKKKKKKKYK..KYK",
    "KYYYYYYYYYK..KYK",
    ".KKKKYKYKYK..KYK",
    "....KYKYKKYYYYK.",
    "....KKKKK.KKKK..",
    "................",
    "................",
    "................",
    "................",
    "................",
]

# ----------------------------------------------------------------------------------------
# Neue Karten: Fluss, Hafen, Insel, Bushaltestelle, Stadt, Suries Stube

SPRITES['mTileWater'] = [
    "vvvvvvvv",
    "vvcXcvvv",
    "vvvvvvvv",
    "vvvvvvVv",
    "vvvvvvvv",
    "vvvvvvcX",
    "cvvvvvvv",
    "vVvvvvvv",
]

SPRITES['mTileBridge'] = [
    "BBBWBBBW",
    "BBBWBBBW",
    "BdBWBBBW",
    "BBBWBBBW",
    "BBBWBdBW",
    "BBBWBBBW",
    "BBBWBBBW",
    "BBBWBBBW",
]

SPRITES['mTileSand'] = [
    "SSSSSSSS",
    "SSSpSSSS",
    "SSSSSSSS",
    "SSSSSSpS",
    "SpSSSSSS",
    "SSSSSSSS",
    "SSSSpSSS",
    "SSSSSSSS",
]

SPRITES['mTileRoad'] = [
    "AAAAAAAA",
    "AAADAAAA",
    "AAAAAAAA",
    "AAAAAAeA",
    "AAAAAAAA",
    "ADAAAAAA",
    "AAAAAAAA",
    "AAAAeAAA",
]

SPRITES['mTileFacade'] = [
    "hhhhhhhh",
    "hnnhhnnh",
    "hcnhhcnh",
    "hnnhhnnh",
    "hhhhhhhh",
    "eeeeeeee",
    "hhhhhhhh",
    "hhhhhhhh",
]

SPRITES['mTilePalm'] = [
    "................",
    "...GGG....GGG...",
    "..GlllG..GlllG..",
    ".Gll..GGGG..llG.",
    ".G...GllllG...G.",
    ".....GlGGlG.....",
    "....Gl.bb.lG....",
    "....G..bd..G....",
    ".......bd.......",
    "........bd......",
    "........bd......",
    "........bd......",
    ".......bd.......",
    ".......bd.......",
    "......bbdd......",
    ".....pppppp.....",
]

SPRITES['mTileBoat'] = [
    "................",
    "......K.........",
    "......KX........",
    "......KXX.......",
    "......KXXX......",
    "......KXXXX.....",
    "......KXXXXX....",
    "......KXXXXXX...",
    "......K.........",
    ".KKKKKKKKKKKKKK.",
    "KzzzzzzzzzzzzzzK",
    ".KwwwwwwwwwwwwK.",
    "..KWWWWWWWWWWK..",
    "...KKKKKKKKKK...",
    "..cXc.....cXc...",
    "................",
]

SPRITES['mTileBusSign'] = [
    "....KKKKKKK.....",
    "...KYYYYYYYK....",
    "..KYYGYYYGYYK...",
    "..KYYGYYYGYYK...",
    "..KYYGGGGGYYK...",
    "..KYYGYYYGYYK...",
    "..KYYGYYYGYYK...",
    "...KYYYYYYYK....",
    "....KKKKKKK.....",
    ".......eD.......",
    ".......eD.......",
    ".......eD.......",
    ".......eD.......",
    ".......eD.......",
    "......eeDD......",
    "................",
]

SPRITES['mTileFarIsland'] = [
    "................",
    "................",
    "......GGG.......",
    ".....GlllG......",
    "....Gl.b.lG.....",
    "......bd........",
    "......bd........",
    "....ggbdggg.....",
    "...gglgggglgg...",
    "..gggggggggggg..",
    ".SSSSSSSSSSSSSS.",
    "..SSSpSSSSpSSS..",
    "....XX....XX....",
    "................",
    "................",
    "................",
]

SPRITES['mTileTable'] = [
    "................",
    "................",
    ".KKKKKKKKKKKKKK.",
    ".KBBBBBBBBBBBBK.",
    ".KBBXXBBBBXXBBK.",
    ".KBXddXBBXddXBK.",
    ".KBXddXBBXddXBK.",
    ".KBBXXBBBBXXBBK.",
    ".KBBBBBBBBBBBBK.",
    ".KddddddddddddK.",
    ".KKKKKKKKKKKKKK.",
    "..Kd........dK..",
    "..KK........KK..",
    "................",
    "................",
    "................",
]

# ----------------------------------------------------------------------------------------
# Rucksack Icons

SPRITES['mItemCameraIcon'] = [
    "................",
    "................",
    ".....KKKKKK.....",
    "....KnnnnnnK....",
    ".KKKKnwwwwnKKKK.",
    "KnnnnnnKKnnnnnnK",
    "KnnnnKKccKKnwwnK",
    "KnnnnKccccKnwwnK",
    "KnnnKccccccKnnnK",
    "KnnnKccccccKnnnK",
    "KnnnnKccccKnnnnK",
    "KnnnnKKccKKnnnnK",
    "KnnnnnnKKnnnnnnK",
    ".KKKKKKKKKKKKKK.",
    "................",
    "................",
]

SPRITES['mItemPhoto01Icon'] = [
    "KKKKKKKKKKKKKKKK",
    "KwwwwwwwwwwwYwwK",
    "KwweeewwwwwYYYwK",
    "KweeeeewewYYYYYK",
    "KwweewwewwwYYYwK",
    "KwwwwwwwwwwwYwwK",
    "KwwwwwwwwwwwwwwK",
    "KwwwwwwwwwwwwwwK",
    "KwwgwwwwwwwdwwwK",
    "KwgggwwwwwdddwwK",
    "KwgggwwwwdcdbdwK",
    "KwwbwwwwwdcdbdwK",
    "KwwbwwwwwdddbdwK",
    "KllllllllllllllK",
    "KllllllllllllllK",
    "KKKKKKKKKKKKKKKK",
]

SPRITES['mItemPhotoBridgeIcon'] = [
    "KKKKKKKKKKKKKKKK",
    "KqqqqqqqqqqqqqqK",
    "KqqXXqqqqqqqqqqK",
    "KqXXXXqqqqqqqqqK",
    "KqqqqqqqqqqqqqqK",
    "KqqqqqqqqqqqqqqK",
    "KBBBBBBBBBBBBBBK",
    "KdBdddBddBdddBdK",
    "KBBBBBBBBBBBBBBK",
    "KgBBvvvvvvvvBBgK",
    "KgBvvvvvvvvvvBgK",
    "KgBvvcvvvvcvvBgK",
    "KggvvvvvvvvvvggK",
    "KggvvvcvvvvvvggK",
    "KgvvvvvvvvvvvvgK",
    "KKKKKKKKKKKKKKKK",
]

SPRITES['mItemPhotoIslandIcon'] = [
    "KKKKKKKKKKKKKKKK",
    "KqqqqqqqqqqqYYqK",
    "KqqqqqqqqqqYYYYK",
    "KqqqqGGGqqqqYYqK",
    "KqqqGlllGqqqqqqK",
    "KqqGlqbqlGqqqqqK",
    "KqqqqqbdqqqqqqqK",
    "KqqqqqbdqqqqqqqK",
    "KqqqgggbdggqqqqK",
    "KqqgglggggglgqqK",
    "KqSSSSSSSSSSSSqK",
    "KvvvvvvvvvvvvvvK",
    "KvvcvvvvvvvcvvvK",
    "KvvvvvvXvvvvvvvK",
    "KvVvvvvvvvvvVvvK",
    "KKKKKKKKKKKKKKKK",
]

SPRITES['mItemPhotoCityIcon'] = [
    "KKKKKKKKKKKKKKKK",
    "KqqqqqqqqqqqqqqK",
    "KqqqqqqeeqqqqqqK",
    "KqqqqqqeeqqqqqqK",
    "KqqNNqqeeqqqqqqK",
    "KqqNNqeeeeqzzqqK",
    "KqqNNqeYeYqzzqqK",
    "KhhNNqeeeeqzzqqK",
    "KhYhNNeYeYezzhhK",
    "KhhhNNeeeeezzhYK",
    "KhYhNYeYeYezYhhK",
    "KhhhNNeeeeezzhhK",
    "KDDDDDDDDDDDDDDK",
    "KDDXXDDDDXXDDDDK",
    "KDDDDDDDDDDDDDDK",
    "KKKKKKKKKKKKKKKK",
]

SPRITES['mItemBoatTicketIcon'] = [
    "................",
    "................",
    "................",
    ".KKKKKKKKKKKKKK.",
    ".KvvvvvvvvvvvvK.",
    ".KXXXXXKXXXXXXK.",
    ".KXXXXXKXXXeXXK.",
    ".KXXXXXKXXXXXXK.",
    ".KXzzzzzzzXeXXK.",
    ".KXXzzzzzXXXXXK.",
    ".KvvvvvvvvvvvvK.",
    ".KKKKKKKKKKKKKK.",
    "................",
    "................",
    "................",
    "................",
]

SPRITES['mItemBusTicketIcon'] = [
    "................",
    "................",
    "................",
    ".KKKKKKKKKKKKKK.",
    ".KYYYYYYYYYYYYK.",
    ".KXXXXXXXXXXXXK.",
    ".KXooooooooXeXK.",
    ".KXocccocccXXXK.",
    ".KXooooooooXeXK.",
    ".KXXKXXXXKXXXXK.",
    ".KYYYYYYYYYYYYK.",
    ".KKKKKKKKKKKKKK.",
    "................",
    "................",
    "................",
    "................",
]

# ----------------------------------------------------------------------------------------
# Schiff fuer die Seekarte (12x10)

SPRITES['mShipSprite'] = [
    ".....K......",
    ".....KX.....",
    ".....KXX....",
    ".....KXXX...",
    ".....KXXXX..",
    ".....K......",
    "KKKKKKKKKKKK",
    "KzzzzzzzzzzK",
    ".KWWWWWWWWK.",
    "..KKKKKKKK..",
]


# ----------------------------------------------------------------------------------------
# Figuren 10x16 (Spielfigur, Surie, Kapitaen/Busfahrer)
# 1 bis 6 = veraenderbare Farben der Figur (Haare 1/2, Shirt 1/2, Hose 1/2, Farbnummer 100 bis 105)

SPRITES['mSpriteFigureFrontLeft'] = [
    "..KKKKKK..",
    ".KbbbbbbK.",
    "KbbbbbbbbK",
    "KbbBBsBbbK",
    "KbyyssyybK",
    "KbBKssKBbK",
    ".KsKssKsK.",
    "..KssssKK.",
    ".KllbbllgK",
    "KllllllgsK",
    "KsKllllKK.",
    ".KKNNGGK..",
    ".KnNKGgK..",
    "..KKKGgK..",
    "....KncK..",
    ".....KK...",
]

SPRITES['mSpriteFigureFrontMiddle'] = [
    "..KKKKKK..",
    ".KbbbbbbK.",
    "KbbbbbbbbK",
    "KbbBsBBbbK",
    "KbyyssyybK",
    "KbBKssKBbK",
    ".KsKssKsK.",
    "..KssssK..",
    ".KllbbllK.",
    "KlgllllglK",
    "KsKllllKsK",
    ".KKGGGGKK.",
    "..KgGGgK..",
    "..KgGGgK..",
    "..KnNNnK..",
    "...KKKK...",
]

SPRITES['mSpriteFigureSideLeft'] = [
    "..KKKKK...",
    ".KbbbbbK..",
    "KbbbbbbyK.",
    "KBBbbbbbbK",
    ".KysbbbbbK",
    ".KsKsNbbbK",
    ".KsKssNbK.",
    ".KsssssBK.",
    "..KblllK..",
    "..KlllgKK.",
    ".KslllGKK.",
    ".KGGKGGK..",
    "..KKNggK..",
    ".KNNKgNcK.",
    ".KKKKKNnK.",
    "......KK..",
]

SPRITES['mSpriteFigureSideMiddle'] = [
    "..KKKKK...",
    ".KbbbbbK..",
    "KbbbbbbyK.",
    "KBBbbbbbbK",
    ".KysbbbbbK",
    ".KsKsNbbbK",
    ".KsKssNbK.",
    ".KsssssBK.",
    "..KKbllK..",
    "...KllgK..",
    "...KllgK..",
    "...KsGKK..",
    "...KKNKK..",
    "...KNNNK..",
    "...KnnKK..",
    "...KKKKK..",
]

SPRITES['mSpriteFigureSideRight'] = [
    "..KKKKK...",
    ".KbbbbbK..",
    "KbbbbbbyK.",
    "KBBbbbbbbK",
    ".KysbbbbbK",
    ".KsKsNbbbK",
    ".KsKssNbK.",
    ".KsssssBK.",
    ".KKblllK..",
    "KsKlllgKK.",
    "KKKllKGgK.",
    "..KGKKGsK.",
    "..KKNgKKK.",
    ".KcNNKnnK.",
    ".KnnnKKK..",
    "..KKK.....",
]

SPRITES['mSpriteFigureBackLeft'] = [
    "..KKKKKK..",
    ".KbBBbbBK.",
    "KBbbbbbbBK",
    "KbbbbbbbbK",
    "KBbbbbNbBK",
    "KyBbbbbbNK",
    ".KNNbbbNK.",
    ".KKKKKKK..",
    "KllllllgK.",
    "KsKgllgllK",
    ".KKllllKsK",
    "..KgGGGKK.",
    "..KggKggK.",
    "..KGGKKK..",
    "..KnnK....",
    "...KK.....",
]

SPRITES['mSpriteFigureBackMiddle'] = [
    "..KKKKKK..",
    ".KbBBbbBK.",
    "KBbbbbbbBK",
    "KbbbbbbbbK",
    "KBbbbbNbBK",
    "KyBbbbbbNK",
    ".KNNbbbNK.",
    "..KKKKKK..",
    ".KlllllgK.",
    "KlggllgglK",
    "KsKllllKsK",
    ".KKGggGKK.",
    "..KgGGgK..",
    "..KnnnnK..",
    "...KKKK...",
    "..........",
]

SPRITES['mTraderSpriteFrontMen'] = [
    "..KKKKKK..",
    ".K111111K.",
    "K11112111K",
    "K112s2211K",
    "K1yyssyy1K",
    "K22KssK21K",
    ".KsKssKsK.",
    "..KssssK..",
    ".K33bb33K.",
    "K34333343K",
    "KsK3333KsK",
    ".KK5665KK.",
    "..K5665K..",
    "..K5665K..",
    "..KnNNnK..",
    "...KKKK...",
]

SPRITES['mTraderSpriteFrontWomen'] = [
    "..KKKKKK..",
    ".K111112K.",
    "K21112111K",
    "K112h2211K",
    "K1yyhhyy1K",
    "K12KhhK21K",
    "K1hKhhKh1K",
    "K1shhhhs1K",
    ".K33hh33K.",
    "K34333343K",
    "Kh644446hK",
    ".K655556K.",
    ".K655556K.",
    "K66555566K",
    ".K6nNNn6K.",
    "..KKKKKK..",
]

# ----------------------------------------------------------------------------------------
# Ausgabe

def check(name, rows):
    width = len(rows[0])
    for r in rows:
        assert len(r) == width, (name, r, len(r))
    return width, len(rows)


def format_bytes(values, per_line):
    lines = []
    for i in range(0, len(values), per_line):
        lines.append('  ' + ','.join(str(v) for v in values[i:i + per_line]) + ',')
    lines[-1] = lines[-1].rstrip(',')
    return '\n'.join(lines)


def raw_array(name, rows):
    width, height = check(name, rows)
    values = [PALETTE_CHARS[c] for r in rows for c in r]
    return 'const PROGMEM byte %s[%d] = {                // %dx%d, 1 Byte je Pixel\n%s\n};' % (
        name, len(values), width, height, format_bytes(values, width))


def packed_array(name, rows):
    width, height = check(name, rows)
    values = [PALETTE_CHARS[c] for r in rows for c in r]
    palette = []
    if 0 in values:
        palette.append(0)                                  # durchsichtig immer an erster Stelle
    for v in values:
        if v not in palette:
            palette.append(v)
    assert len(palette) <= 16, (name, palette)
    palette += [0] * (16 - len(palette))
    nibbles = [palette.index(v) for v in values]
    if len(nibbles) % 2:
        nibbles.append(0)
    data = [(nibbles[i] << 4) | nibbles[i + 1] for i in range(0, len(nibbles), 2)]
    return 'const PROGMEM byte %s[%d] = {                // %dx%d, gepackt\n%s,\n%s\n};' % (
        name, 16 + len(data), width, height, format_bytes(palette, 16), format_bytes(data, width // 2))


HEADER = '''// ========================================================================================
// Description:       Grafiken (Kacheln, Icons, Figuren, Schiff).
//                    DIESE DATEI WIRD ERZEUGT: python3 tools/sprites.py
//                    Aenderungen bitte in tools/sprites.py vornehmen.
// ----------------------------------------------------------------------------------------
// 8x8 Muster:        1 Byte je Pixel (Farbnummer aus mPalette)
// gepackt:           16 Byte Farbtabelle, danach 4 Bit je Pixel (Index in die Farbtabelle),
//                    auslesen mit getPackedPixel()
// ========================================================================================
'''

if __name__ == '__main__':
    parts = [HEADER]
    for name, rows in SPRITES.items():
        is_pattern = len(rows) == 8 and len(rows[0]) == 8
        parts.append(raw_array(name, rows) if is_pattern else packed_array(name, rows))
    target = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'AssetsData.ino')
    with open(target, 'w') as f:
        f.write('\n\n'.join(parts) + '\n')
    print('geschrieben: AssetsData.ino (%d Grafiken)' % len(SPRITES))
