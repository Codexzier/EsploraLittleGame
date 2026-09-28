#!/usr/bin/env python3
# ========================================================================================
# Description:       Hilfsskript um Kacheln / Sprites als lesbare Zeichen-Grafik zu pflegen.
#                    Gibt die PROGMEM Byte Arrays fuer MapComponent.ino und
#                    BackpackComponent.ino (Schluessel Icon) aus.
#
# Aufruf:            python3 tools/sprites.py
# ========================================================================================

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


def to_array(name, rows):
    width = len(rows[0])
    for r in rows:
        assert len(r) == width, (name, r, len(r))
    values = [PALETTE_CHARS[c] for r in rows for c in r]
    lines = []
    for r in range(len(rows)):
        lines.append('  ' + ','.join(str(v) for v in values[r * width:(r + 1) * width]) + ',')
    lines[-1] = lines[-1].rstrip(',')
    return 'const PROGMEM byte %s[%d] = {\n%s\n};' % (name, len(values), '\n'.join(lines))


if __name__ == '__main__':
    for name, rows in SPRITES.items():
        print(to_array(name, rows))
        print()
