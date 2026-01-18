#include "config.h"

#include <stdint.h>
#include <stdlib.h>

#include "processor.h"
#include "opcodes.h"

#include "loadfile.h"

#include "quarklib/qprint/qprint.h"
#include <stdio.h>

#include <SDL2/SDL.h>
#include <initsdl.h>
#include "sdlmanager.h"

// SDL_Window* window;
// uint32_t framestart;
// uint32_t frametime;

void step(Processor* p){

	OpCode instruction = *fetch(p, p->pc);
	address_t a;
	register_size_t operand;

	switch(instruction){
		case OC_NOP:
			break;
		case OC_READ:
			readm(p);
			break;
		case OC_WRITE:
			qdebug("writing %x at %x\n", p->x, p->d);
			writem(p);
			break;
		case OC_SAVE:
			a = 0x0000;
			p->pc++;
			a |= *fetch(p, p->pc);
			p->pc++;
			a |= ((*fetch(p, p->pc))&0x3F)<<8;
			save(p, a);
			break;
		case OC_RESTORE:
			a = 0x0000;
			p->pc++;
			a |= *fetch(p, p->pc);
			p->pc++;
			a |= ((*fetch(p, p->pc))&0x3F)<<8;
			restore(p, a);
			break;
		case OC_INCD:
			p->d++;
			break;
		case OC_DECD:
			p->d--;
			break;
		case OC_LDD:
			a = 0x0000;
			p->pc++;
			a |= *fetch(p, p->pc);
			p->pc++;
			a |= ((*fetch(p, p->pc))&0x3F)<<8;
			p->d = a;
			qdebug("loaded %x into d\n", a);
			break;
		case OC_INC:
			p->a++;
			break;
		case OC_DEC:
			p->a--;
			break;
		case OC_LDX:
			operand = 0x00;
			p->pc++;
			operand = (*fetch(p, p->pc))&0x7F;
			// printf("aaa %x\n", operand);
			p->x = operand;
			qdebug("loaded %x into x\n", operand);
			break;
		case OC_SWAPXY:
			swapXY(p);
			break;
		case OC_DUPEXY:
			p->y = p->x;
			break;
		case OC_SWAPXA:
			swapXA(p);
			break;
		case OC_DUPEXA:
			p->a = p->x;
			break;
		case OC_PUSH:
			p->sp--;
			*fetch(p, p->sp) = p->x;
			break;
		case OC_POP:
			p->x = *fetch(p, p->sp);
			p->sp++;
			break;
		case OC_LEAP:
			operand = 0x00;
			p->pc++;
			operand = *fetch(p, p->pc)&0x7F;
			if(((p->a)&0x7F) == 0){
				p->pc += operand;
				qdebug("Leaped forward %d\n", operand);
			}
			else qdebug("Did not leap %d\n", operand);
			// p->pc--;
			break;
		case OC_CALL:
			p->sp--;
			p->sp--;
			p->d = p->pc;	//!!! important, p->d is not concerved accress function calls!!!
			save(p, p->sp);	//not sure about endianness here
			a = 0x0000;
			p->pc++;
			a |= *fetch(p, p->pc);
			p->pc++;
			a |= ((*fetch(p, p->pc))&0x3F)<<8;
			p->d = a;
			p->pc = a;
			qdebug("\n\nCalled %x\n\n", p->pc);
			p->pc--;
			break;
		case OC_RET:
			restore(p, p->sp);
			p->sp++;
			p->sp++;
			p->pc = p->d;
			qdebug("returned to %x\n", p->pc);
			// p->pc--;	//!!! not sure
			p->pc++;
			p->pc++;
			break;
		case OC_ADD:
			arithmetic(p, OP_ADD);
			break;
		case OC_SUB:
			arithmetic(p, OP_SUB);
			break;
		case OC_MUL:
			arithmetic(p, OP_MUL);
			break;
		case OC_MOD:
			arithmetic(p, OP_MOD);
			break;
		case OC_AND:
			arithmetic(p, OP_AND);
			break;
		case OC_XOR:
			arithmetic(p, OP_XOR);
			break;
		case OC_OR:
			arithmetic(p, OP_OR);
			break;
		case OC_NOT:
			arithmetic(p, OP_NOT);
			break;
		default:
			qerror("Unkown OpCode : %d\n", instruction);
			break;
	}
	p->pc++;
}


void printram(Processor* p){
	const int width = 16;
	const int LINES = 4;
	qprint("----\n");
	// for(int y=0; y<(0x1EFF / width); y++){
	for(int y=0; y<LINES; y++){
		for(int x=0; x<width; x++){
			qprint("%d ", *fetch(p, 0x0000 + (width*y) + x));
		}
		qprint("\n");
	}
	qprint("----\n");
}


uint8_t sample_texture(Processor* p, int index, int x, int y){
	uint8_t* texture = &(p->mmap.chr[index*(TILESIZE*TILESIZE)]);
	uint8_t c = texture[(y*TILESIZE) + x];
	// return p->mmap.col[]
	// qdebug("c = %d\n", c);
	return c;
	// return 0;
}

void render(Processor* p){
	qlog("RENDER START\n");
	int spritecount = p->mmap.spritecount;
	Sprite s;
	uint8_t* color;
	for(int y=0; y<TILEMAP_Y*TILESIZE; y++){
		for(int x=0; x<TILEMAP_X*TILESIZE; x++){
			p->mmap.screen[(y*TILEMAP_X*TILESIZE) + x] = 0xFF000000;
		}
	}
	for(int i=0; i<spritecount; i++){
		s.x = p->mmap.sprite_data[(sizeof(Sprite)*i) + 0];
		s.y = p->mmap.sprite_data[(sizeof(Sprite)*i) + 1];
		s.n = p->mmap.sprite_data[(sizeof(Sprite)*i) + 2];
		s.c = p->mmap.sprite_data[(sizeof(Sprite)*i) + 3];
		qvald(s.x);
		qvald(s.y);
		qvald(s.c);
		qvald(s.n);
		for(int dy=0; dy<TILESIZE; dy++){						//whoever is reading this i am so so sorry
			for(int dx=0; dx<TILESIZE; dx++){
				if((2*s.x)+dx < 0 || (TILESIZE*TILEMAP_X) <= (2*s.x)+dx || (2*s.y)+dy < 0 || (TILESIZE*TILEMAP_Y) <= (2*s.y)+dy) continue;
				color = &(p->mmap.col[(PALETTE_SIZE*s.c*4) + (sample_texture(p, s.n, dx, dy)*4)]);
				if(color[3] != 0) p->mmap.screen[(((2*s.y)+dy)*TILEMAP_X*TILESIZE) + (2*s.x) + dx] = (color[3]<<24) | (color[0]<<16) | (color[1]<<8) | color[2];
				// p->mmap.screen[(((2*s.y)+dy)*TILEMAP_X*TILESIZE) + (2*s.x) + dx] = 0xffFF44FF;
				// p->mmap.screen[(((2*s.y)+dy)*TILEMAP_X*TILESIZE) + (2*s.x) + dx] |= 0xff0000F0;
			}
		}
	
	}
	qlog("RENDER END\n");

	for(int y=0; y<HEIGHT; y++){
		for(int x=0; x<WIDTH; x++){
			// ((uint32_t*)SDL_GetWindowSurface(p->sdl.window)->pixels)[(y*WIDTH) + x] = 0xFFFFFF00; //ARGB
			((uint32_t*)SDL_GetWindowSurface(p->sdl.window)->pixels)[(y*WIDTH) + x] = p->mmap.screen[((y/SCALEFACTOR) * TILEMAP_X * TILESIZE) + (x/SCALEFACTOR)];
		}
	}

	m_endFrame(p->sdl.window, &p->sdl.framestart, &p->sdl.frametime);	
}

void emulate(const char* rom_path, const char* graphics_path){

	uint8_t* rom = (uint8_t*)loadfile(rom_path);
	uint8_t* graphics_rom = (uint8_t*)loadfile(graphics_path);
	uint32_t* screen = malloc((TILEMAP_X*TILESIZE*TILEMAP_Y*TILESIZE)*sizeof(uint32_t));

	Processor processor = {0, 0, 0, 0, 0, 0, (MemoryMap){0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

	// processor.sdl.window = initSDL("silly emulator");
	processor.sdl.window = initSDL(WINDOWNAME);

	processor.mmap.ram = malloc(0x1EFF * sizeof(uint8_t));
	for(int i=0; i<0x1EFF; i++) processor.mmap.ram[i] = 0x00;
	processor.mmap.rom = rom;

	processor.mmap.chr = &(graphics_rom[PALETTE_SIZE*PALETTE_COUNT*sizeof(uint32_t)]);
	processor.mmap.bg0 = NULL;
	processor.mmap.bg1 = NULL;
	processor.mmap.col = graphics_rom;
	processor.mmap.sprite_data = malloc(sizeof(Sprite) * MAX_SPRITE);
	for(int i=0; i<MAX_SPRITE*sizeof(Sprite); i++) processor.mmap.sprite_data[i] = 0x00;

	processor.mmap.controller = 0b00000000;
	processor.mmap.spritecount = 0;

	processor.mmap.screen = screen;
	
	processor.pc = 0x2000;
	int i = 0;
	int j = 0;
	// while(j<(1+2)){
	uint8_t quit = 0;
	uint8_t* keyboardstate;
	while(!quit){
		quit = m_handleInput(&keyboardstate);	//!!! all o this shouldn't be in here, i absolutely have to refactor this shit
		processor.mmap.controller = 0x00;
		if(m_getkey(keyboardstate, 'w')) processor.mmap.controller |= 0x40;
		if(m_getkey(keyboardstate, 's')) processor.mmap.controller |= 0x20;
		if(m_getkey(keyboardstate, 'a')) processor.mmap.controller |= 0x10;
		if(m_getkey(keyboardstate, 'd')) processor.mmap.controller |= 0x08;
		qprint("controller : %x\n", processor.mmap.controller);
		printf("pc : %x\n", processor.pc);
		step(&processor);
		printram(&processor);
		i++;
		// if(i>=64) break;
		if(processor.pc == 0x2003){
			qlog("i = %d\n", i);
			j++;
		}
	}
}



