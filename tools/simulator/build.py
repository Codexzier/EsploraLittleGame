#!/usr/bin/env python3
# ========================================================================================
# Description:       Baut den PC Simulator und spielt den Testablauf durch.
#                    Die .ino Dateien werden wie von der Arduino IDE zusammengefuegt,
#                    Funktions Prototypen werden erzeugt und alles mit g++ uebersetzt.
#
# Aufruf:            python3 tools/simulator/build.py <Pfad zur TFT Bibliothek>
#                    z.B. python3 tools/simulator/build.py ~/Arduino/libraries/TFT
#
# Ergebnis:          tools/simulator/out/*.png (Bildschirmfotos, 3-fach vergroessert)
# ========================================================================================

import glob
import os
import re
import struct
import subprocess
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
SKETCH = os.path.abspath(os.path.join(HERE, '..', '..'))
BUILD = os.path.join(HERE, 'build')
OUT = os.path.join(HERE, 'out')
SCALE = 3


def find_font(tft_dir):
    for path in glob.glob(os.path.join(tft_dir, '**', 'glcdfont.c'), recursive=True):
        body = re.search(r'font\[\]\s*PROGMEM\s*=\s*\{(.*?)\};', open(path).read(), re.S)
        if body:
            return body.group(1)
    sys.exit('glcdfont.c nicht gefunden in ' + tft_dir)


def sketch_sources():
    main = os.path.join(SKETCH, 'EsploraLittleGame.ino')
    others = sorted(p for p in glob.glob(os.path.join(SKETCH, '*.ino')) if p != main)
    return [main] + others


def prototypes(code):
    result = []
    pattern = re.compile(r'^((?:const\s+)?[A-Za-z_]\w*\*?)\s+(\**\w+)\(([^)]*)\)\s*\{', re.M)
    for m in pattern.finditer(code):
        if m.group(2) in ('if', 'for', 'while', 'switch'):
            continue
        result.append('%s %s(%s);' % (m.group(1), m.group(2), m.group(3)))
    return result


def write_png(ppm_path, png_path):
    data = open(ppm_path, 'rb').read()
    header, pixels = data.split(b'\n', 3)[:3], data.split(b'\n', 3)[3]
    width, height = [int(v) for v in header[1].split()]
    rows = []
    for y in range(height):
        row = pixels[y * width * 3:(y + 1) * width * 3]
        wide = b''.join(row[x * 3:x * 3 + 3] * SCALE for x in range(width))
        rows.extend([b'\x00' + wide] * SCALE)

    def chunk(kind, body):
        return struct.pack('>I', len(body)) + kind + body + struct.pack('>I', zlib.crc32(kind + body) & 0xffffffff)

    png = b'\x89PNG\r\n\x1a\n'
    png += chunk(b'IHDR', struct.pack('>IIBBBBB', width * SCALE, height * SCALE, 8, 2, 0, 0, 0))
    png += chunk(b'IDAT', zlib.compress(b''.join(rows), 9))
    png += chunk(b'IEND', b'')
    open(png_path, 'wb').write(png)


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__ or 'Aufruf: build.py <Pfad zur TFT Bibliothek>')

    os.makedirs(BUILD, exist_ok=True)
    os.makedirs(OUT, exist_ok=True)

    code = ''
    for path in sketch_sources():
        text = open(path).read()
        text = re.sub(r'^#include\s*<(SPI|TFT|Esplora|avr/pgmspace)\.h>.*$', '', text, flags=re.M)
        code += '\n// ---- %s ----\n#line 1 "%s"\n%s\n' % (os.path.basename(path), path, text)

    game = os.path.join(BUILD, 'game_all.cpp')
    with open(game, 'w') as f:
        f.write('#include "%s"\n' % os.path.join(HERE, 'esplora_mock.h'))
        f.write('const unsigned char font[] = {%s};\n' % find_font(sys.argv[1]))
        f.write('MockTFT EsploraTFT;\nMockEsplora Esplora;\nunsigned long gSimMillis = 0;\n')
        f.write('\n'.join(prototypes(code)) + '\n')
        f.write(code)
        f.write('\n#include "%s"\n' % os.path.join(HERE, 'scenario.cpp'))

    binary = os.path.join(BUILD, 'simulator')
    subprocess.check_call(['g++', '-std=c++11', '-Wall', '-Wno-unused-variable', '-O1', '-o', binary, game])

    for old in glob.glob(os.path.join(OUT, '*')):
        os.remove(old)
    status = subprocess.call([binary, OUT])

    for ppm in sorted(glob.glob(os.path.join(OUT, '*.ppm'))):
        write_png(ppm, ppm[:-4] + '.png')
        os.remove(ppm)

    sys.exit(status)


if __name__ == '__main__':
    main()
