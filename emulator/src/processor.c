#include "processor.h"
#include "quarklib/qprint/qprint.h"


register_size_t* fetch(Processor* p, address_t a){
	if(a < 0x1F00){
		return &(p->mmap.ram[a]);
	}

	if((0x1F00 <= a) && (a <= 0x1FFF)){
		qdebug("$cmfetching %x$0\n");
		if((0x1F80 <= a) && (a <= 0x1F80 + (sizeof(Sprite) * MAX_SPRITE))){
			return &(p->mmap.sprite_data[a-0x1F80]);
		}
		switch(a){
			case 0x1FFF:
				qlog("controller value is %x\n", p->mmap.controller);
				return &(p->mmap.controller);
			default:
				qerror("Unkown SPC special address %x\n", a);
				return 0;
				break;
		
		}
	}

	if((0x2000 <= a) && (a<=0x3FFF)){
		return &(p->mmap.rom[a-0x2000]);
	}
	
	qerror("fetch on non mapped part of memory : 0x%x\n", a);
	return 0;
}

void swapXY(Processor* p){
	p->x ^= p->y;
	p->y ^= p->x;
	p->x ^= p->y;
}

void swapXA(Processor* p){
	p->x ^= p->a;
	p->a ^= p->x;
	p->x ^= p->a;
}


void arithmetic(Processor* p, Operation o){
	switch(o){
		case OP_ADD:
			p->a += p->x;
			break;
		case OP_SUB:
			p->a -= p->x;
			break;
		case OP_MUL:
			p->a *= p->x;
			break;
		case OP_MOD:
			p->a %= p->x;
			break;
		case OP_AND:
			p->a &= p->x;
			break;
		case OP_XOR:
			p->a ^= p->x;
			break;
		case OP_OR:
			p->a |= p->x;
			break;
		case OP_NOT:
			p->a = !(p->a);
			break;
		default:
			qerror("WOOOW THATS NOT A VALID `Operation` %d\n", o);
	}
	p->a = p->a&0x3F;
	qlog("a is now %x\n", p->a);
}

void readm(Processor* p){
	p->x = *fetch(p, p->d);
}
void writem(Processor* p){
	*fetch(p, p->d) = p->x;
}

//for D register
//yes i know im using memory as if its 8 bit because i dont care rn i dont have the time
void save(Processor*p, address_t a){
	*fetch(p, a) = (p->d)&0xFF;
	*fetch(p, a+1) = ((p->d)&0x3FFF) >> 8;
}
void restore(Processor*p, address_t a){
	p->d = 0x0000;
	p->d |= *fetch(p, a);
	p->d |= *fetch(p, a+1)<<8;
	p->d &= 0x3FFF;
}

