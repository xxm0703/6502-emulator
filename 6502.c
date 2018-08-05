#include "6502.h"
#include <stdlib.h>

inline uint16_t ind(uint16_t add) return norm(comp.mem, norm(comp.ins, add));
inline uint16_t indx(uint8_t zpg) return norm(comp.mem, read(zpg) + comp.cpu->X);
inline uint16_t indy(uint8_t zpg) return norm(comp.mem, read(zpg)) + comp,cpu->Y;

inline uint16_t zpg(uint8_t zpg) return read(zpg);
inline uint16_t zpgx(uint8_t zpg) return zpg(zpg) + comp.cpu->X;
inline uint16_t zpgy(uint8_t zpg) return zpg(zpg) + comp.cpu->Y;

inline uint16_t abs(int16_t abs) return norm(comp.ins, abs);
inline uint16_t absx(uint16_t abs) return abs(abs) + comp.cpu->X;
inline uint16_t absy(uint16_t abs) return abs(abs) + comp.cpu->Y;

inline uint8_t get(uint16_t abs) return comp.mem[abs];
inline uint16_t rel(uint8_t inp) return inp + read(inp + 1);
inline uint8_t read(uint16_t pc) return comp.ins[pc];
inline uint8_t norm(uint8_t *src, uint16_t inx) return src[inx] | src[inx+1] <<8;

machine_t comp;

int main(){
        comp.mem = (uint8_t *)malloc(64 * 1024 * sizeof(int8_t));
        comp.ins = (uint8_t *)malloc(64 * 1024 * sizeof(int8_t));
        comp.cpu = (cpu_t *)malloc(sizeof(cpu_t));
        // run(&comp)
        return 0;
}

int run(){
        int8_t *mem = comp.mem;
        cpu_t *cpu = comp.cpu;
        uint16_t inp = cpu->PC;
        uint8_t p;
        for(;;) {

                switch(read(inp)) {

                // BRK
                case 0x0:
                        inp += 2;
                        break;

                // ORA
                case 0x1:
                        cpu->acc = get(indx(inp + 1)) | cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x5:
                        cpu->acc = get(zpg(inp + 1)) | cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(p);
                        break;

                // ASL
                case 0x6:
                        p = get(zpg(inp + 1))
                            p >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
                        p <<= 1;
                        zero(p);
                        nega(p);
                        mem[zpg(inp+1)] = p;
                        inp += 2;
                        break;

                // PHP
                case 0x8:
                        cpu->flags |= B;
                        mem[1 << 8 | cpu->SP--] = cpu->flags;
                        inp++;
                        break;

                // ORA
                case 0x9:
                        cpu->acc = *(inp + 1) | cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ASL
                case 0xA:
                        cpu->acc >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
                        cpu->acc <<= 1;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 1;
                        break;

                // ORA
                case 0xD:
                        cpu->acc = get(abs(inp + 1)) | cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ASL
                case 0xE:
                        p = get(abs(inp + 1));
                        p >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
                        p <<= 1;
                        zero(p);
                        nega(p);
                        mem[abs(inp + 1)] = p;
                        inp += 3;
                        break;

                // BPL
                case 0x10:
                        inp = cpu->flags & N ? inp + 2 : rel(inp);
                        break;

                // ORA
                case 0x11:
                        cpu->acc = get(indy(inp + 1)) | cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x15:
                        cpu->acc = get(zpgx(inp + 1)) | cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ASL
                case 0x16:
                        p = get(zpgx(inp+1))
                            p >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
                        p <<= 1;
                        zero(p);
                        nega(p);
                        mem[zpgx(inp+1)] = p;
                        inp += 2;
                        break;

                // CLC
                case 0x18:
                        cpu->flags &= ~C;
                        inp++;
                        break;

                // ORA
                case 0x19:
                        cpu->acc = get(absy(inp + 1)) | cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x1D:
                        cpu->acc = get(absx(inp + 1)) | cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ASL
                case 0x1E:
                        p = get(absx(inp + 1));
                        p >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
                        p <<= 1;
                        zero(p);
                        nega(p);
                        mem[absx(inp+1)] = p;
                        inp += 3;
                        break;

                // JSR
                case 0x20:
                        mem[1 << 8 | cpu->SP--] = (inp+2) >> 8;
                        mem[1 << 8 | cpu->SP--] = (inp+2) & 0xFF;
                        inp = abs(inp+1);
                        break;

                // AND
                case 0x21:
                        cpu->acc = get(indx(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // BIT
                case 0x24:
                        p = get(zpg(inp + 1));
                        nega(p);
                        p & 0x80 ? cpu->flags |= V : cpu->flags &= ~V;
                        zero(cpu->acc & p);
                        inp += 2;
                        break;

                // AND
                case 0x25:
                        cpu->acc = get(zpg(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ROL
                case 0x26:
                        p = get(zpg(inp + 1)) >> 7;
                        mem[zpg(inp + 1)] = mem[zpg(inp + 1)] << 1 | (cpu->flags & C);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 2;
                        break;

                case 0x28:
                        cpu->flags = mem[1 << 8 | cpu->SP++];
                        inp++;
                        break;

                // AND
                case 0x29:
                        cpu->acc = read(inp + 1) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ROL
                case 0x2A:
                        p = cpu->acc >> 7;
                        cpu->acc = cpu->acc << 1 | (cpu->flags & C);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp++;
                        break;

                // BIT
                case 0x2C:
                        p = get(abs(inp + 1));
                        nega(p);
                        p & 0x80 ? cpu->flags |= V : cpu->flags &= ~V;
                        zero(cpu->acc & p);
                        inp += 3;
                        break;

                // AND
                case 0x2D:
                        cpu->acc = get(abs(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ROL
                case 0x2E:
                        p = get(abs(inp + 1)) >> 7;
                        mem[abs(inp + 1)] = mem[abs(inp + 1)] << 1 | (cpu->flags & C);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 3;
                        break;

                // BMI
                case 0x30:
                        inp = cpu->flags & N ? rel(inp) : inp + 2;
                        break;

                // AND
                case 0x31:
                        cpu->acc = get(indy(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x35:
                        cpu->acc = get(zpgx(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ROL
                case 0x36:
                        p = get(zpgx(inp + 1)) >> 7;
                        mem[zpgx(inp + 1)] = mem[zpgx(inp + 1)] << 1 | (cpu->flags & C);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 2;
                        break;

                // SEC
                case 0x38:
                        cpu->flags |= C;
                        break;

                // AND
                case 0x39:
                        cpu->acc = get(absy(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x3D:
                        cpu->acc = get(absx(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // ROL
                case 0x3E:
                        p = get(absx(inp + 1)) >> 7;
                        mem[absx(inp + 1)] = mem[absx(inp + 1)] << 1 | (cpu->flags & C);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 3;
                        break;

                // RTI
                case 0x40:
                        cpu->flags = get(1 << 8 | cpu->SP++);
                        inp = get(1 << 8 | cpu->SP++) << 8 | get(1 << 8 | cpu->SP++);
                        inp++;
                        break;

                // EOR
                case 0x41:
                        cpu->acc = get(indx(inp + 1)) ^ cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x45:
                        cpu->acc = get(zpg(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // LSR
                case 0x46:
                        p = get(zpg(inp + 1));
                        cpu->flags |= (p & 1);
                        p >>= 1;
                        zero(p);
                        mem[zpg(inp+1)] = p;
                        inp += 2;
                        break;

                // PHA
                case 0x48:
                        mem[cpu->SP--] = cpu->acc;
                        inp++;
                        break;

                // EOR
                case 0x49:
                        cpu->acc = get(read(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // LSR
                case 0x4A:
                        cpu->flags |= (cpu->acc & 1);
                        cpu->acc >>= 1;
                        zero(cpu->acc);
                        inp++;
                        break;

                // JMP
                case 0x4C:
                        inp = abs(inp + 1);
                        break;

                // EOR
                case 0x4D:
                        cpu->acc = get(abs(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // LSR
                case 0x4E:
                        p = get(abs(inp + 1));
                        cpu->flags |= (p & 1);
                        p >>= 1;
                        zero(p);
                        mem[abs(inp+1)] = p;
                        inp += 3;
                        break;

                // BVC
                case 0x50:
                        inp = cpu->flags & V ? inp + 2 : rel(inp);
                        break;

                // EOR
                case 0x51:
                        cpu->acc = get(indy(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x55:
                        cpu->acc = get(zpgx(inp + 1)) & cpu->acc;
                        inp += 2;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // LSR
                case 0x56:
                        p = get(zpgx(inp + 1));
                        cpu->flags |= (p & C);
                        p >>= 1;
                        zero(p);
                        mem[zpgx(inp+1)] = p;
                        inp += 2;
                        break;

                // CLI
                case 0x58:
                        cpu->flags &= ~I;
                        break;

                // EOR
                case 0x59:
                        cpu->acc = get(absy(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                case 0x5D:
                        cpu->acc = get(absx(inp + 1)) & cpu->acc;
                        inp += 3;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        break;

                // LSR
                case 0x5E:
                        p = get(absx(inp + 1));
                        cpu->flags |= (p & 1);
                        p >>= 1;
                        zero(p);
                        mem[absx(inp+1)] = p;
                        inp += 3;
                        break;

                // RTS
                case 0x60:
                        inp = get(1 << 8 | cpu->SP++) << 8 | get(1 << 8 | cpu->SP++);
                        break;

                // ADC
                case 0x61:
                        p = ((uint16_t)(cpu->acc) + get(indx(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(indx(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 2;
                        break;

                case 0x65:
                        p = ((uint16_t)(cpu->acc) + get(zpg(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(zpg(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 2;
                        break;

                // ROR
                case 0x66:
                        p = get(zpg(inp + 1)) & 1;
                        mem[zpg(inp + 1)] = mem[zpg(inp + 1)] >> 1 | (cpu->flags << 7);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 2;
                        break;

                case 0x68:

                        break;

                // ADC
                case 0x69:
                        p = ((uint16_t)(cpu->acc) + read(inp + 1) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + read(inp + 1) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 2;
                        break;

                case 0x6A:
                        p = cpu->acc & 1;
                        cpu->acc = cpu->acc >> 1 | (cpu->flags << 7);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 1;

                        break;

                // JMP
                case 0x6C:
                        inp = ind(inp + 1);
                        break;

                // ADC
                case 0x6D:
                        p = ((uint16_t)(cpu->acc) + get(abs(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(abs(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 3;
                        break;

                // ROR
                case 0x6E:
                        p = get(abs(inp + 1)) & 1;
                        mem[abs(inp + 1)] = mem[abs(inp + 1)] >> 1 | (cpu->flags << 7);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 3;
                        break;

                case 0x70:

                        break;

                // ADC
                case 0x71:
                        p = ((uint16_t)(cpu->acc) + get(indy(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(indy(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 2;
                        break;

                case 0x75:
                        p = ((uint16_t)(cpu->acc) + get(zpgx(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(zpgx(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 2;
                        break;

                // ROR
                case 0x76:
                        p = get(zpgx(inp + 1)) & 1;
                        mem[zpgx(inp + 1)] = mem[zpgx(inp + 1)] >> 1 | (cpu->flags << 7);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 2;
                        break;

                case 0x78:

                        break;

                // ADC
                case 0x79:
                        p = ((uint16_t)(cpu->acc) + get(absy(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(absy(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 3;
                        break;

                case 0x7D:
                        p = ((uint16_t)(cpu->acc) + get(absx(inp + 1)) + cpu->flags & C) >> 8;
                        cpu->acc = (uint16_t)(cpu->acc) + get(absx(inp + 1)) + cpu->flags & C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        inp += 3;
                        break;

                // ROR
                case 0x7E:
                        p = get(absx(inp + 1)) & 1;
                        mem[absx(inp + 1)] = mem[absx(inp + 1)] >> 1 | (cpu->flags << 7);
                        p ? cpu->flags |= C : cpu->flags &= ~C;
                        zero(cpu->acc);
                        nega(cpu->acc);
                        inp += 3;
                        break;

                case 0x81:

                        break;

                case 0x84:

                        break;

                case 0x85:

                        break;

                case 0x86:

                        break;

                case 0x88:

                        break;

                case 0x8A:

                        break;

                case 0x8C:

                        break;

                case 0x8D:

                        break;

                case 0x8E:

                        break;

                case 0x90:

                        break;

                case 0x91:

                        break;

                case 0x94:

                        break;

                case 0x95:

                        break;

                case 0x96:

                        break;

                case 0x98:

                        break;

                case 0x99:

                        break;

                case 0x9A:

                        break;

                case 0x9D:

                        break;

                case 0xA0:

                        break;

                case 0xA1:

                        break;

                case 0xA2:

                        break;

                case 0xA4:

                        break;

                case 0xA5:

                        break;

                case 0xA6:

                        break;

                case 0xA8:

                        break;

                case 0xA9:

                        break;

                case 0xAA:

                        break;

                case 0xAC:

                        break;

                case 0xAD:

                        break;

                case 0xAE:

                        break;

                case 0xB0:

                        break;

                case 0xB1:

                        break;

                case 0xB4:

                        break;

                case 0xB5:

                        break;

                case 0xB6:

                        break;

                case 0xB8:

                        break;

                case 0xB9:

                        break;

                case 0xBA:

                        break;

                case 0xBC:

                        break;

                case 0xBD:

                        break;

                case 0xBE:

                        break;

                case 0xC0:

                        break;

                case 0xC1:

                        break;

                case 0xC4:

                        break;

                case 0xC5:

                        break;

                case 0xC6:

                        break;

                case 0xC8:

                        break;

                case 0xC9:

                        break;

                case 0xCA:

                        break;

                case 0xCC:

                        break;

                case 0xCD:

                        break;

                case 0xCE:

                        break;

                case 0xD0:

                        break;

                case 0xD1:

                        break;

                case 0xD5:

                        break;

                case 0xD6:

                        break;

                case 0xD8:

                        break;

                case 0xD9:

                        break;

                case 0xDD:

                        break;

                case 0xDE:

                        break;

                case 0xE0:

                        break;

                case 0xE1:

                        break;

                case 0xE4:

                        break;

                case 0xE5:

                        break;

                case 0xE6:

                        break;

                case 0xE8:

                        break;

                case 0xE9:

                        break;

                case 0xEA:

                        break;

                case 0xEC:

                        break;

                case 0xED:

                        break;

                case 0xEE:

                        break;

                case 0xF0:

                        break;

                case 0xF1:

                        break;

                case 0xF4:

                        break;

                case 0xF5:

                        break;

                case 0xF6:

                        break;

                case 0xF8:

                        break;

                case 0xF9:

                        break;

                case 0xFD:

                        break;

                case 0xFE:

                        break;
                }



        }
}
