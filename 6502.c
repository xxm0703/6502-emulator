#include "6502.h"
#include <stdlib.h>

inline uint16_t ind(uint16_t add) return norm(comp.mem, norm(comp.ins, add));
inline uint16_t indx(uint8_t zpg) return norm(comp.mem, read(zpg) + comp.cpu->X);
inline uint16_t indy(uint8_t zpg) return norm(comp.mem, read(zpg)) + comp, cpu->Y;

inline uint16_t zpg(uint8_t zpg) return read(zpg);
inline uint16_t zpgx(uint8_t zpg) return zpg(zpg) + comp.cpu->X;
inline uint16_t zpgy(uint8_t zpg) return zpg(zpg) + comp.cpu->Y;

inline uint16_t abs(int16_t abs) return norm(comp.ins, abs);
inline uint16_t absx(uint16_t abs) return abs(abs) + comp.cpu->X;
inline uint16_t absy(uint16_t abs) return abs(abs) + comp.cpu->Y;

inline uint8_t get(uint16_t abs) return comp.mem[abs];
inline uint16_t rel(uint8_t inp) return inp + read(inp + 1);
inline uint8_t read(uint16_t pc) return comp.ins[pc];
inline uint8_t norm(uint8_t *src, uint16_t inx) return src[inx] | src[inx + 1] << 8;

machine_t comp;

int main() {
    comp.mem = (uint8_t *) malloc(64 * 1024 * sizeof(int8_t));
    comp.ins = (uint8_t *) malloc(64 * 1024 * sizeof(int8_t));
    comp.cpu = (cpu_t *) malloc(sizeof(cpu_t));
    // run(&comp)
    return 0;
}

int run() {
    int8_t *mem = comp.mem;
    cpu_t *cpu = comp.cpu;
    uint16_t inp = cpu->PC;
	union{
		uint16_t as_16;
		uint8_t as_8;
	} p;
    for (;;) {

        switch (read(inp)) {

            // BRK
        case 0x0:
			cpu->flags |= I;
            mem[1 << 8 | cpu->SP--] = (inp + 2) >> 8;
			mem[1 << 8 | cpu->SP--] = (inp + 2) & 0xFF;
            mem[1 << 8 | cpu->SP--] = cpu->flags;
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
            nega(cpu->acc);
            break;

            // ASL
        case 0x6:
            p.as_8 = get(zpg(inp + 1))
            p.as_8 >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
            p.as_8 <<= 1;
            zero(p.as_8);
            nega(p.as_8);
            mem[zpg(inp + 1)] = p.as_8;
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
            cpu->acc = read(inp + 1) | cpu->acc;
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
            p.as_8 = get(abs(inp + 1));
            p.as_8 >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
            p.as_8 <<= 1;
            zero(p.as_8);
            nega(p.as_8);
            mem[abs(inp + 1)] = p.as_8;
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
            p.as_8 = get(zpgx(inp + 1))
            p.as_8 >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
            p.as_8 <<= 1;
            zero(p.as_8);
            nega(p.as_8);
            mem[zpgx(inp + 1)] = p.as_8;
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
            p.as_8 = get(absx(inp + 1));
            p.as_8 >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
            p.as_8 <<= 1;
            zero(p.as_8);
            nega(p.as_8);
            mem[absx(inp + 1)] = p.as_8;
            inp += 3;
            break;

            // JSR
        case 0x20:
            mem[1 << 8 | cpu->SP--] = (inp + 2) >> 8;
			mem[1 << 8 | cpu->SP--] = (inp + 2) & 0xFF;
            inp = abs(inp + 1);
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
            p.as_8 = get(zpg(inp + 1));
            nega(p.as_8);
            p.as_8 & 0x80 ? cpu->flags |= V : cpu->flags &= ~V;
            zero(cpu->acc & p.as_8);
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
            p.as_8 = get(zpg(inp + 1)) >> 7;
            mem[zpg(inp + 1)] = mem[zpg(inp + 1)] << 1 | (cpu->flags & C);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 2;
            break;

			// PLP
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
            p.as_8 = cpu->acc >> 7;
            cpu->acc = cpu->acc << 1 | (cpu->flags & C);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp++;
            break;

            // BIT
        case 0x2C:
            p.as_8 = get(abs(inp + 1));
            nega(p.as_8);
            p.as_8 & 0x80 ? cpu->flags |= V : cpu->flags &= ~V;
            zero(cpu->acc & p.as_8);
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
            p.as_8 = get(abs(inp + 1)) >> 7;
            mem[abs(inp + 1)] = mem[abs(inp + 1)] << 1 | (cpu->flags & C);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
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
            p.as_8 = get(zpgx(inp + 1)) >> 7;
            mem[zpgx(inp + 1)] = mem[zpgx(inp + 1)] << 1 | (cpu->flags & C);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
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
            p.as_8 = get(absx(inp + 1)) >> 7;
            mem[absx(inp + 1)] = mem[absx(inp + 1)] << 1 | (cpu->flags & C);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 3;
            break;

            // RTI
        case 0x40:
            cpu->flags = get(1 << 8 | cpu->SP++);
            inp = get(1 << 8 | cpu->SP + 1) | get(1 << 8 | cpu->SP + 2) << 8;
			cpu->SP += 2;
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
            p.as_8 = get(zpg(inp + 1));
            cpu->flags |= (p.as_8 & 1);
            p.as_8 >>= 1;
            zero(p.as_8);
            mem[zpg(inp + 1)] = p.as_8;
            inp += 2;
            break;

            // PHA
        case 0x48:
            mem[1 << 8 | cpu->SP--] = cpu->acc;
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
            p.as_8 = get(abs(inp + 1));
            cpu->flags |= (p.as_8 & 1);
            p.as_8 >>= 1;
            zero(p.as_8);
            mem[abs(inp + 1)] = p.as_8;
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
            p.as_8 = get(zpgx(inp + 1));
            cpu->flags |= (p.as_8 & C);
            p.as_8 >>= 1;
            zero(p.as_8);
            mem[zpgx(inp + 1)] = p.as_8;
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
            p.as_8 = get(absx(inp + 1));
            cpu->flags |= (p.as_8 & 1);
            p.as_8 >>= 1;
            zero(p.as_8);
            mem[absx(inp + 1)] = p.as_8;
            inp += 3;
            break;

            // RTS
        case 0x60:
            inp = get(1 << 8 | cpu->SP + 1) | get(1 << 8 | cpu->SP + 2) << 8;
			cpu->SP += 2;
			inp++;
            break;

            // ADC
        case 0x61:
            p.as_8 = ((uint16_t)(cpu->acc) + get(indx(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(indx(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 2;
            break;

        case 0x65:
            p.as_8 = ((uint16_t)(cpu->acc) + get(zpg(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(zpg(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 2;
            break;

            // ROR
        case 0x66:
            p.as_8 = get(zpg(inp + 1)) & 1;
            mem[zpg(inp + 1)] = mem[zpg(inp + 1)] >> 1 | (cpu->flags << 7);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 2;
            break;

            // PLA
        case 0x68
			cpu->acc = mem[1 << 8 | cpu->SP++];
			zero(cpu->acc);
			nega(cpu->acc);
			inp++;
        break;

        // ADC
        case 0x69:
            p.as_8 = ((uint16_t)(cpu->acc) + read(inp + 1) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + read(inp + 1) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 2;
            break;

        case 0x6A:
            p.as_8 = cpu->acc & 1;
            cpu->acc = cpu->acc >> 1 | (cpu->flags << 7);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
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
            p.as_8 = ((uint16_t)(cpu->acc) + get(abs(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(abs(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 3;
            break;

            // ROR
        case 0x6E:
            p.as_8 = get(abs(inp + 1)) & 1;
            mem[abs(inp + 1)] = mem[abs(inp + 1)] >> 1 | (cpu->flags << 7);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 3;
            break;

			// BVS
        case 0x70:
			inp = cpu->flags & V ? rel(inp) : inp + 2;
            break;

            // ADC
        case 0x71:
            p.as_8 = ((uint16_t)(cpu->acc) + get(indy(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(indy(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 2;
            break;

        case 0x75:
            p.as_8 = ((uint16_t)(cpu->acc) + get(zpgx(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(zpgx(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 2;
            break;

            // ROR
        case 0x76:
            p.as_8 = get(zpgx(inp + 1)) & 1;
            mem[zpgx(inp + 1)] = mem[zpgx(inp + 1)] >> 1 | (cpu->flags << 7);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 2;
            break;

			// SEI
        case 0x78:
			cpu->flags |= I;
			inp++;
            break;

            // ADC
        case 0x79:
            p.as_8 = ((uint16_t)(cpu->acc) + get(absy(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(absy(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 3;
            break;

        case 0x7D:
            p.as_8 = ((uint16_t)(cpu->acc) + get(absx(inp + 1)) + cpu->flags & C) >> 8;
            cpu->acc = (uint16_t)(cpu->acc) + get(absx(inp + 1)) + cpu->flags & C;
            zero(cpu->acc);
            nega(cpu->acc);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            inp += 3;
            break;

            // ROR
        case 0x7E:
            p.as_8 = get(absx(inp + 1)) & 1;
            mem[absx(inp + 1)] = mem[absx(inp + 1)] >> 1 | (cpu->flags << 7);
            p.as_8 ? cpu->flags |= C : cpu->flags &= ~C;
            zero(cpu->acc);
            nega(cpu->acc);
            inp += 3;
            break;

			// STA
        case 0x81:
			mem[indx(inp + 1)] = cpu->acc;
			inp += 2;
            break;

			// STY
        case 0x84:
			mem[zpg(inp + 1)] = cpu->Y;
			inp += 2;
            break;

			// STA
        case 0x85:
			mem[zpg(inp + 1)] = cpu->acc;
			inp += 2;
            break;

			// STX
        case 0x86:
			mem[zpg(inp + 1)] = cpu->X;
			inp += 2;
            break;

			// DEY
        case 0x88:
			cpu->Y--;
			nega(cpu->Y);
			zero(cpu->Y);
			inp++;
            break;

			// TXA
        case 0x8A:
			cpu->acc = cpu->X;
			nega(cpu->X);
			zero(cpu->X);
			inp++;
            break;

			// STY
        case 0x8C:
			mem[abs(inp + 1)] = cpu->Y;
			inp += 3;
            break;

			// STA
        case 0x8D:
			mem[abs(inp + 1)] = cpu->acc;
			inp += 3;
            break;

			// STX
        case 0x8E:
			mem[abs(inp + 1)] = cpu->X;
			inp += 3;
            break;

			// BCC
        case 0x90:
			inp = cpu->flags & C ? inp + 2 : rel(inp);
            break;

			// STA
        case 0x91:
			mem[indy(inp + 1)] = cpu->acc;
			inp += 2;
            break;

			// STY
        case 0x94:
			mem[zpgx(inp + 1)] = cpu->Y;
			inp += 2;
            break;

			// STA
        case 0x95:
			mem[zpgx(inp + 1)] = cpu->acc;
			inp += 2;
            break;

			// STX
        case 0x96:
			mem[zpgy(inp + 1)] = cpu->X;
			inp += 2;
            break;

			// TYA
        case 0x98:
			cpu->acc = cpu->Y;
			nega(cpu->acc);
			zero(cpu->acc);
			inp++;
            break;

			// STA
        case 0x99:
			mem[absy(inp + 1)] = cpu->acc;
			inp += 3;
            break;

			// TXS
        case 0x9A:
			cpu->SP = cpu->X;
			inp++;
            break;

			// STA
        case 0x9D:
			mem[absx(inp + 1)] = cpu->acc;
			inp += 3;
            break;

			// LDY
        case 0xA0:
			cpu->Y = read(inp + 1);
			nega(cpu->Y);
			zero(cpu->Y);
			inp += 2;
            break;

			// LDA
        case 0xA1:
			cpu->acc = get(indx(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 2;
            break;

			// LDX
        case 0xA2:
			cpu->X = read(inp + 1);
			nega(cpu->X);
			zero(cpu->X);
			inp += 2;
            break;

			// LDY
        case 0xA4:
			cpu->Y = get(zpg(inp + 1));
			nega(cpu->Y);
			zero(cpu->Y);
			inp += 2;
            break;

			// LDA
        case 0xA5:
			cpu->acc = get(zpg(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 2;
            break;

			// LDX
        case 0xA6:
			cpu->X = get(zpg(inp + 1));
			nega(cpu->X);
			zero(cpu->X);
			inp += 2;
            break;

			// TAY
        case 0xA8:
			cpu->Y = cpu->acc;
			zero(cpu->acc);
			nega(cpu->acc);
			inp++;
            break;

			// LDA
        case 0xA9:
			cpu->acc = read(inp + 1);
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 2;
            break;

			// TAX
        case 0xAA:
			cpu->X = cpu->acc;
			zero(cpu->acc);
			nega(cpu->acc);
			inp++;
            break;

			// LDY
        case 0xAC:
			cpu->Y = get(abs(inp + 1));
			nega(cpu->Y);
			zero(cpu->Y);
			inp += 3;
            break;

			// LDA
        case 0xAD:
			cpu->acc = get(abs(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 3;
			break;

			// LDX
        case 0xAE:
			cpu->X = get(abs(inp + 1));
			nega(cpu->X);
			zero(cpu->X);
			inp += 3;
            break;

			// BCS
        case 0xB0:
            inp = cpu->flags & C ? rel(inp) : inp + 2;
            break;

			// LDA
        case 0xB1:
			cpu->acc = get(indy(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 2;
            break;

			// LDY
        case 0xB4:
			cpu->Y = get(zpgx(inp + 1));
			nega(cpu->Y);
			zero(cpu->Y);
			inp += 2;
            break;

			// LDA
        case 0xB5:
			cpu->acc = get(zpgx(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 2;
            break;

			// LDX
        case 0xB6:
			cpu->X = get(zpgy(inp + 1));
			nega(cpu->X);
			zero(cpu->X);
			inp += 2;
            break;

			// CLV
        case 0xB8:
			cpu->flags &= ~V;
			inp++;
            break;

			// LDA
        case 0xB9:
			cpu->acc = get(absy(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 3;
            break;

			// TSX
        case 0xBA:
			cpu->X = cpu->SP;
			inp++;
            break;

			// LDY
        case 0xBC:
			cpu->Y = get(absx(inp + 1));
			nega(cpu->Y);
			zero(cpu->Y);
			inp += 3;
            break;

			// LDA
        case 0xBD:
			cpu->acc = get(absx(inp + 1));
			nega(cpu->acc);
			zero(cpu->acc);
			inp += 3;
            break;

			// LDX
        case 0xBE:
			cpu->X = get(absy(inp + 1));
			nega(cpu->X);
			zero(cpu->X);
			inp += 3;
            break;

			// CPY
        case 0xC0:
			p.as_16 = cpu->Y - read(inp + 1);
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// CMP
        case 0xC1:
			p.as_16 = cpu->acc - get(indx(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// CPY
        case 0xC4:
			p.as_16 = cpu->Y - get(zpg(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// CMP
        case 0xC5:
			p.as_16 = cpu->acc - get(zpg(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// DEC
        case 0xC6:
			p.as_8 = get(zpg(inp + 1)) - 1;
			nega(p.as_8);
			zero(p.as_8);
			mem[zpg(inp + 1)] = p.as_8;
			inp += 2;
            break;

			// INY
        case 0xC8:
			cpu->Y++;
			nega(cpu->Y);
			zero(cpu->Y);
			inp++;
            break;

			// CMP
        case 0xC9:
			p.as_16 = cpu->acc - read(inp + 1);
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// DEX
        case 0xCA:
			cpu->X--;
			nega(cpu->X);
			zero(cpu->X);
			inp++;
            break;

			// CPY
        case 0xCC:
			p.as_16 = cpu->Y - get(abs(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 3;
            break;

			// CMP
        case 0xCD:
			p.as_16 = cpu->acc - get(abs(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 3;
            break;

			// DEC
        case 0xCE:
			p.as_8 = get(abs(inp + 1)) - 1;
			nega(p.as_8);
			zero(p.as_8);
			mem[abs(inp + 1)] = p.as_8;
			inp += 3;
            break;

			// BNE
        case 0xD0:
            inp = cpu->flags & Z ? inp + 2 : rel(inp);
            break;

			// CMP
        case 0xD1:
			p.as_16 = cpu->acc - get(indy(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

        case 0xD5:
			p.as_16 = cpu->acc - get(zpgx(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// DEC
        case 0xD6:
			p.as_8 = get(zpgx(inp + 1)) - 1;
			nega(p.as_8);
			zero(p.as_8);
			mem[zpgx(inp + 1)] = p.as_8;
			inp += 2;
            break;

			// CLD
        case 0xD8:
			cpu->flags &= ~D;
			inp++;
            break;

			// CMP
        case 0xD9:
			p.as_16 = cpu->acc - get(absy(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 3;
            break;

        case 0xDD:
			p.as_16 = cpu->acc - get(absx(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 3;
            break;

			// DEC
        case 0xDE:
			p.as_8 = get(absx(inp + 1)) - 1;
			nega(p.as_8);
			zero(p.as_8);
			mem[absx(inp + 1)] = p.as_8;
			inp += 3;
            break;

			// CPX
        case 0xE0:
			p.as_16 = cpu->X - read(inp + 1);
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// SBC
        case 0xE1:
		 	p.as_8 = get(indx(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(indx(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < (get(indx(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 2;
            break;

			// CPX
        case 0xE4:
			p.as_16 = cpu->X - get(zpg(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 2;
            break;

			// SBC
        case 0xE5:
		 	p.as_8 = get(zpg(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(zpg(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < (get(zpg(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 2;
            break;

			// INC
        case 0xE6:
			p.as_8 = ++mem[zpg(inp + 1)];
			zero(p.as_8);
			nega(p.as_8);
			inp += 2;
            break;

			// INX
        case 0xE8:
			cpu->X++;
			nega(cpu->X);
			zero(cpu->X);
			inp++;
            break;

			// SBC
        case 0xE9:
		 	p.as_8 = read(inp + 1);
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ read(inp + 1)) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < read(inp + 1) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 2;
            break;

			// NOP
        case 0xEA:
			inp++;
            break;

			// CPX
        case 0xEC:
			p.as_16 = cpu->X - get(abs(inp + 1));
			p.as_16 >> 8 ? cpu->flags |= C : cpu->flags &= ~C;
			zero(p.as_8);
			nega(p.as_8 & 0x80);
			inp += 3;
            break;

			// SBC
        case 0xED:
		 	p.as_8 = get(abs(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(abs(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < get(abs(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 3;
            break;

			// INC
        case 0xEE:
			p.as_8 = ++mem[abs(inp + 1)];
			zero(p.as_8);
			nega(p.as_8);
			inp += 3;
            break;

			// BEQ
        case 0xF0:
            inp = cpu->flags & Z ? rel(inp) : inp + 2;
            break;

			// SBC
		case 0xF1:
		 	p.as_8 = get(indy(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(indy(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < get(indy(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 2;
            break;

			// SBC
        case 0xF5:
		 	p.as_8 = get(zpgx(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(zpgx(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < get(zpgx(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 2;
            break;

			// INC
        case 0xF6:
			p.as_8 = ++mem[zpgx(inp + 1)];
			zero(p.as_8);
			nega(p.as_8);
			inp += 2;
            break;

			// SED
        case 0xF8:
			cpu->flags |= D;
			inp++;
            break;

			// SBC
        case 0xF9:
		 	p.as_8 = get(absy(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(absy(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < get(absy(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 3;
            break;

        case 0xFD:
		 	p.as_8 = get(absx(inp + 1));
			p.as_16 = cpu->acc - p.as_8 - (cpu->flags & C);
			nega(p.as_8);
			zero(p.as_8);
		    ((cpu->acc ^ p.as_8) & 0x80) && ((cpu->acc ^ get(absx(inp + 1))) & 0x80) ? cpu->flags |= V : cpu->flags &= ~V;
		    if (cpu->flags & D)
		    {
		    	if ( ((cpu->acc & 0x0F) - (cpu->flags & C)) < get(absx(inp + 1)) & 0x0F)) p.as_16 -= 6;
		        if (p.as_16 > 0x99)
		        {
		        	p.as_16 -= 0x60;
		        }
		    }
		    p.as_16 < 0x100 ? cpu->flags &= ~C : cpu->flags |= C;
			cpu->acc = p.as_8;
			inp += 3;
            break;

			// INC
        case 0xFE:
			p.as_8 = ++mem[absx(inp + 1)];
			zero(p.as_8);
			nega(p.as_8);
			inp += 3;
            break;
        }

    }
}
