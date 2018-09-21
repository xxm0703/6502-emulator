#include "6502.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
	FILE *f = fopen(argv[argc-1], "r");
	char c;
	fread(mach.ins, 1, 65, f);
	printf("%s", mach.ins);
	// mach.inst =
	return 0;
}
