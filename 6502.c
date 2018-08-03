#include"6502.h"


inline uint16_t *ind(uint16_t *add) return abs(comp.mem[abs(*add)]);
inline uint16_t *indx(uint8_t *zpg) return comp.mem[*zpg + comp.cpu->X << 8 | *zpg + comp.cpu->X + 1];
inline uint16_t *indy(uint8_t *zpg) return comp.mem[*zpg << 8 | *zpg + 1] + comp.cpu->Y;

inline uint16_t *zpg(uint8_t *zpg) return *zpg;
inline uint16_t *zpgx(uint8_t *zpg) return *zpg + comp.cpu->X;
inline uint16_t *zpgy(uint8_t *zpg) return *zpg + comp.cpu->Y;

inline uint16_t *abs(int16_t *add) return *abs >> 8 | *abs << 8;
inline uint16_t *absx(uint16_t *abs) return *(abs(abs)) + comp.cpu->X;
inline uint16_t *absy(uint16_t *abs) return *(abs(abs)) + comp.cpu->Y;

inline uint16_t get(uint16_t abs) return comp.mem[abs];
inline uint16_t rel(int8_t *ofs, uint8_t *inp) return inp + *ofs;

machine_t comp;

int main(){
  comp.mem = (int8_t *)malloc(64 * 1024 * sizeof(int8_t));
  comp.cpu = (cpu_t *)malloc(sizeof(cpu_t));
  // run(&comp)
  return 0;
}

int run(uint8_t *inp){
  int8_t *mem = comp.mem;
  cpu_t *cpu = comp.cpu;
  union{
    int8_t as_8;
    int16_t as_16
  } p;
  for(;;){

    switch(*inp){

      // BRK
      case 0x0:
        inp += 2;
      break;

      // ORA
      case 0x1:
        cpu->acc = get(indx(inp + 1)) | cpu->acc;
        inp += 2;
        cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
        cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      case 0x5:
        comp->cpu->acc = zpg(inp + 1) | comp->cpu->acc;
        inp += 2;
        cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
        cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      // ASL
      case 0x6:
      *p.as_8 = get(zpg(inp + 1))
      *p.as_8 <<= 1;
      *p.as_8 ? cpu->flags &= ~Z : cpu->flags |= Z;
      *p.as_8 >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      *p.as_8 >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
      inp += 2;
      break;

      case 0x8:

      break;
      // ORA
      case 0x9:
      comp->cpu->acc = *(inp + 1) | comp->cpu->acc;
      inp += 2;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      // ASL
      case 0xA:
      cpu->acc <<= 1;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      cpu->acc >> 7 ? cpu->flags |= C : cpu->flags &= ~C;
      inp += 1;
      break;

      // ORA
      case 0xD:
      comp->cpu->acc = get(abs(inp + 1)) | comp->cpu->acc;
      inp += 3;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      // ASL
      case 0xE:
      *p.as_16 = get(abs(inp + 1));
      *p.as_16 <<= 1;
      *p.as_16 ? cpu->flags &= ~Z : cpu->flags |= Z;
      *p.as_16 >> 15 ? cpu->flags |= N : cpu->flags &= ~N;
      *p.as_16 >> 15 ? cpu->flags |= C : cpu->flags &= ~C;
      inp += 3;
      break;

      case 0x10:

      break;

      // ORA
      case 0x11:
      comp->cpu->acc = get(indy(inp + 1)) | comp->cpu->acc;
      inp += 2;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      case 0x15:
      comp->cpu->acc = get(zpgx(inp + 1)) | comp->cpu->acc;
      inp += 2;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      // ASL
      case 0x16:
      *p.as_8 = get(zpgx(inp+1))
      *p.as_8 <<= 1;
      *p.as_8 ? cpu->flags &= ~Z : cpu->flags |= Z;
      *p.as_8 >> 15 ? cpu->flags |= N : cpu->flags &= ~N;
      *p.as_8 >> 15 ? cpu->flags |= C : cpu->flags &= ~C;
      inp += 2;
      break;

      case 0x18:

      break;

      // ORA
      case 0x19:
      comp->cpu->acc = get(absy(inp + 1)) | comp->cpu->acc;
      inp += 3;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      case 0x1D:
      comp->cpu->acc = get(absx(inp + 1)) | comp->cpu->acc;
      inp += 3;
      cpu->acc ? cpu->flags &= ~Z : cpu->flags |= Z;
      cpu->acc >> 7 ? cpu->flags |= N : cpu->flags &= ~N;
      break;

      case 0x1E:
      *p.as_16 = get(absx(inp + 1));
      *p.as_16 <<= 1;
      *p.as_16 ? cpu->flags &= ~Z : cpu->flags |= Z;
      *p.as_16 >> 15 ? cpu->flags |= N : cpu->flags &= ~N;
      *p.as_16 >> 15 ? cpu->flags |= C : cpu->flags &= ~C;
      inp += 3;
      break;

      case 0x20:

      break;

      case 0x21:

      break;

      case 0x24:

      break;

      case 0x25:

      break;

      case 0x26:

      break;

      case 0x28:

      break;

      case 0x29:

      break;

      case 0x2A:

      break;

      case 0x2C:

      break;

      case 0x2D:

      break;

      case 0x2E:

      break;

      case 0x30:

      break;

      case 0x31:

      break;

      case 0x35:

      break;

      case 0x36:

      break;

      case 0x38:

      break;

      case 0x39:

      break;

      case 0x3D:

      break;

      case 0x3E:

      break;

      case 0x40:

      break;

      case 0x41:

      break;

      case 0x45:

      break;

      case 0x46:

      break;

      case 0x48:

      break;

      case 0x49:

      break;

      case 0x4A:

      break;

      case 0x4C:

      break;

      case 0x4D:

      break;

      case 0x4E:

      break;

      case 0x50:

      break;

      case 0x51:

      break;

      case 0x55:

      break;

      case 0x56:

      break;

      case 0x58:

      break;

      case 0x59:

      break;

      case 0x5D:

      break;

      case 0x5E:

      break;

      case 0x60:

      break;

      case 0x61:

      break;

      case 0x65:

      break;

      case 0x66:

      break;

      case 0x68:

      break;

      case 0x69:

      break;

      case 0x6A:

      break;

      case 0x6C:

      break;

      case 0x6D:

      break;

      case 0x6E:

      break;

      case 0x70:

      break;

      case 0x71:

      break;

      case 0x75:

      break;

      case 0x76:

      break;

      case 0x78:

      break;

      case 0x79:

      break;

      case 0x7D:

      break;

      case 0x7E:

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
