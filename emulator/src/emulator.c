#include "processor.h"
#include "opcodes.h"



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
			operand = fetch(p, p->pc);
		case OC_SWAPXY:
		case OC_DUPEXY:
		case OC_SWAPXA:
		case OC_DUPEXA:
		case OC_PUSH:
		case OC_POP:
		case OC_LEAP:
		case OC_CALL:
		case OC_RET:            
		case OC_ADD:
		case OC_SUB:
		case OC_MUL:
		case OC_MOD:
		case OC_AND:
		case OC_XOR:
		case OC_OR:
		case OC_NOT:
