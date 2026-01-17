#!/bin/zsh

cd emulator/
make
cd ..

python3 compiler/compile.py
echo ""
hexdump -C game/rom.owo

