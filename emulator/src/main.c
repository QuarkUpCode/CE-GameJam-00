#include "quarklib/qprint/qprint.h"
// #include <stdio.h>
#include "emulator.h"
#include <unistd.h>

int main(int argc, char** argv){
	// printf("testtesttest\n\n\n\n");
	qprint("fish\n");
	// qprint("a");
	// char* a = "test\n";
	// write(1, a, 2);
	emulate("./game/rom.owo", "./game/graphics.owo");
}

