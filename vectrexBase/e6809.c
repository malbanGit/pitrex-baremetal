#include <stdio.h>
#include <stdint.h>

#include "vecx.h"
#include <vectrex/vectrexInterface.h>

#include "e6809.h"
extern int isShallowRead;

/* code assumptions:
 *  - it is assumed that an 'int' is at least 16 bits long.
 *  - a 16-bit register has valid bits only in the lower 16 bits and an
 *    8-bit register has valid bits only in the lower 8 bits. the upper
 *    may contain garbage!
 *  - all reading functions are assumed to return the requested data in
 *    the lower bits with the unused upper bits all set to zero.
 */


M6809 CPU;

/* user defined read and write functions */

uint8_t(*e6809_read8) (uint16_t address);
void(*e6809_write8) (uint16_t address, uint8_t data);

/* obtain a particular condition code. returns 0 or 1. */

static __inline uint8_t get_cc(uint8_t flag)
{
	return (CPU.reg_cc / flag) & 1;
}

/* set a particular condition code to either 0 or 1.
 * value parameter must be either 0 or 1.
 */

static __inline void set_cc(uint8_t flag, uint8_t value)
{
	CPU.reg_cc &= ~flag;
	CPU.reg_cc |= value * flag;
}

/* test carry */

static __inline uint8_t test_c(uint16_t i0, uint16_t i1,
	uint16_t r, uint16_t sub)
{
	uint16_t flag;

	flag = (i0 | i1) & ~r; /* one of the inputs is 1 and output is 0 */
	flag |= (i0 & i1);      /* both inputs are 1 */
	flag = (flag >> 7) & 1;
	flag ^= sub; /* on a sub, carry is opposite the carry of an add */

	return (uint8_t)flag;
}

/* test negative */

static __inline uint8_t test_n(uint16_t r)
{
	return (r >> 7) & 1;
}

/* test for zero in lower 8 bits */

static __inline uint8_t test_z8(uint16_t r)
{
	uint16_t flag;

	flag = ~r;
	flag = (flag >> 4) & (flag & 0xf);
	flag = (flag >> 2) & (flag & 0x3);
	flag = (flag >> 1) & (flag & 0x1);

	return (uint8_t)flag;
}

/* test for zero in lower 16 bits */

static __inline uint8_t test_z16(uint16_t r)
{
	uint16_t flag;

	flag = ~r;
	flag = (flag >> 8) & (flag & 0xff);
	flag = (flag >> 4) & (flag & 0xf);
	flag = (flag >> 2) & (flag & 0x3);
	flag = (flag >> 1) & (flag & 0x1);

	return (uint8_t)flag;
}

/* overflow is set whenever the sign bits of the inputs are the same
 * but the sign bit of the result is not same as the sign bits of the
 * inputs.
 */

static __inline uint8_t test_v(uint16_t i0, uint16_t i1, uint16_t r)
{
	uint16_t flag;

	flag = ~(i0 ^ i1); /* input sign bits are the same */
	flag &= (i0 ^ r);  /* input sign and output sign not same */
	flag = (flag >> 7) & 1;

	return (uint8_t)flag;
}

static __inline uint16_t get_reg_d(void)
{
	return (CPU.reg_a << 8) | (CPU.reg_b & 0xff);
}

static __inline void set_reg_d(uint16_t value)
{
	CPU.reg_a = (uint8_t)(value >> 8);
	CPU.reg_b = (uint8_t)value;
}

/* read a byte ... the returned value has the lower 8-bits set to the byte
 * while the upper bits are all zero.
 */

static __inline uint8_t vx_read8(uint16_t address)
{
	return (*e6809_read8) (address);
}

/* write a byte ... only the lower 8-bits of the uint16_t data
 * is written. the upper bits are ignored.
 */

static __inline void vx_write8(uint16_t address, uint8_t data)
{
	(*e6809_write8) (address, data);
}


/*



uint8_t via_read(uint16_t address);
void via_write(uint16_t address, uint8_t data);



static __inline  uint8_t vx_read8(uint16_t address)
{
    uint8_t data = 0xff;

    if ((address & 0xe000) == 0xe000)
    {
        // rom 
        data = rom[address & 0x1fff];
    }
    else if ((address & 0xe000) == 0xc000)
    {
        if (address & 0x800)
        {
            // ram 
            data = ram[address & 0x3ff];
        }
        else if (address & 0x1000)
        {
            // io 
            if (directEmulation)
            {
              data =  GET(address);
            }
              
            else
              data = via_read(address);
        }
    }
    else if (address < 0x8000)
    {
        // cartridge 
        data = cart[address];
    }

    return data;
}

static __inline void  vx_write8(uint16_t address, uint8_t data)
{
    if ((address & 0xe000) == 0xe000)
    {
        // rom 
    }
    else if ((address & 0xe000) == 0xc000)
    {
        // it is possible for both ram and io to be written at the same! 

        if (address & 0x800)
        {
          // if bedlam
          // do not allow write to VecRfsr
//          if ((CPU.reg_pc<0x100) && ((address == 0xc83d) || (address == 0xc83e)) ) return;
          
            ram[address & 0x3ff] = data;
        }

        if (address & 0x1000)
        {
            via_write(address, data);
        }
    }
    else if (address < 0x8000)
    {
        // cartridge 
    }
}

*/














static __inline uint16_t vx_read16(uint16_t address)
{
	return (vx_read8(address) << 8) | vx_read8(address + 1);
}

static __inline void vx_write16(uint16_t address, uint16_t data)
{
	vx_write8(address, (uint8_t)(data >> 8));
	vx_write8(address + 1, (uint8_t)data);
}

static __inline void push8(uint16_t *sp, uint8_t data)
{
	(*sp)--;
	vx_write8(*sp, data);
}

static __inline uint8_t pull8(uint16_t *sp)
{
	uint8_t data;

	data = vx_read8(*sp);
	(*sp)++;

	return data;
}

static __inline void push16(uint16_t *sp, uint16_t data)
{
	push8(sp, (uint8_t)data);
	push8(sp, (uint8_t)(data >> 8));
}

static __inline uint16_t pull16(uint16_t *sp)
{
	uint16_t tmp = pull8(sp);
	return (tmp << 8) | pull8(sp);
}

/* read a byte from the address pointed to by the pc */

static __inline uint8_t pc_vx_read8(void)
{
	return vx_read8(CPU.reg_pc++);
}

/* read a word from the address pointed to by the pc */

static __inline uint16_t pc_vx_read16(void)
{
	uint16_t data;

	data = vx_read16(CPU.reg_pc);
	CPU.reg_pc += 2;

	return data;
}

/* sign extend an 8-bit quantity into a 16-bit quantity */

static __inline uint16_t sign_extend(uint16_t data)
{
	return (~(data & 0x80) + 1) | (data & 0xff);
}

/* direct addressing, upper byte of the address comes from
 * the direct page register, and the lower byte comes from the
 * instruction itself.
 */

static __inline uint16_t ea_direct(void)
{
	return (CPU.reg_dp << 8) | pc_vx_read8();
}

/* extended addressing, address is obtained from 2 bytes following
 * the instruction.
 */

static __inline uint16_t ea_extended(void)
{
	return pc_vx_read16();
}

/* indexed addressing */

static __inline uint16_t ea_indexed(uint16_t *cycles)
{
	uint16_t ea = 0;

	/* post byte */

	uint16_t op = pc_vx_read8();

	uint16_t r = (op >> 5) & 3;

	switch (op)
	{
	case 0x00: case 0x01: case 0x02: case 0x03:
	case 0x04: case 0x05: case 0x06: case 0x07:
	case 0x08: case 0x09: case 0x0a: case 0x0b:
	case 0x0c: case 0x0d: case 0x0e: case 0x0f:
	case 0x20: case 0x21: case 0x22: case 0x23:
	case 0x24: case 0x25: case 0x26: case 0x27:
	case 0x28: case 0x29: case 0x2a: case 0x2b:
	case 0x2c: case 0x2d: case 0x2e: case 0x2f:
	case 0x40: case 0x41: case 0x42: case 0x43:
	case 0x44: case 0x45: case 0x46: case 0x47:
	case 0x48: case 0x49: case 0x4a: case 0x4b:
	case 0x4c: case 0x4d: case 0x4e: case 0x4f:
	case 0x60: case 0x61: case 0x62: case 0x63:
	case 0x64: case 0x65: case 0x66: case 0x67:
	case 0x68: case 0x69: case 0x6a: case 0x6b:
	case 0x6c: case 0x6d: case 0x6e: case 0x6f:
		/* R, +[0, 15] */

		ea = *CPU.rptr_xyus[r] + (op & 0xf);
		(*cycles)++;
		break;
	case 0x10: case 0x11: case 0x12: case 0x13:
	case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1a: case 0x1b:
	case 0x1c: case 0x1d: case 0x1e: case 0x1f:
	case 0x30: case 0x31: case 0x32: case 0x33:
	case 0x34: case 0x35: case 0x36: case 0x37:
	case 0x38: case 0x39: case 0x3a: case 0x3b:
	case 0x3c: case 0x3d: case 0x3e: case 0x3f:
	case 0x50: case 0x51: case 0x52: case 0x53:
	case 0x54: case 0x55: case 0x56: case 0x57:
	case 0x58: case 0x59: case 0x5a: case 0x5b:
	case 0x5c: case 0x5d: case 0x5e: case 0x5f:
	case 0x70: case 0x71: case 0x72: case 0x73:
	case 0x74: case 0x75: case 0x76: case 0x77:
	case 0x78: case 0x79: case 0x7a: case 0x7b:
	case 0x7c: case 0x7d: case 0x7e: case 0x7f:
		/* R, +[-16, -1] */

		ea = *CPU.rptr_xyus[r] + (op & 0xf) - 0x10;
		(*cycles)++;
		break;
	case 0x80: case 0x81:
	case 0xa0: case 0xa1:
	case 0xc0: case 0xc1:
	case 0xe0: case 0xe1:
		/* ,R+ / ,R++ */

		ea = *CPU.rptr_xyus[r];
		*CPU.rptr_xyus[r] += 1 + (op & 1);
		*cycles += 2 + (op & 1);
		break;
	case 0x90: case 0x91:
	case 0xb0: case 0xb1:
	case 0xd0: case 0xd1:
	case 0xf0: case 0xf1:
		/* [,R+] ??? / [,R++] */

		ea = vx_read16(*CPU.rptr_xyus[r]);
		*CPU.rptr_xyus[r] += 1 + (op & 1);
		*cycles += 5 + (op & 1);
		break;
	case 0x82: case 0x83:
	case 0xa2: case 0xa3:
	case 0xc2: case 0xc3:
	case 0xe2: case 0xe3:

		/* ,-R / ,--R */

		*CPU.rptr_xyus[r] -= 1 + (op & 1);
		ea = *CPU.rptr_xyus[r];
		*cycles += 2 + (op & 1);
		break;
	case 0x92: case 0x93:
	case 0xb2: case 0xb3:
	case 0xd2: case 0xd3:
	case 0xf2: case 0xf3:
		/* [,-R] ??? / [,--R] */

		*CPU.rptr_xyus[r] -= 1 + (op & 1);
		ea = vx_read16(*CPU.rptr_xyus[r]);
		*cycles += 5 + (op & 1);
		break;
	case 0x84: case 0xa4:
	case 0xc4: case 0xe4:
		/* ,R */

		ea = *CPU.rptr_xyus[r];
		break;
	case 0x94: case 0xb4:
	case 0xd4: case 0xf4:
		/* [,R] */

		ea = vx_read16(*CPU.rptr_xyus[r]);
		*cycles += 3;
		break;
	case 0x85: case 0xa5:
	case 0xc5: case 0xe5:
		/* B,R */

		ea = *CPU.rptr_xyus[r] + sign_extend(CPU.reg_b);
		*cycles += 1;
		break;
	case 0x95: case 0xb5:
	case 0xd5: case 0xf5:
		/* [B,R] */

		ea = vx_read16(*CPU.rptr_xyus[r] + sign_extend(CPU.reg_b));
		*cycles += 4;
		break;
	case 0x86: case 0xa6:
	case 0xc6: case 0xe6:
		/* A,R */

		ea = *CPU.rptr_xyus[r] + sign_extend(CPU.reg_a);
		*cycles += 1;
		break;
	case 0x96: case 0xb6:
	case 0xd6: case 0xf6:
		/* [A,R] */

		ea = vx_read16(*CPU.rptr_xyus[r] + sign_extend(CPU.reg_a));
		*cycles += 4;
		break;
	case 0x88: case 0xa8:
	case 0xc8: case 0xe8:
		/* byte,R */

		ea = *CPU.rptr_xyus[r] + sign_extend(pc_vx_read8());
		*cycles += 1;
		break;
	case 0x98: case 0xb8:
	case 0xd8: case 0xf8:
		/* [byte,R] */

		ea = vx_read16(*CPU.rptr_xyus[r] + sign_extend(pc_vx_read8()));
		*cycles += 4;
		break;
	case 0x89: case 0xa9:
	case 0xc9: case 0xe9:
		/* word,R */

		ea = *CPU.rptr_xyus[r] + pc_vx_read16();
		*cycles += 4;
		break;
	case 0x99: case 0xb9:
	case 0xd9: case 0xf9:
		/* [word,R] */

		ea = vx_read16(*CPU.rptr_xyus[r] + pc_vx_read16());
		*cycles += 7;
		break;
	case 0x8b: case 0xab:
	case 0xcb: case 0xeb:
		/* D,R */

		ea = *CPU.rptr_xyus[r] + get_reg_d();
		*cycles += 4;
		break;
	case 0x9b: case 0xbb:
	case 0xdb: case 0xfb:
		/* [D,R] */

		ea = vx_read16(*CPU.rptr_xyus[r] + get_reg_d());
		*cycles += 7;
		break;
	case 0x8c: case 0xac:
	case 0xcc: case 0xec:
		/* byte, PC */

		r = sign_extend(pc_vx_read8());
		ea = CPU.reg_pc + r;
		*cycles += 1;
		break;
	case 0x9c: case 0xbc:
	case 0xdc: case 0xfc:
		/* [byte, PC] */

		r = sign_extend(pc_vx_read8());
		ea = vx_read16(CPU.reg_pc + r);
		*cycles += 4;
		break;
	case 0x8d: case 0xad:
	case 0xcd: case 0xed:
		/* word, PC */

		r = pc_vx_read16();
		ea = CPU.reg_pc + r;
		*cycles += 5;
		break;
	case 0x9d: case 0xbd:
	case 0xdd: case 0xfd:
		/* [word, PC] */

		r = pc_vx_read16();
		ea = vx_read16(CPU.reg_pc + r);
		*cycles += 8;
		break;
	case 0x9f:
		/* [address] */

		ea = vx_read16(pc_vx_read16());
		*cycles += 5;
		break;
	default:
		printf("undefined post-byte\n");
		break;
	}

	return ea;
}

/* instruction: neg
 * essentially (0 - data).
 */

static __inline uint16_t inst_neg(uint16_t data)
{
	uint16_t i0 = 0;
	uint16_t i1 = ~data;
	uint16_t r = i0 + i1 + 1;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 1));

	return r;
}

/* instruction: com */

static __inline uint16_t inst_com(uint16_t data)
{
	uint16_t r = ~data;

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, 0);
	set_cc(FLAG_C, 1);

	return r;
}

/* instruction: lsr
 * cannot be faked as an add or substract.
 */

static __inline uint16_t inst_lsr(uint16_t data)
{
	uint16_t r = (data >> 1) & 0x7f;

	set_cc(FLAG_N, 0);
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_C, data & 1);

	return r;
}

/* instruction: ror
 * cannot be faked as an add or substract.
 */

static __inline uint16_t inst_ror(uint16_t data)
{
	uint16_t c = get_cc(FLAG_C);
	uint16_t r = ((data >> 1) & 0x7f) | (c << 7);

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_C, data & 1);

	return r;
}

/* instruction: asr
 * cannot be faked as an add or substract.
 */

static __inline uint16_t inst_asr(uint16_t data)
{
	uint16_t r = ((data >> 1) & 0x7f) | (data & 0x80);

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_C, data & 1);

	return r;
}

/* instruction: asl
 * essentially (data + data). simple addition.
 */

static __inline uint16_t inst_asl(uint16_t data)
{
	uint16_t i0 = data;
	uint16_t i1 = data;
	uint16_t r = i0 + i1;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 0));

	return r;
}

/* instruction: rol
 * essentially (data + data + carry). addition with carry.
 */

static __inline uint16_t inst_rol(uint16_t data)
{
	uint16_t i0 = data;
	uint16_t i1 = data;
	uint16_t c = get_cc(FLAG_C);
	uint16_t r = i0 + i1 + c;

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 0));

	return r;
}

/* instruction: dec
 * essentially (data - 1).
 */

static __inline uint16_t inst_dec(uint16_t data)
{
	uint16_t i0 = data;
	uint16_t i1 = 0xff;
	uint16_t r = i0 + i1;

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));

	return r;
}

/* instruction: inc
 * essentially (data + 1).
 */

static __inline uint16_t inst_inc(uint16_t data)
{
	uint16_t i0 = data;
	uint16_t i1 = 1;
	uint16_t r = i0 + i1;

	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));

	return r;
}

/* instruction: tst */

static __inline void inst_tst8(uint8_t data)
{
	set_cc(FLAG_N, test_n(data));
	set_cc(FLAG_Z, test_z8(data));
	set_cc(FLAG_V, 0);
}

static __inline void inst_tst16(uint16_t data)
{
	set_cc(FLAG_N, test_n(data >> 8));
	set_cc(FLAG_Z, test_z16(data));
	set_cc(FLAG_V, 0);
}

/* instruction: clr */

static __inline void inst_clr(void)
{
	set_cc(FLAG_N, 0);
	set_cc(FLAG_Z, 1);
	set_cc(FLAG_V, 0);
	set_cc(FLAG_C, 0);
}

/* instruction: suba/subb */

static __inline uint8_t inst_sub8(uint8_t data0, uint8_t data1)
{
	uint8_t i0 = data0;
	uint8_t i1 = ~data1;
	uint8_t r = i0 + i1 + 1;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 1));

	return r;
}

/* instruction: sbca/sbcb/cmpa/cmpb.
 * only 8-bit version, 16-bit version not needed.
 */

static __inline uint8_t inst_sbc(uint8_t data0, uint8_t data1)
{
	uint8_t i0 = data0;
	uint8_t i1 = ~data1;
	uint8_t c = 1 - get_cc(FLAG_C);
	uint8_t r = i0 + i1 + c;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 1));

	return r;
}

/* instruction: anda/andb/bita/bitb.
 * only 8-bit version, 16-bit version not needed.
 */

static __inline uint8_t inst_and(uint8_t data0, uint8_t data1)
{
	uint8_t r = data0 & data1;

	inst_tst8(r);

	return r;
}

/* instruction: eora/eorb.
 * only 8-bit version, 16-bit version not needed.
 */

static __inline uint8_t inst_eor(uint8_t data0, uint8_t data1)
{
	uint8_t r = data0 ^ data1;

	inst_tst8(r);

	return r;
}

/* instruction: adca/adcb
 * only 8-bit version, 16-bit version not needed.
 */

static __inline uint8_t inst_adc(uint8_t data0, uint8_t data1)
{
	uint8_t i0 = data0;
	uint8_t i1 = data1;
	uint8_t c = get_cc(FLAG_C);
	uint8_t r = i0 + i1 + c;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 0));

	return r;
}

/* instruction: ora/orb.
 * only 8-bit version, 16-bit version not needed.
 */

static __inline uint8_t inst_or(uint8_t data0, uint8_t data1)
{
	uint8_t r = data0 | data1;

	inst_tst8(r);

	return r;
}

/* instruction: adda/addb */

static __inline uint8_t inst_add8(uint8_t data0, uint8_t data1)
{
	uint8_t i0 = data0;
	uint8_t i1 = data1;
	uint8_t r = i0 + i1;

	set_cc(FLAG_H, test_c(i0 << 4, i1 << 4, r << 4, 0));
	set_cc(FLAG_N, test_n(r));
	set_cc(FLAG_Z, test_z8(r));
	set_cc(FLAG_V, test_v(i0, i1, r));
	set_cc(FLAG_C, test_c(i0, i1, r, 0));

	return (uint8_t)r;
}

/* instruction: addd */

static __inline uint16_t inst_add16(uint16_t data0, uint16_t data1)
{
	uint16_t i0 = data0;
	uint16_t i1 = data1;
	uint16_t r = i0 + i1;

	set_cc(FLAG_N, test_n(r >> 8));
	set_cc(FLAG_Z, test_z16(r));
	set_cc(FLAG_V, test_v(i0 >> 8, i1 >> 8, r >> 8));
	set_cc(FLAG_C, test_c(i0 >> 8, i1 >> 8, r >> 8, 0));

	return r;
}

/* instruction: subd */

static __inline uint16_t inst_sub16(uint16_t data0, uint16_t data1)
{
	uint16_t i0 = data0;
	uint16_t i1 = ~data1;
	uint16_t r = i0 + i1 + 1;

	set_cc(FLAG_N, test_n(r >> 8));
	set_cc(FLAG_Z, test_z16(r));
	set_cc(FLAG_V, test_v(i0 >> 8, i1 >> 8, r >> 8));
	set_cc(FLAG_C, test_c(i0 >> 8, i1 >> 8, r >> 8, 1));

	return r;
}

/* instruction: 8-bit offset branch */

static __inline void inst_bra8(uint16_t test, uint16_t op, uint16_t *cycles)
{
	uint16_t offset = pc_vx_read8();

	/* trying to avoid an if statement */

	uint16_t mask = (test ^ (op & 1)) - 1; /* 0xffff when taken, 0 when not taken */
	CPU.reg_pc += sign_extend(offset) & mask;

	*cycles += 3;
}

/* instruction: 16-bit offset branch */

static __inline void inst_bra16(uint16_t test, uint16_t op, uint16_t *cycles)
{
	uint16_t offset = pc_vx_read16();

	/* trying to avoid an if statement */

	uint16_t mask = (test ^ (op & 1)) - 1; /* 0xffff when taken, 0 when not taken */
	CPU.reg_pc += offset & mask;

	*cycles += 5 - mask;
}

/* instruction: pshs/pshu */

static __inline void inst_psh(uint16_t op, uint16_t *sp,
	uint16_t data, uint16_t *cycles)
{
	if (op & 0x80)
	{
		push16(sp, CPU.reg_pc);
		*cycles += 2;
	}

	if (op & 0x40)
	{
		/* either s or u */
		push16(sp, data);
		*cycles += 2;
	}

	if (op & 0x20)
	{
		push16(sp, CPU.reg_y);
		*cycles += 2;
	}

	if (op & 0x10)
	{
		push16(sp, CPU.reg_x);
		*cycles += 2;
	}

	if (op & 0x08)
	{
		push8(sp, CPU.reg_dp);
		*cycles += 1;
	}

	if (op & 0x04)
	{
		push8(sp, CPU.reg_b);
		*cycles += 1;
	}

	if (op & 0x02)
	{
		push8(sp, CPU.reg_a);
		*cycles += 1;
	}

	if (op & 0x01)
	{
		push8(sp, CPU.reg_cc);
		*cycles += 1;
	}
}

/* instruction: puls/pulu */

static __inline void inst_pul(uint16_t op, uint16_t *sp, uint16_t *osp,
	uint16_t *cycles)
{
	if (op & 0x01)
	{
		CPU.reg_cc = pull8(sp);
		*cycles += 1;
	}

	if (op & 0x02)
	{
		CPU.reg_a = pull8(sp);
		*cycles += 1;
	}

	if (op & 0x04)
	{
		CPU.reg_b = pull8(sp);
		*cycles += 1;
	}

	if (op & 0x08)
	{
		CPU.reg_dp = pull8(sp);
		*cycles += 1;
	}

	if (op & 0x10)
	{
		CPU.reg_x = pull16(sp);
		*cycles += 2;
	}

	if (op & 0x20)
	{
		CPU.reg_y = pull16(sp);
		*cycles += 2;
	}

	if (op & 0x40)
	{
		/* either s or u */
		*osp = pull16(sp);
		*cycles += 2;
	}

	if (op & 0x80)
	{
		CPU.reg_pc = pull16(sp);
		*cycles += 2;
	}
}

static __inline uint16_t exgtfr_read(uint16_t reg)
{
	uint16_t data;

	switch (reg)
	{
	case 0x0:
		data = get_reg_d();
		break;
	case 0x1:
		data = CPU.reg_x;
		break;
	case 0x2:
		data = CPU.reg_y;
		break;
	case 0x3:
		data = CPU.reg_u;
		break;
	case 0x4:
		data = CPU.reg_s;
		break;
	case 0x5:
		data = CPU.reg_pc;
		break;
	case 0x8:
		data = 0xff00 | CPU.reg_a;
		break;
	case 0x9:
		data = 0xff00 | CPU.reg_b;
		break;
	case 0xa:
		data = 0xff00 | CPU.reg_cc;
		break;
	case 0xb:
		data = 0xff00 | CPU.reg_dp;
		break;
	default:
		data = 0xffff;
		printf("illegal exgtfr reg %.1x\n", reg);
		break;
	}

	return data;
}

static __inline void exgtfr_write(uint16_t reg, uint16_t data)
{
	switch (reg)
	{
	case 0x0:
		set_reg_d(data);
		break;
	case 0x1:
		CPU.reg_x = data;
		break;
	case 0x2:
		CPU.reg_y = data;
		break;
	case 0x3:
		CPU.reg_u = data;
		break;
	case 0x4:
		CPU.reg_s = data;
		break;
	case 0x5:
		CPU.reg_pc = data;
		break;
	case 0x8:
		CPU.reg_a = (uint8_t)data;
		break;
	case 0x9:
		CPU.reg_b = (uint8_t)data;
		break;
	case 0xa:
		CPU.reg_cc = (uint8_t)data;
		break;
	case 0xb:
		CPU.reg_dp = (uint8_t)data;
		break;
	default:
		printf("illegal exgtfr reg %.1x\n", reg);
		break;
	}
}

/* instruction: exg */

static __inline void inst_exg(void)
{
	uint16_t op = pc_vx_read8();

	uint16_t tmp = exgtfr_read(op & 0xf);
	exgtfr_write(op & 0xf, exgtfr_read(op >> 4));
	exgtfr_write(op >> 4, tmp);
}

/* instruction: tfr */

static __inline void inst_tfr(void)
{
	uint16_t op = pc_vx_read8();

	exgtfr_write(op & 0xf, exgtfr_read(op >> 4));
}

/* reset the 6809 */

void e6809_reset(void)
{
	CPU.reg_x = 0;
	CPU.reg_y = 0;
	CPU.reg_u = 0;
	CPU.reg_s = 0;

	CPU.reg_a = 0;
	CPU.reg_b = 0;

	CPU.reg_dp = 0;

	CPU.reg_cc = FLAG_I | FLAG_F;
	CPU.irq_status = IRQ_NORMAL;

	CPU.rptr_xyus[0] = &CPU.reg_x;
	CPU.rptr_xyus[1] = &CPU.reg_y;
	CPU.rptr_xyus[2] = &CPU.reg_u;
	CPU.rptr_xyus[3] = &CPU.reg_s;

	CPU.reg_pc = vx_read16(0xfffe);
}

/* execute a single instruction or handle interrupts and return */
uint16_t old_op = 0;
uint16_t old_pc = 0;

uint16_t e6809_sstep(uint16_t irq_i, uint16_t irq_f)
{
	uint16_t cycles = 0;

	if (irq_f)
	{
		if (get_cc(FLAG_F) == 0)
		{
			if (CPU.irq_status != IRQ_CWAI)
			{
				set_cc(FLAG_E, 0);
				inst_psh(0x81, &CPU.reg_s, CPU.reg_u, &cycles);
			}

			set_cc(FLAG_I, 1);
			set_cc(FLAG_F, 1);

			CPU.reg_pc = vx_read16(0xfff6);
			CPU.irq_status = IRQ_NORMAL;
			cycles += 7;
		}
		else
		{
			if (CPU.irq_status == IRQ_SYNC)
			{
				CPU.irq_status = IRQ_NORMAL;
			}
		}
	}

	if (irq_i)
	{
		if (get_cc(FLAG_I) == 0)
		{
			if (CPU.irq_status != IRQ_CWAI)
			{
				set_cc(FLAG_E, 1);
				inst_psh(0xff, &CPU.reg_s, CPU.reg_u, &cycles);
			}

			set_cc(FLAG_I, 1);

			CPU.reg_pc = vx_read16(0xfff8);
			CPU.irq_status = IRQ_NORMAL;
			cycles += 7;
		}
		else
		{
			if (CPU.irq_status == IRQ_SYNC)
			{
				CPU.irq_status = IRQ_NORMAL;
			}
		}
	}

	if (CPU.irq_status != IRQ_NORMAL)
	{
		return cycles + 1;
	}

	
	
	
	
 	old_pc = CPU.reg_pc;
	uint16_t op = pc_vx_read8();
	old_op = op;
	uint16_t ea, i0, i1, r;

	switch (op)
	{
		/* page 0 instructions */

		/* neg, nega, negb */
	case 0x00:
		ea = ea_direct();
        isShallowRead = 1;
		r = inst_neg(vx_read8(ea));
        isShallowRead = 0;
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x40:
		CPU.reg_a = (uint8_t)inst_neg(CPU.reg_a);
		cycles += 2;
		break;
	case 0x50:
		CPU.reg_b = (uint8_t)inst_neg(CPU.reg_b);
		cycles += 2;
		break;
	case 0x60:
		ea = ea_indexed(&cycles);
		r = inst_neg(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x70:
		ea = ea_extended();
        isShallowRead = 1;
		r = inst_neg(vx_read8(ea));
        isShallowRead = 0;
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* com, coma, comb */
	case 0x03:
		ea = ea_direct();
		r = inst_com(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x43:
		CPU.reg_a = (uint8_t)inst_com(CPU.reg_a);
		cycles += 2;
		break;
	case 0x53:
		CPU.reg_b = (uint8_t)inst_com(CPU.reg_b);
		cycles += 2;
		break;
	case 0x63:
		ea = ea_indexed(&cycles);
		r = inst_com(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x73:
		ea = ea_extended();
		r = inst_com(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* lsr, lsra, lsrb */
	case 0x04:
		ea = ea_direct();
		r = inst_lsr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x44:
		CPU.reg_a = (uint8_t)inst_lsr(CPU.reg_a);
		cycles += 2;
		break;
	case 0x54:
		CPU.reg_b = (uint8_t)inst_lsr(CPU.reg_b);
		cycles += 2;
		break;
	case 0x64:
		ea = ea_indexed(&cycles);
		r = inst_lsr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x74:
		ea = ea_extended();
		r = inst_lsr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* ror, rora, rorb */
	case 0x06:
		ea = ea_direct();
		r = inst_ror(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x46:
		CPU.reg_a = (uint8_t)inst_ror(CPU.reg_a);
		cycles += 2;
		break;
	case 0x56:
		CPU.reg_b = (uint8_t)inst_ror(CPU.reg_b);
		cycles += 2;
		break;
	case 0x66:
		ea = ea_indexed(&cycles);
		r = inst_ror(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x76:
		ea = ea_extended();
		r = inst_ror(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* asr, asra, asrb */
	case 0x07:
		ea = ea_direct();
		r = inst_asr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x47:
		CPU.reg_a = (uint8_t)inst_asr(CPU.reg_a);
		cycles += 2;
		break;
	case 0x57:
		CPU.reg_b = (uint8_t)inst_asr(CPU.reg_b);
		cycles += 2;
		break;
	case 0x67:
		ea = ea_indexed(&cycles);
		r = inst_asr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x77:
		ea = ea_extended();
		r = inst_asr(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* asl, asla, aslb */
	case 0x08:
		ea = ea_direct();
		r = inst_asl(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x48:
		CPU.reg_a = (uint8_t)inst_asl(CPU.reg_a);
		cycles += 2;
		break;
	case 0x58:
		CPU.reg_b = (uint8_t)inst_asl(CPU.reg_b);
        cycles += 2;
		break;
	case 0x68:
		ea = ea_indexed(&cycles);
		r = inst_asl(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x78:
		ea = ea_extended();
		r = inst_asl(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* rol, rola, rolb */
	case 0x09:
		ea = ea_direct();
		r = inst_rol(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x49:
		CPU.reg_a = (uint8_t)inst_rol(CPU.reg_a);
		cycles += 2;
		break;
	case 0x59:
		CPU.reg_b = (uint8_t)inst_rol(CPU.reg_b);
		cycles += 2;
		break;
	case 0x69:
		ea = ea_indexed(&cycles);
		r = inst_rol(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x79:
		ea = ea_extended();
		r = inst_rol(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* dec, deca, decb */
	case 0x0a:
		ea = ea_direct();
        isShallowRead = 1;
		r = inst_dec(vx_read8(ea));
        isShallowRead = 0;
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x4a:
		CPU.reg_a = (uint8_t)inst_dec(CPU.reg_a);
        cycles += 2;
		break;
	case 0x5a:
		CPU.reg_b = (uint8_t)inst_dec(CPU.reg_b);
		cycles += 2;
		break;
	case 0x6a:
		ea = ea_indexed(&cycles);
		r = inst_dec(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x7a:
		ea = ea_extended();
        isShallowRead = 1;
		r = inst_dec(vx_read8(ea));
        isShallowRead = 0;
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* inc, inca, incb */
	case 0x0c:
		ea = ea_direct();
        isShallowRead = 1;
        r = inst_inc(vx_read8(ea));
        isShallowRead = 0;
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x4c:
		CPU.reg_a = (uint8_t)inst_inc(CPU.reg_a);
		cycles += 2;
		break;
	case 0x5c:
		CPU.reg_b = (uint8_t)inst_inc(CPU.reg_b);
		cycles += 2;
		break;
	case 0x6c:
		ea = ea_indexed(&cycles);
		r = inst_inc(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 6;
		break;
	case 0x7c:
        isShallowRead = 1;
		ea = ea_extended();
        isShallowRead = 0;
		r = inst_inc(vx_read8(ea));
		vx_write8(ea, (uint8_t)r);
		cycles += 7;
		break;
		/* tst, tsta, tstb */
	case 0x0d:
		ea = ea_direct();
		inst_tst8(vx_read8(ea));
		cycles += 6;
		break;
	case 0x4d:
		inst_tst8(CPU.reg_a);
		cycles += 2;
		break;
	case 0x5d:
		inst_tst8(CPU.reg_b);
		cycles += 2;
		break;
	case 0x6d:
		ea = ea_indexed(&cycles);
		inst_tst8(vx_read8(ea));
		cycles += 6;
		break;
	case 0x7d:
		ea = ea_extended();
		inst_tst8(vx_read8(ea));
		cycles += 7;
		break;
		/* jmp */
	case 0x0e:
		CPU.reg_pc = ea_direct();
		cycles += 3;
		break;
	case 0x6e:
		CPU.reg_pc = ea_indexed(&cycles);
		cycles += 3;
		break;
	case 0x7e:
		CPU.reg_pc = ea_extended();
		cycles += 4;
		break;
		/* clr */
	case 0x0f:
		ea = ea_direct();
        isShallowRead = 1;
		inst_clr();
        isShallowRead = 0;
		vx_write8(ea, 0);
		cycles += 6;
		break;
	case 0x4f:
		inst_clr();
		CPU.reg_a = 0;
		cycles += 2;
		break;
	case 0x5f:
		inst_clr();
		CPU.reg_b = 0;
		cycles += 2;
		break;
	case 0x6f:
		ea = ea_indexed(&cycles);
		inst_clr();
		vx_write8(ea, 0);
		cycles += 6;
		break;
	case 0x7f:
		ea = ea_extended();
        isShallowRead = 1;
		inst_clr();
        isShallowRead = 0;
		vx_write8(ea, 0);
		cycles += 7;
		break;
		/* suba */
	case 0x80:
		CPU.reg_a = inst_sub8(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x90:
		ea = ea_direct();
		CPU.reg_a = inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa0:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb0:
		ea = ea_extended();
		CPU.reg_a = inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* subb */
	case 0xc0:
		CPU.reg_b = inst_sub8(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd0:
		ea = ea_direct();
		CPU.reg_b = inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe0:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf0:
		ea = ea_extended();
		CPU.reg_b = inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* cmpa */
	case 0x81:
		inst_sub8(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x91:
		ea = ea_direct();
		inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa1:
		ea = ea_indexed(&cycles);
		inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb1:
		ea = ea_extended();
		inst_sub8(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* cmpb */
	case 0xc1:
		inst_sub8(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd1:
		ea = ea_direct();
		inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe1:
		ea = ea_indexed(&cycles);
		inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf1:
		ea = ea_extended();
		inst_sub8(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* sbca */
	case 0x82:
		CPU.reg_a = inst_sbc(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x92:
		ea = ea_direct();
		CPU.reg_a = inst_sbc(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa2:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_sbc(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb2:
		ea = ea_extended();
		CPU.reg_a = inst_sbc(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* sbcb */
	case 0xc2:
		CPU.reg_b = inst_sbc(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd2:
		ea = ea_direct();
		CPU.reg_b = inst_sbc(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe2:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_sbc(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf2:
		ea = ea_extended();
		CPU.reg_b = inst_sbc(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* anda */
	case 0x84:
		CPU.reg_a = inst_and(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x94:
		ea = ea_direct();
		CPU.reg_a = inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa4:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb4:
		ea = ea_extended();
		CPU.reg_a = inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* andb */
	case 0xc4:
		CPU.reg_b = inst_and(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd4:
		ea = ea_direct();
		CPU.reg_b = inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe4:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf4:
		ea = ea_extended();
		CPU.reg_b = inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* bita */
	case 0x85:
		inst_and(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x95:
		ea = ea_direct();
		inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa5:
		ea = ea_indexed(&cycles);
		inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb5:
		ea = ea_extended();
		inst_and(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* bitb */
	case 0xc5:
		inst_and(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd5:
		ea = ea_direct();
		inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe5:
		ea = ea_indexed(&cycles);
		inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf5:
		ea = ea_extended();
		inst_and(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* lda */
	case 0x86:
		CPU.reg_a = pc_vx_read8();
		inst_tst8(CPU.reg_a);
		cycles += 2;
		break;
	case 0x96:
		ea = ea_direct();
		CPU.reg_a = vx_read8(ea);
		inst_tst8(CPU.reg_a);
		cycles += 4;
		break;
	case 0xa6:
		ea = ea_indexed(&cycles);
		CPU.reg_a = vx_read8(ea);
		inst_tst8(CPU.reg_a);
		cycles += 4;
		break;
	case 0xb6:
		ea = ea_extended();
		CPU.reg_a = vx_read8(ea);
		inst_tst8(CPU.reg_a);
		cycles += 5;
		break;
		/* ldb */
	case 0xc6:
		CPU.reg_b = pc_vx_read8();
		inst_tst8(CPU.reg_b);
		cycles += 2;
		break;
	case 0xd6:
		ea = ea_direct();
		CPU.reg_b = vx_read8(ea);
		inst_tst8(CPU.reg_b);
		cycles += 4;
		break;
	case 0xe6:
		ea = ea_indexed(&cycles);
		CPU.reg_b = vx_read8(ea);
		inst_tst8(CPU.reg_b);
		cycles += 4;
		break;
	case 0xf6:
		ea = ea_extended();
		CPU.reg_b = vx_read8(ea);
		inst_tst8(CPU.reg_b);
		cycles += 5;
		break;
		/* sta */
	case 0x97:
		ea = ea_direct();
		vx_write8(ea, CPU.reg_a);
		inst_tst8(CPU.reg_a);
		cycles += 4;
		break;
	case 0xa7:
		ea = ea_indexed(&cycles);
		vx_write8(ea, CPU.reg_a);
		inst_tst8(CPU.reg_a);
		cycles += 4;
		break;
	case 0xb7:
		ea = ea_extended();
		vx_write8(ea, CPU.reg_a);
		inst_tst8(CPU.reg_a);
		cycles += 5;
		break;
		/* stb */
	case 0xd7:
		ea = ea_direct();
		vx_write8(ea, CPU.reg_b);
		inst_tst8(CPU.reg_b);
		cycles += 4;
		break;
	case 0xe7:
		ea = ea_indexed(&cycles);
		vx_write8(ea, CPU.reg_b);
		inst_tst8(CPU.reg_b);
		cycles += 4;
		break;
	case 0xf7:
		ea = ea_extended();
		vx_write8(ea, CPU.reg_b);
		inst_tst8(CPU.reg_b);
		cycles += 5;
		break;
		/* eora */
	case 0x88:
		CPU.reg_a = inst_eor(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x98:
		ea = ea_direct();
		CPU.reg_a = inst_eor(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa8:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_eor(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb8:
		ea = ea_extended();
		CPU.reg_a = inst_eor(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* eorb */
	case 0xc8:
		CPU.reg_b = inst_eor(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd8:
		ea = ea_direct();
		CPU.reg_b = inst_eor(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe8:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_eor(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf8:
		ea = ea_extended();
		CPU.reg_b = inst_eor(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* adca */
	case 0x89:
		CPU.reg_a = inst_adc(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x99:
		ea = ea_direct();
		CPU.reg_a = inst_adc(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xa9:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_adc(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xb9:
		ea = ea_extended();
		CPU.reg_a = inst_adc(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* adcb */
	case 0xc9:
		CPU.reg_b = inst_adc(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xd9:
		ea = ea_direct();
		CPU.reg_b = inst_adc(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xe9:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_adc(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xf9:
		ea = ea_extended();
		CPU.reg_b = inst_adc(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* ora */
	case 0x8a:
		CPU.reg_a = inst_or(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x9a:
		ea = ea_direct();
		CPU.reg_a = inst_or(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xaa:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_or(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xba:
		ea = ea_extended();
		CPU.reg_a = inst_or(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* orb */
	case 0xca:
		CPU.reg_b = inst_or(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xda:
		ea = ea_direct();
		CPU.reg_b = inst_or(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xea:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_or(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xfa:
		ea = ea_extended();
		CPU.reg_b = inst_or(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* adda */
	case 0x8b:
		CPU.reg_a = inst_add8(CPU.reg_a, pc_vx_read8());
		cycles += 2;
		break;
	case 0x9b:
		ea = ea_direct();
		CPU.reg_a = inst_add8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xab:
		ea = ea_indexed(&cycles);
		CPU.reg_a = inst_add8(CPU.reg_a, vx_read8(ea));
		cycles += 4;
		break;
	case 0xbb:
		ea = ea_extended();
		CPU.reg_a = inst_add8(CPU.reg_a, vx_read8(ea));
		cycles += 5;
		break;
		/* addb */
	case 0xcb:
		CPU.reg_b = inst_add8(CPU.reg_b, pc_vx_read8());
		cycles += 2;
		break;
	case 0xdb:
		ea = ea_direct();
		CPU.reg_b = inst_add8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xeb:
		ea = ea_indexed(&cycles);
		CPU.reg_b = inst_add8(CPU.reg_b, vx_read8(ea));
		cycles += 4;
		break;
	case 0xfb:
		ea = ea_extended();
		CPU.reg_b = inst_add8(CPU.reg_b, vx_read8(ea));
		cycles += 5;
		break;
		/* subd */
	case 0x83:
		set_reg_d(inst_sub16(get_reg_d(), pc_vx_read16()));
		cycles += 4;
		break;
	case 0x93:
		ea = ea_direct();
		set_reg_d(inst_sub16(get_reg_d(), vx_read16(ea)));
		cycles += 6;
		break;
	case 0xa3:
		ea = ea_indexed(&cycles);
		set_reg_d(inst_sub16(get_reg_d(), vx_read16(ea)));
		cycles += 6;
		break;
	case 0xb3:
		ea = ea_extended();
		set_reg_d(inst_sub16(get_reg_d(), vx_read16(ea)));
		cycles += 7;
		break;
		/* cmpx */
	case 0x8c:
		inst_sub16(CPU.reg_x, pc_vx_read16());
		cycles += 4;
		break;
	case 0x9c:
		ea = ea_direct();
		inst_sub16(CPU.reg_x, vx_read16(ea));
		cycles += 6;
		break;
	case 0xac:
		ea = ea_indexed(&cycles);
		inst_sub16(CPU.reg_x, vx_read16(ea));
		cycles += 6;
		break;
	case 0xbc:
		ea = ea_extended();
		inst_sub16(CPU.reg_x, vx_read16(ea));
		cycles += 7;
		break;
		/* ldx */
	case 0x8e:
		CPU.reg_x = pc_vx_read16();
		inst_tst16(CPU.reg_x);
		cycles += 3;
		break;
	case 0x9e:
		ea = ea_direct();
		CPU.reg_x = vx_read16(ea);
		inst_tst16(CPU.reg_x);
		cycles += 5;
		break;
	case 0xae:
		ea = ea_indexed(&cycles);
		CPU.reg_x = vx_read16(ea);
		inst_tst16(CPU.reg_x);
		cycles += 5;
		break;
	case 0xbe:
		ea = ea_extended();
		CPU.reg_x = vx_read16(ea);
		inst_tst16(CPU.reg_x);
		cycles += 6;
		break;
		/* ldu */
	case 0xce:
		CPU.reg_u = pc_vx_read16();
		inst_tst16(CPU.reg_u);
		cycles += 3;
		break;
	case 0xde:
		ea = ea_direct();
		CPU.reg_u = vx_read16(ea);
		inst_tst16(CPU.reg_u);
		cycles += 5;
		break;
	case 0xee:
		ea = ea_indexed(&cycles);
		CPU.reg_u = vx_read16(ea);
		inst_tst16(CPU.reg_u);
		cycles += 5;
		break;
	case 0xfe:
		ea = ea_extended();
		CPU.reg_u = vx_read16(ea);
		inst_tst16(CPU.reg_u);
		cycles += 6;
		break;
		/* stx */
	case 0x9f:
		ea = ea_direct();
		vx_write16(ea, CPU.reg_x);
		inst_tst16(CPU.reg_x);
		cycles += 5;
		break;
	case 0xaf:
		ea = ea_indexed(&cycles);
		vx_write16(ea, CPU.reg_x);
		inst_tst16(CPU.reg_x);
		cycles += 5;
		break;
	case 0xbf:
		ea = ea_extended();
		vx_write16(ea, CPU.reg_x);
		inst_tst16(CPU.reg_x);
		cycles += 6;
		break;
		/* stu */
	case 0xdf:
		ea = ea_direct();
		vx_write16(ea, CPU.reg_u);
		inst_tst16(CPU.reg_u);
		cycles += 5;
		break;
	case 0xef:
		ea = ea_indexed(&cycles);
		vx_write16(ea, CPU.reg_u);
		inst_tst16(CPU.reg_u);
		cycles += 5;
		break;
	case 0xff:
		ea = ea_extended();
		vx_write16(ea, CPU.reg_u);
		inst_tst16(CPU.reg_u);
		cycles += 6;
		break;
		/* addd */
	case 0xc3:
		set_reg_d(inst_add16(get_reg_d(), pc_vx_read16()));
		cycles += 4;
		break;
	case 0xd3:
		ea = ea_direct();
		set_reg_d(inst_add16(get_reg_d(), vx_read16(ea)));
		cycles += 6;
		break;
	case 0xe3:
		ea = ea_indexed(&cycles);
		set_reg_d(inst_add16(get_reg_d(), vx_read16(ea)));
		cycles += 6;
		break;
	case 0xf3:
		ea = ea_extended();
		set_reg_d(inst_add16(get_reg_d(), vx_read16(ea)));
		cycles += 7;
		break;
		/* ldd */
	case 0xcc:
		set_reg_d(pc_vx_read16());
		inst_tst16(get_reg_d());
		cycles += 3;
		break;
	case 0xdc:
		ea = ea_direct();
		set_reg_d(vx_read16(ea));
		inst_tst16(get_reg_d());
		cycles += 5;
		break;
	case 0xec:
		ea = ea_indexed(&cycles);
		set_reg_d(vx_read16(ea));
		inst_tst16(get_reg_d());
		cycles += 5;
		break;
	case 0xfc:
		ea = ea_extended();
		set_reg_d(vx_read16(ea));
		inst_tst16(get_reg_d());
		cycles += 6;
		break;
		/* std */
	case 0xdd:
		ea = ea_direct();
		vx_write16(ea, get_reg_d());
		inst_tst16(get_reg_d());
		cycles += 5;
		break;
	case 0xed:
		ea = ea_indexed(&cycles);
		vx_write16(ea, get_reg_d());
		inst_tst16(get_reg_d());
		cycles += 5;
		break;
	case 0xfd:
		ea = ea_extended();
		vx_write16(ea, get_reg_d());
		inst_tst16(get_reg_d());
		cycles += 6;
		break;
		/* nop */
	case 0x12:
		cycles += 2;
		break;
		/* mul */
	case 0x3d:
		r = (CPU.reg_a & 0xff) * (CPU.reg_b & 0xff);
		set_reg_d(r);

		set_cc(FLAG_Z, test_z16(r));
		set_cc(FLAG_C, (r >> 7) & 1);

		cycles += 11;
		break;
		/* bra */
	case 0x20:
		/* brn */
	case 0x21:
		inst_bra8(0, op, &cycles);
		break;
		/* bhi */
	case 0x22:
		/* bls */
	case 0x23:
		inst_bra8(get_cc(FLAG_C) | get_cc(FLAG_Z), op, &cycles);
		break;
		/* bhs/bcc */
	case 0x24:
		/* blo/bcs */
	case 0x25:
		inst_bra8(get_cc(FLAG_C), op, &cycles);
		break;
		/* bne */
	case 0x26:
		/* beq */
	case 0x27:
		inst_bra8(get_cc(FLAG_Z), op, &cycles);
		break;
		/* bvc */
	case 0x28:
		/* bvs */
	case 0x29:
		inst_bra8(get_cc(FLAG_V), op, &cycles);
		break;
		/* bpl */
	case 0x2a:
		/* bmi */
	case 0x2b:
		inst_bra8(get_cc(FLAG_N), op, &cycles);
		break;
		/* bge */
	case 0x2c:
		/* blt */
	case 0x2d:
		inst_bra8(get_cc(FLAG_N) ^ get_cc(FLAG_V), op, &cycles);
		break;
		/* bgt */
	case 0x2e:
		/* ble */
	case 0x2f:
		inst_bra8(get_cc(FLAG_Z) |
			(get_cc(FLAG_N) ^ get_cc(FLAG_V)), op, &cycles);
		break;
		/* lbra */
	case 0x16:
		r = pc_vx_read16();
		CPU.reg_pc += r;
		cycles += 5;
		break;
		/* lbsr */
	case 0x17:
		r = pc_vx_read16();
		push16(&CPU.reg_s, CPU.reg_pc);
		CPU.reg_pc += r;
		cycles += 9;
		break;
		/* bsr */
	case 0x8d:
		r = pc_vx_read8();
		push16(&CPU.reg_s, CPU.reg_pc);
		CPU.reg_pc += sign_extend(r);
		cycles += 7;
		break;
		/* jsr */
	case 0x9d:
		ea = ea_direct();
		push16(&CPU.reg_s, CPU.reg_pc);
		CPU.reg_pc = ea;
		cycles += 7;
		break;
	case 0xad:
		ea = ea_indexed(&cycles);
		push16(&CPU.reg_s, CPU.reg_pc);
		CPU.reg_pc = ea;
		cycles += 7;
		break;
	case 0xbd:
		ea = ea_extended();
		push16(&CPU.reg_s, CPU.reg_pc);
		CPU.reg_pc = ea;
		cycles += 8;
		break;
		/* leax */
	case 0x30:
		CPU.reg_x = ea_indexed(&cycles);
		set_cc(FLAG_Z, test_z16(CPU.reg_x));
		cycles += 4;
		break;
		/* leay */
	case 0x31:
		CPU.reg_y = ea_indexed(&cycles);
		set_cc(FLAG_Z, test_z16(CPU.reg_y));
		cycles += 4;
		break;
		/* leas */
	case 0x32:
		CPU.reg_s = ea_indexed(&cycles);
		cycles += 4;
		break;
		/* leau */
	case 0x33:
		CPU.reg_u = ea_indexed(&cycles);
		cycles += 4;
		break;
		/* pshs */
	case 0x34:
		inst_psh(pc_vx_read8(), &CPU.reg_s, CPU.reg_u, &cycles);
		cycles += 5;
		break;
		/* puls */
	case 0x35:
		inst_pul(pc_vx_read8(), &CPU.reg_s, &CPU.reg_u, &cycles);
		cycles += 5;
		break;
		/* pshu */
	case 0x36:
		inst_psh(pc_vx_read8(), &CPU.reg_u, CPU.reg_s, &cycles);
		cycles += 5;
		break;
		/* pulu */
	case 0x37:
		inst_pul(pc_vx_read8(), &CPU.reg_u, &CPU.reg_s, &cycles);
		cycles += 5;
		break;
		/* rts */
	case 0x39:
		CPU.reg_pc = pull16(&CPU.reg_s);
		cycles += 5;
		break;
		/* abx */
	case 0x3a:
		CPU.reg_x += CPU.reg_b & 0xff;
		cycles += 3;
		break;
		/* orcc */
	case 0x1a:
		CPU.reg_cc |= pc_vx_read8();
		cycles += 3;
		break;
		/* andcc */
	case 0x1c:
		CPU.reg_cc &= pc_vx_read8();
		cycles += 3;
		break;
		/* sex */
	case 0x1d:
		set_reg_d(sign_extend(CPU.reg_b));
		set_cc(FLAG_N, test_n(CPU.reg_a));
		set_cc(FLAG_Z, test_z16(get_reg_d()));
		cycles += 2;
		break;
		/* exg */
	case 0x1e:
		inst_exg();
		cycles += 8;
		break;
		/* tfr */
	case 0x1f:
		inst_tfr();
		cycles += 6;
		break;
		/* rti */
	case 0x3b:
		if (get_cc(FLAG_E))
		{
			inst_pul(0xff, &CPU.reg_s, &CPU.reg_u, &cycles);
		}
		else
		{
			inst_pul(0x81, &CPU.reg_s, &CPU.reg_u, &cycles);
		}

		cycles += 3;
		break;
		/* swi */
	case 0x3f:
		set_cc(FLAG_E, 1);
		inst_psh(0xff, &CPU.reg_s, CPU.reg_u, &cycles);
		set_cc(FLAG_I, 1);
		set_cc(FLAG_F, 1);
		CPU.reg_pc = vx_read16(0xfffa);
		cycles += 7;
		break;
		/* sync */
	case 0x13:
		CPU.irq_status = IRQ_SYNC;
		cycles += 2;
		break;
		/* daa */
	case 0x19:
		i0 = CPU.reg_a;
		i1 = 0;

		if ((CPU.reg_a & 0x0f) > 0x09 || get_cc(FLAG_H) == 1)
		{
			i1 |= 0x06;
		}

		if ((CPU.reg_a & 0xf0) > 0x80 && (CPU.reg_a & 0x0f) > 0x09)
		{
			i1 |= 0x60;
		}

		if ((CPU.reg_a & 0xf0) > 0x90 || get_cc(FLAG_C) == 1)
		{
			i1 |= 0x60;
		}

		CPU.reg_a = i0 + i1;

		set_cc(FLAG_N, test_n(CPU.reg_a));
		set_cc(FLAG_Z, test_z8(CPU.reg_a));
		set_cc(FLAG_V, 0);
		set_cc(FLAG_C, test_c(i0, i1, CPU.reg_a, 0));
		cycles += 2;
		break;
		/* cwai */
	case 0x3c:
		CPU.reg_cc &= pc_vx_read8();
		set_cc(FLAG_E, 1);
		inst_psh(0xff, &CPU.reg_s, CPU.reg_u, &cycles);
		CPU.irq_status = IRQ_CWAI;
		cycles += 4;
		break;

		/* page 1 instructions */

	case 0x10:
		op = pc_vx_read8();

		switch (op)
		{
			/* lbra */
		case 0x20:
			/* lbrn */
		case 0x21:
			inst_bra16(0, op, &cycles);
			break;
			/* lbhi */
		case 0x22:
			/* lbls */
		case 0x23:
			inst_bra16(get_cc(FLAG_C) | get_cc(FLAG_Z), op, &cycles);
			break;
			/* lbhs/lbcc */
		case 0x24:
			/* lblo/lbcs */
		case 0x25:
			inst_bra16(get_cc(FLAG_C), op, &cycles);
			break;
			/* lbne */
		case 0x26:
			/* lbeq */
		case 0x27:
			inst_bra16(get_cc(FLAG_Z), op, &cycles);
			break;
			/* lbvc */
		case 0x28:
			/* lbvs */
		case 0x29:
			inst_bra16(get_cc(FLAG_V), op, &cycles);
			break;
			/* lbpl */
		case 0x2a:
			/* lbmi */
		case 0x2b:
			inst_bra16(get_cc(FLAG_N), op, &cycles);
			break;
			/* lbge */
		case 0x2c:
			/* lblt */
		case 0x2d:
			inst_bra16(get_cc(FLAG_N) ^ get_cc(FLAG_V), op, &cycles);
			break;
			/* lbgt */
		case 0x2e:
			/* lble */
		case 0x2f:
			inst_bra16(get_cc(FLAG_Z) |
				(get_cc(FLAG_N) ^ get_cc(FLAG_V)), op, &cycles);
			break;
			/* cmpd */
		case 0x83:
			inst_sub16(get_reg_d(), pc_vx_read16());
			cycles += 5;
			break;
		case 0x93:
			ea = ea_direct();
			inst_sub16(get_reg_d(), vx_read16(ea));
			cycles += 7;
			break;
		case 0xa3:
			ea = ea_indexed(&cycles);
			inst_sub16(get_reg_d(), vx_read16(ea));
			cycles += 7;
			break;
		case 0xb3:
			ea = ea_extended();
			inst_sub16(get_reg_d(), vx_read16(ea));
			cycles += 8;
			break;
			/* cmpy */
		case 0x8c:
			inst_sub16(CPU.reg_y, pc_vx_read16());
			cycles += 5;
			break;
		case 0x9c:
			ea = ea_direct();
			inst_sub16(CPU.reg_y, vx_read16(ea));
			cycles += 7;
			break;
		case 0xac:
			ea = ea_indexed(&cycles);
			inst_sub16(CPU.reg_y, vx_read16(ea));
			cycles += 7;
			break;
		case 0xbc:
			ea = ea_extended();
			inst_sub16(CPU.reg_y, vx_read16(ea));
			cycles += 8;
			break;
			/* ldy */
		case 0x8e:
			CPU.reg_y = pc_vx_read16();
			inst_tst16(CPU.reg_y);
			cycles += 4;
			break;
		case 0x9e:
			ea = ea_direct();
			CPU.reg_y = vx_read16(ea);
			inst_tst16(CPU.reg_y);
			cycles += 6;
			break;
		case 0xae:
			ea = ea_indexed(&cycles);
			CPU.reg_y = vx_read16(ea);
			inst_tst16(CPU.reg_y);
			cycles += 6;
			break;
		case 0xbe:
			ea = ea_extended();
			CPU.reg_y = vx_read16(ea);
			inst_tst16(CPU.reg_y);
			cycles += 7;
			break;
			/* sty */
		case 0x9f:
			ea = ea_direct();
			vx_write16(ea, CPU.reg_y);
			inst_tst16(CPU.reg_y);
			cycles += 6;
			break;
		case 0xaf:
			ea = ea_indexed(&cycles);
			vx_write16(ea, CPU.reg_y);
			inst_tst16(CPU.reg_y);
			cycles += 6;
			break;
		case 0xbf:
			ea = ea_extended();
			vx_write16(ea, CPU.reg_y);
			inst_tst16(CPU.reg_y);
			cycles += 7;
			break;
			/* lds */
		case 0xce:
			CPU.reg_s = pc_vx_read16();
			inst_tst16(CPU.reg_s);
			cycles += 4;
			break;
		case 0xde:
			ea = ea_direct();
			CPU.reg_s = vx_read16(ea);
			inst_tst16(CPU.reg_s);
			cycles += 6;
			break;
		case 0xee:
			ea = ea_indexed(&cycles);
			CPU.reg_s = vx_read16(ea);
			inst_tst16(CPU.reg_s);
			cycles += 6;
			break;
		case 0xfe:
			ea = ea_extended();
			CPU.reg_s = vx_read16(ea);
			inst_tst16(CPU.reg_s);
			cycles += 7;
			break;
			/* sts */
		case 0xdf:
			ea = ea_direct();
			vx_write16(ea, CPU.reg_s);
			inst_tst16(CPU.reg_s);
			cycles += 6;
			break;
		case 0xef:
			ea = ea_indexed(&cycles);
			vx_write16(ea, CPU.reg_s);
			inst_tst16(CPU.reg_s);
			cycles += 6;
			break;
		case 0xff:
			ea = ea_extended();
			vx_write16(ea, CPU.reg_s);
			inst_tst16(CPU.reg_s);
			cycles += 7;
			break;
			/* swi2 */
		case 0x3f:
			set_cc(FLAG_E, 1);
			inst_psh(0xff, &CPU.reg_s, CPU.reg_u, &cycles);
			CPU.reg_pc = vx_read16(0xfff4);
			cycles += 8;
			break;
		default:
			printf("unknown page-1 op code: %.2x\n", op);
			break;
		}

		break;

		/* page 2 instructions */

	case 0x11:
		op = pc_vx_read8();

		switch (op)
		{
			/* cmpu */
		case 0x83:
			inst_sub16(CPU.reg_u, pc_vx_read16());
			cycles += 5;
			break;
		case 0x93:
			ea = ea_direct();
			inst_sub16(CPU.reg_u, vx_read16(ea));
			cycles += 7;
			break;
		case 0xa3:
			ea = ea_indexed(&cycles);
			inst_sub16(CPU.reg_u, vx_read16(ea));
			cycles += 7;
			break;
		case 0xb3:
			ea = ea_extended();
			inst_sub16(CPU.reg_u, vx_read16(ea));
			cycles += 8;
			break;
			/* cmps */
		case 0x8c:
			inst_sub16(CPU.reg_s, pc_vx_read16());
			cycles += 5;
			break;
		case 0x9c:
			ea = ea_direct();
			inst_sub16(CPU.reg_s, vx_read16(ea));
			cycles += 7;
			break;
		case 0xac:
			ea = ea_indexed(&cycles);
			inst_sub16(CPU.reg_s, vx_read16(ea));
			cycles += 7;
			break;
		case 0xbc:
			ea = ea_extended();
			inst_sub16(CPU.reg_s, vx_read16(ea));
			cycles += 8;
			break;
			/* swi3 */
		case 0x3f:
			set_cc(FLAG_E, 1);
			inst_psh(0xff, &CPU.reg_s, CPU.reg_u, &cycles);
			CPU.reg_pc = vx_read16(0xfff2);
			cycles += 8;
			break;
		default:
			printf("unknown page-2 op code: %.2x\n", op);
			break;
		}

		break;

	default:
		printf("unknown page-0 op code: %.2x, %04x Bank: %i\n", op, old_pc, currentBank);
		break;
	}

	return cycles;
}
