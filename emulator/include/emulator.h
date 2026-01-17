#ifndef EMULATOR_H
#define EMULATOR_H

#include "processor.h"

// void emulate(const char* rom_path);
void emulate(const char* rom_path, const char* graphics_path);
void render(Processor* p);

#endif
