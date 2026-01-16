#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>
#include <sys/types.h>

//its gonna be a 7bit console
typedef uint8_t register_size_t;
// 14bit address
typedef uint16_t address_t;

typedef struct {

	register_size_t x;	// aaaa bbb, A indicates tile, B indicates subtile 2px displacement (or simply placed at 2px * x)
	register_size_t y;
	register_size_t n;	//which graphics from chr to use
	register_size_t c;	//which color palette to use

} Sprite;

typedef struct{

	register_size_t* ram;	//holds stack from $6942 to $68C2 (no that cant happen with 14 but address :despair:)
	register_size_t* rom;

	/* should have a dedicated PPU section if i have time */

	register_size_t* chr;	//where sprite data will go
	register_size_t* col;	//where the 6 color palettes will go
	register_size_t* bg0;
	register_size_t* bg1;

} MemoryMap;

typedef struct {

	register_size_t x;	//main register
	register_size_t y;
	register_size_t a;	//accumulator
	register_size_t sp;	//stack pointer
	address_t pc;	//program counter
	address_t d;	//address register, all read/write use the value here as address

	MemoryMap mmap;

} Processor;

typedef enum {
	
	OP_ADD=1,
	OP_SUB,
	OP_MUL,
	OP_MOD,


} Operation;

void swapXY(Processor* p);
void arithmetic(Processor* p, Operation o);	//always uses A in place and X as operand (A = A + X, A = A - X, ...)

//Uses D register as address
register_t load(Processor* p);
void write(Processor* p);

//for D register
void save(Processor*p, address_t a);
void restore(Processor*p, address_t a);

#endif
