#ifndef OPCODES_H
#define OPCODES_H

typedef enum {

	OC_NOP=0,
	OC_READ,	// x = mem[d]
	OC_WRITE,	// mem[d] = x
	OC_SAVE,	// lsb(addr) = d
	OC_RESTORE,	// d = lsb(addr)
	OC_INCD,	// d++
	OC_DECD,	// d--
	OC_LDD,		// d = n
	OC_INC,		// a++
	OC_DEC,		// a--
	OC_LDX,		// x = n
	OC_SWAPXY,	// x <-> y
	OC_DUPEXY,	// y = x
	OC_SWAPXA,	// x <-> a
	OC_DUPEXA,	// a = x
	OC_PUSH,	// push x
	OC_POP,		// pop into x
	OC_LEAP,	// will add n to pc iif a == 0
	OC_CALL,	// push PC in lsb and jump to
	OC_RET,		// pop PC in lsb from stack

	OC_ADD,		// a += x
	OC_SUB,		// a -= x
	OC_MUL,		// a *= x
	OC_MOD,		// a %= x

} OpCode;

#endif
