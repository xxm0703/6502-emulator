#include "6502.h"

int main(int argc, char const *argv[]) {
	FILE *f = fopen(argv[argc-1], "r");
	setup_machine(&mach);
	fread(mach.ins, 1, 10000, f);
	run(&mach);
	printf("%d", (uint8_t)mach.mem[0xFE]);
	return 0;
}
