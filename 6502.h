#include <stdint.h>

#ifndef 6502_H
#define 6502_H

#define C 1
#define Z 2
#define I 4
#define D 8
#define B 16
#define V 64
#define N 128

#define zero(x) (x ? cpu->flags &= ~Z : cpu->flags |= Z)
#define nega(x) (x >> 7 ? cpu->flags |= N : cpu->flags &= ~N)

typedef struct {
        int8_t flags, acc, X, Y;
        uint8_t SP;
        uint16_t PC;
} cpu_t;

typedef struct {
        cpu_t *cpu;
        int8_t *mem, *ins;
} machine_t;

inline uint16_t ind(uint8_t);
inline uint16_t indx(uint8_t);
inline uint16_t indy(uint8_t);

inline uint16_t zpg(uint8_t);
inline uint16_t zpgx(uint8_t);
inline uint16_t zpgy(uint8_t);

inline uint16_t abs(uint8_t);
inline uint16_t absx(uint8_t);
inline uint16_t absy(uint8_t);

inline uint16_t rel(uint8_t);
inline uint16_t norm(uint8_t *, uint16_t);
inline uint8_t get(uint16_t);
inline uint8_t read(uint16_t);



#endif
