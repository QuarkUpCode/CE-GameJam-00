#!/bin/zsh

python3 compiler/graphics.py

cd emulator/
make clean
make
cd ..

python3 compiler/compile.py
echo ""
hexdump -C game/rom.owo

