#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "processor.h"

typedef struct {

	Processor p;
	SDL_Window* window;
	uint32_t framestart;
	uint32_t frametime;

} Emulator;

// void emulate(const char* rom_path);
void emulate(const char* rom_path, const char* graphics_path);
void render(Emulator* e);

#endif
