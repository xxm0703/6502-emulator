#include <stdint.h>

#ifndef x6502_H
#define x6502_H

#define C 1
#define Z 2
#define I 4
#define D 8
#define B 16
#define V 64
#define N 128

#define cary(x) if(x){cpu->flags |= C;}else{cpu->flags &= ~C;}
#define zero(x) if(x){cpu->flags &= ~Z;}else{cpu->flags |= Z;}
#define nega(x) if(x >> 7){cpu->flags |= N;}else{cpu->flags &= ~N;}
#define ovrf(x) if(x){cpu->flags |= V;}else{cpu->flags &= ~V;}

typedef struct {
        int8_t flags, acc, X, Y;
        uint8_t SP;
        uint16_t PC;
} cpu_t;

typedef struct {
        cpu_t *cpu;
        int8_t *mem, *ins;
} machine_t;

machine_t mach;

void setup_machine(machine_t *);
int run(machine_t *);


static inline uint8_t read(uint16_t pc){
	return mach.ins[pc];
}
static inline uint16_t rel(uint8_t inp){
	return inp + read(inp + 1);
}
static inline uint16_t norm(uint8_t *src, uint16_t inx){
	return src[inx] | src[inx + 1] << 8;
}
static inline uint8_t get(uint16_t abs){
	return mach.mem[abs];
}

static inline uint16_t ind(uint8_t add){
	return norm(mach.mem, norm(mach.ins, add));
}
static inline uint16_t indx(uint8_t zpg){
	return norm(mach.mem, read(zpg) + mach.cpu->X);
}
static inline uint16_t indy(uint8_t zpg){
	return norm(mach.mem, read(zpg)) + mach.cpu->Y;
}

static inline uint16_t zpg(uint8_t zpg){
	return read(zpg);
}
static inline uint16_t zpgx(uint8_t add){
	return zpg(add) + mach.cpu->X;
}
static inline uint16_t zpgy(uint8_t add){
	return zpg(add) + mach.cpu->Y;
}

static inline uint16_t absm(uint8_t abs){
	return norm(mach.ins, abs);
}
static inline uint16_t absx(uint8_t abs){
	return absm(abs) + mach.cpu->X;
}
static inline uint16_t absy(uint8_t abs){
	return absm(abs) + mach.cpu->Y;
}



#endif
