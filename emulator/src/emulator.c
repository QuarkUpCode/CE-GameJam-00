#include <stdint.h>
#include <stdlib.h>

#include "processor.h"
#include "opcodes.h"

#include "loadfile.h"

#include "quarklib/qprint/qprint.h"


void step(Processor* p){

	OpCode instruction = *fetch(p, p->pc);
	address_t a;
	register_size_t operand;

	switch(instruction){
		case OC_NOP:
			break;
		case OC_READ:
			read(p);
			break;
		case OC_WRITE:
			write(p);
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
			operand = *fetch(p, p->pc)&0x8F;
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
			operand = *fetch(p, p->pc)&0x8F;
			if(((p->a)&0x8F) == 0){
				p->pc += operand;
				qdebug("Leaped forward %d\n", operand);
			}
			else qdebug("Did not leap %d\n", operand);
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
			qdebug("Called %x\n", p->pc);
			break;
		case OC_RET:
			restore(p, p->sp);
			p->sp++;
			p->sp++;
			p->pc = p->d;
			qdebug("returned to %x\n", p->pc);
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
	for(int y=0; y<(0x1EFF / width); y++){
		for(int x=0; x<width; x++){
			qprint("%2x", *fetch(p, 0x2000 + (width*y) + x));
		}
		qprint("\n");
	}
}

void emulate(const char* rom_path){
	uint8_t* rom = loadfile(rom_path);

	Processor processor = {0, 0, 0, 0, 0, 0, (MemoryMap){0, 0, 0, 0, 0, 0, 0}};
	processor.mmap.ram = malloc(0x1EFF * sizeof(uint8_t));
	processor.mmap.rom = rom;

	processor.mmap.chr = NULL;
	processor.mmap.bg0 = NULL;
	processor.mmap.bg1 = NULL;
	processor.mmap.col = NULL;

	processor.mmap.controller = 0b00000000;
	
	processor.pc = 0x2000;
	int i = 0;
	while(1){
		step(&processor);
		printram(&processor);
		if(i>=256) break;
	}
}



