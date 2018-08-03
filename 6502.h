#ifndef 6502_H
#define 6502_H

#define N 1
#define Z 2
#define C 4
#define I 8
#define D 16
#define V 32


typedef struct{
  int8_t flags, acc, X, Y;
} cpu_t;

typedef struct{
  cpu_t *cpu;
  int8_t *mem;
} machine_t;

inline uint16_t ind(uint8_t, int8_t);
inline uint16_t zpg(uint8_t);
inline uint16_t abs(uint16_t);



#endif
