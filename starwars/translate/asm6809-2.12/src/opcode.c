/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <stdlib.h>

#include "array.h"
#include "asm6809.h"
#include "dict.h"
#include "opcode.h"

/* Shorten these macros for a more readable table: */

#define INHERENT OPCODE_INHERENT
#define DIRECT   OPCODE_DIRECT
#define INDEXED  OPCODE_INDEXED
#define EXTENDED OPCODE_EXTENDED
#define MEM      OPCODE_MEM
#define IMM8     OPCODE_IMM8
#define IMM16    OPCODE_IMM16
#define PAIR     OPCODE_PAIR
#define STACKU   OPCODE_STACKU
#define STACKS   OPCODE_STACKS
#define REL8     OPCODE_REL8
#define REL16    OPCODE_REL16
#define IMM32    OPCODE_IMM32
#define IMM8_MEM OPCODE_IMM8_MEM|OPCODE_MEM
#define REG_MEM  OPCODE_REG_MEM
#define TFM      OPCODE_TFM

/* MC6809 opcode table. */

static struct opcode const opcodes_6809[] = {

	{ .op = "neg", .type = MEM, .direct = 0x00, .indexed = 0x60, .extended = 0x70 },
	{ .op = "com", .type = MEM, .direct = 0x03, .indexed = 0x63, .extended = 0x73 },
	{ .op = "lsr", .type = MEM, .direct = 0x04, .indexed = 0x64, .extended = 0x74 },
	{ .op = "ror", .type = MEM, .direct = 0x06, .indexed = 0x66, .extended = 0x76 },
	{ .op = "asr", .type = MEM, .direct = 0x07, .indexed = 0x67, .extended = 0x77 },
	{ .op = "asl", .type = MEM, .direct = 0x08, .indexed = 0x68, .extended = 0x78 },
	{ .op = "lsl", .type = MEM, .direct = 0x08, .indexed = 0x68, .extended = 0x78 },
	{ .op = "rol", .type = MEM, .direct = 0x09, .indexed = 0x69, .extended = 0x79 },
	{ .op = "dec", .type = MEM, .direct = 0x0a, .indexed = 0x6a, .extended = 0x7a },
	{ .op = "inc", .type = MEM, .direct = 0x0c, .indexed = 0x6c, .extended = 0x7c },
	{ .op = "tst", .type = MEM, .direct = 0x0d, .indexed = 0x6d, .extended = 0x7d },
	{ .op = "jmp", .type = MEM, .direct = 0x0e, .indexed = 0x6e, .extended = 0x7e },
	{ .op = "clr", .type = MEM, .direct = 0x0f, .indexed = 0x6f, .extended = 0x7f },

	{ .op = "nop", .type = INHERENT, .immediate = 0x12 },
	{ .op = "sync", .type = INHERENT, .immediate = 0x13 },
	{ .op = "lbra", .type = REL16, .immediate = 0x16 },
	{ .op = "lbsr", .type = REL16, .immediate = 0x17 },
	{ .op = "daa", .type = INHERENT, .immediate = 0x19 },
	{ .op = "orcc", .type = IMM8, .immediate = 0x1a },
	{ .op = "andcc", .type = IMM8, .immediate = 0x1c },
	{ .op = "sex", .type = INHERENT, .immediate = 0x1d },
	{ .op = "exg", .type = PAIR, .immediate = 0x1e },
	{ .op = "tfr", .type = PAIR, .immediate = 0x1f },

	{ .op = "bra", .type = REL8, .immediate = 0x20 },
	{ .op = "brn", .type = REL8, .immediate = 0x21 },
	{ .op = "bhi", .type = REL8, .immediate = 0x22 },
	{ .op = "bls", .type = REL8, .immediate = 0x23 },
	{ .op = "bcc", .type = REL8, .immediate = 0x24 },
	{ .op = "bhs", .type = REL8, .immediate = 0x24 },
	{ .op = "bcs", .type = REL8, .immediate = 0x25 },
	{ .op = "blo", .type = REL8, .immediate = 0x25 },
	{ .op = "bne", .type = REL8, .immediate = 0x26 },
	{ .op = "beq", .type = REL8, .immediate = 0x27 },
	{ .op = "bvc", .type = REL8, .immediate = 0x28 },
	{ .op = "bvs", .type = REL8, .immediate = 0x29 },
	{ .op = "bpl", .type = REL8, .immediate = 0x2a },
	{ .op = "bmi", .type = REL8, .immediate = 0x2b },
	{ .op = "bge", .type = REL8, .immediate = 0x2c },
	{ .op = "blt", .type = REL8, .immediate = 0x2d },
	{ .op = "bgt", .type = REL8, .immediate = 0x2e },
	{ .op = "ble", .type = REL8, .immediate = 0x2f },

	{ .op = "leax", .type = INDEXED, .indexed = 0x30 },
	{ .op = "leay", .type = INDEXED, .indexed = 0x31 },
	{ .op = "leas", .type = INDEXED, .indexed = 0x32 },
	{ .op = "leau", .type = INDEXED, .indexed = 0x33 },

	{ .op = "pshs", .type = STACKS, .immediate = 0x34 },
	{ .op = "puls", .type = STACKS, .immediate = 0x35 },
	{ .op = "pshu", .type = STACKU, .immediate = 0x36 },
	{ .op = "pulu", .type = STACKU, .immediate = 0x37 },
	{ .op = "rts", .type = INHERENT, .immediate = 0x39 },
	{ .op = "abx", .type = INHERENT, .immediate = 0x3a },
	{ .op = "rti", .type = INHERENT, .immediate = 0x3b },
	{ .op = "cwai", .type = IMM8, .immediate = 0x3c },
	{ .op = "mul", .type = INHERENT, .immediate = 0x3d },
	{ .op = "swi", .type = INHERENT, .immediate = 0x3f },

	{ .op = "nega", .type = INHERENT, .immediate = 0x40 },
	{ .op = "coma", .type = INHERENT, .immediate = 0x43 },
	{ .op = "lsra", .type = INHERENT, .immediate = 0x44 },
	{ .op = "rora", .type = INHERENT, .immediate = 0x46 },
	{ .op = "asra", .type = INHERENT, .immediate = 0x47 },
	{ .op = "asla", .type = INHERENT, .immediate = 0x48 },
	{ .op = "lsla", .type = INHERENT, .immediate = 0x48 },
	{ .op = "rola", .type = INHERENT, .immediate = 0x49 },
	{ .op = "deca", .type = INHERENT, .immediate = 0x4a },
	{ .op = "inca", .type = INHERENT, .immediate = 0x4c },
	{ .op = "tsta", .type = INHERENT, .immediate = 0x4d },
	{ .op = "clra", .type = INHERENT, .immediate = 0x4f },

	{ .op = "negb", .type = INHERENT, .immediate = 0x50 },
	{ .op = "comb", .type = INHERENT, .immediate = 0x53 },
	{ .op = "lsrb", .type = INHERENT, .immediate = 0x54 },
	{ .op = "rorb", .type = INHERENT, .immediate = 0x56 },
	{ .op = "asrb", .type = INHERENT, .immediate = 0x57 },
	{ .op = "aslb", .type = INHERENT, .immediate = 0x58 },
	{ .op = "lslb", .type = INHERENT, .immediate = 0x58 },
	{ .op = "rolb", .type = INHERENT, .immediate = 0x59 },
	{ .op = "decb", .type = INHERENT, .immediate = 0x5a },
	{ .op = "incb", .type = INHERENT, .immediate = 0x5c },
	{ .op = "tstb", .type = INHERENT, .immediate = 0x5d },
	{ .op = "clrb", .type = INHERENT, .immediate = 0x5f },

	{ .op = "suba", .type = IMM8|MEM,  .immediate = 0x80, .direct = 0x90, .indexed = 0xa0, .extended = 0xb0 },
	{ .op = "cmpa", .type = IMM8|MEM,  .immediate = 0x81, .direct = 0x91, .indexed = 0xa1, .extended = 0xb1 },
	{ .op = "sbca", .type = IMM8|MEM,  .immediate = 0x82, .direct = 0x92, .indexed = 0xa2, .extended = 0xb2 },
	{ .op = "subd", .type = IMM16|MEM, .immediate = 0x83, .direct = 0x93, .indexed = 0xa3, .extended = 0xb3 },
	{ .op = "anda", .type = IMM8|MEM,  .immediate = 0x84, .direct = 0x94, .indexed = 0xa4, .extended = 0xb4 },
	{ .op = "bita", .type = IMM8|MEM,  .immediate = 0x85, .direct = 0x95, .indexed = 0xa5, .extended = 0xb5 },
	{ .op = "lda",  .type = IMM8|MEM,  .immediate = 0x86, .direct = 0x96, .indexed = 0xa6, .extended = 0xb6 },
	{ .op = "sta",  .type = MEM,                          .direct = 0x97, .indexed = 0xa7, .extended = 0xb7 },
	{ .op = "eora", .type = IMM8|MEM,  .immediate = 0x88, .direct = 0x98, .indexed = 0xa8, .extended = 0xb8 },
	{ .op = "adca", .type = IMM8|MEM,  .immediate = 0x89, .direct = 0x99, .indexed = 0xa9, .extended = 0xb9 },
	{ .op = "ora",  .type = IMM8|MEM,  .immediate = 0x8a, .direct = 0x9a, .indexed = 0xaa, .extended = 0xba },
	{ .op = "adda", .type = IMM8|MEM,  .immediate = 0x8b, .direct = 0x9b, .indexed = 0xab, .extended = 0xbb },
	{ .op = "cmpx", .type = IMM16|MEM, .immediate = 0x8c, .direct = 0x9c, .indexed = 0xac, .extended = 0xbc },
	{ .op = "ldx",  .type = IMM16|MEM, .immediate = 0x8e, .direct = 0x9e, .indexed = 0xae, .extended = 0xbe },
	{ .op = "stx",  .type = MEM,                          .direct = 0x9f, .indexed = 0xaf, .extended = 0xbf },

	{ .op = "bsr",  .type = REL8, .immediate = 0x8d },
	{ .op = "jsr",  .type = MEM, .direct = 0x9d, .indexed = 0xad, .extended = 0xbd },

	{ .op = "subb", .type = IMM8|MEM,  .immediate = 0xc0, .direct = 0xd0, .indexed = 0xe0, .extended = 0xf0 },
	{ .op = "cmpb", .type = IMM8|MEM,  .immediate = 0xc1, .direct = 0xd1, .indexed = 0xe1, .extended = 0xf1 },
	{ .op = "sbcb", .type = IMM8|MEM,  .immediate = 0xc2, .direct = 0xd2, .indexed = 0xe2, .extended = 0xf2 },
	{ .op = "addd", .type = IMM16|MEM, .immediate = 0xc3, .direct = 0xd3, .indexed = 0xe3, .extended = 0xf3 },
	{ .op = "andb", .type = IMM8|MEM,  .immediate = 0xc4, .direct = 0xd4, .indexed = 0xe4, .extended = 0xf4 },
	{ .op = "bitb", .type = IMM8|MEM,  .immediate = 0xc5, .direct = 0xd5, .indexed = 0xe5, .extended = 0xf5 },
	{ .op = "ldb",  .type = IMM8|MEM,  .immediate = 0xc6, .direct = 0xd6, .indexed = 0xe6, .extended = 0xf6 },
	{ .op = "stb",  .type = MEM,                          .direct = 0xd7, .indexed = 0xe7, .extended = 0xf7 },
	{ .op = "eorb", .type = IMM8|MEM,  .immediate = 0xc8, .direct = 0xd8, .indexed = 0xe8, .extended = 0xf8 },
	{ .op = "adcb", .type = IMM8|MEM,  .immediate = 0xc9, .direct = 0xd9, .indexed = 0xe9, .extended = 0xf9 },
	{ .op = "orb",  .type = IMM8|MEM,  .immediate = 0xca, .direct = 0xda, .indexed = 0xea, .extended = 0xfa },
	{ .op = "addb", .type = IMM8|MEM,  .immediate = 0xcb, .direct = 0xdb, .indexed = 0xeb, .extended = 0xfb },
	{ .op = "ldd",  .type = IMM16|MEM, .immediate = 0xcc, .direct = 0xdc, .indexed = 0xec, .extended = 0xfc },
	{ .op = "std",  .type = MEM,                          .direct = 0xdd, .indexed = 0xed, .extended = 0xfd },
	{ .op = "ldu",  .type = IMM16|MEM, .immediate = 0xce, .direct = 0xde, .indexed = 0xee, .extended = 0xfe },
	{ .op = "stu",  .type = MEM,                          .direct = 0xdf, .indexed = 0xef, .extended = 0xff },

	{ .op = "lbrn", .type = REL16, .immediate = 0x1021 },
	{ .op = "lbhi", .type = REL16, .immediate = 0x1022 },
	{ .op = "lbls", .type = REL16, .immediate = 0x1023 },
	{ .op = "lbcc", .type = REL16, .immediate = 0x1024 },
	{ .op = "lbhs", .type = REL16, .immediate = 0x1024 },
	{ .op = "lbcs", .type = REL16, .immediate = 0x1025 },
	{ .op = "lblo", .type = REL16, .immediate = 0x1025 },
	{ .op = "lbne", .type = REL16, .immediate = 0x1026 },
	{ .op = "lbeq", .type = REL16, .immediate = 0x1027 },
	{ .op = "lbvc", .type = REL16, .immediate = 0x1028 },
	{ .op = "lbvs", .type = REL16, .immediate = 0x1029 },
	{ .op = "lbpl", .type = REL16, .immediate = 0x102a },
	{ .op = "lbmi", .type = REL16, .immediate = 0x102b },
	{ .op = "lbge", .type = REL16, .immediate = 0x102c },
	{ .op = "lblt", .type = REL16, .immediate = 0x102d },
	{ .op = "lbgt", .type = REL16, .immediate = 0x102e },
	{ .op = "lble", .type = REL16, .immediate = 0x102f },

	{ .op = "swi2", .type = INHERENT, .immediate = 0x103f },

	{ .op = "cmpd", .type = IMM16|MEM, .immediate = 0x1083, .direct = 0x1093, .indexed = 0x10a3, .extended = 0x10b3 },
	{ .op = "cmpy", .type = IMM16|MEM, .immediate = 0x108c, .direct = 0x109c, .indexed = 0x10ac, .extended = 0x10bc },
	{ .op = "ldy",  .type = IMM16|MEM, .immediate = 0x108e, .direct = 0x109e, .indexed = 0x10ae, .extended = 0x10be },
	{ .op = "sty",  .type = MEM,                            .direct = 0x109f, .indexed = 0x10af, .extended = 0x10bf },
	{ .op = "lds",  .type = IMM16|MEM, .immediate = 0x10ce, .direct = 0x10de, .indexed = 0x10ee, .extended = 0x10fe },
	{ .op = "sts",  .type = MEM,                            .direct = 0x10df, .indexed = 0x10ef, .extended = 0x10ff },

	{ .op = "swi3", .type = INHERENT, .immediate = 0x113f },

	{ .op = "cmpu", .type = IMM16|MEM, .immediate = 0x1183, .direct = 0x1193, .indexed = 0x11a3, .extended = 0x11b3 },
	{ .op = "cmps", .type = IMM16|MEM, .immediate = 0x118c, .direct = 0x119c, .indexed = 0x11ac, .extended = 0x11bc },

};

/* HD6309 opcode table. */

static struct opcode const opcodes_6309[] = {

	{ .op = "oim", .type = IMM8_MEM, .direct = 0x01, .indexed = 0x61, .extended = 0x71 },
	{ .op = "aim", .type = IMM8_MEM, .direct = 0x02, .indexed = 0x62, .extended = 0x72 },
	{ .op = "eim", .type = IMM8_MEM, .direct = 0x05, .indexed = 0x65, .extended = 0x75 },
	{ .op = "tim", .type = IMM8_MEM, .direct = 0x0b, .indexed = 0x6b, .extended = 0x7b },

	{ .op = "sexw", .type = INHERENT, .immediate = 0x14 },

	{ .op = "ldq",  .type = IMM32|MEM,  .immediate = 0xcd, .direct = 0x10dc, .indexed = 0x10ec, .extended = 0x10fc },

	{ .op = "addr", .type = PAIR, .immediate = 0x1030 },
	{ .op = "adcr", .type = PAIR, .immediate = 0x1031 },
	{ .op = "subr", .type = PAIR, .immediate = 0x1032 },
	{ .op = "sbcr", .type = PAIR, .immediate = 0x1033 },
	{ .op = "andr", .type = PAIR, .immediate = 0x1034 },
	{ .op = "orr",  .type = PAIR, .immediate = 0x1035 },
	{ .op = "eorr", .type = PAIR, .immediate = 0x1036 },
	{ .op = "cmpr", .type = PAIR, .immediate = 0x1037 },
	{ .op = "pshsw", .type = INHERENT, .immediate = 0x1038 },
	{ .op = "pulsw", .type = INHERENT, .immediate = 0x1039 },
	{ .op = "pshuw", .type = INHERENT, .immediate = 0x103a },
	{ .op = "puluw", .type = INHERENT, .immediate = 0x103b },

	{ .op = "negd", .type = INHERENT, .immediate = 0x1040 },
	{ .op = "comd", .type = INHERENT, .immediate = 0x1043 },
	{ .op = "lsrd", .type = INHERENT, .immediate = 0x1044 },
	{ .op = "rord", .type = INHERENT, .immediate = 0x1046 },
	{ .op = "asrd", .type = INHERENT, .immediate = 0x1047 },
	{ .op = "lsld", .type = INHERENT, .immediate = 0x1048 },
	{ .op = "asld", .type = INHERENT, .immediate = 0x1048 },
	{ .op = "rold", .type = INHERENT, .immediate = 0x1049 },
	{ .op = "decd", .type = INHERENT, .immediate = 0x104a },
	{ .op = "incd", .type = INHERENT, .immediate = 0x104c },
	{ .op = "tstd", .type = INHERENT, .immediate = 0x104d },
	{ .op = "clrd", .type = INHERENT, .immediate = 0x104f },

	{ .op = "comw", .type = INHERENT, .immediate = 0x1053 },
	{ .op = "lsrw", .type = INHERENT, .immediate = 0x1054 },
	{ .op = "rorw", .type = INHERENT, .immediate = 0x1056 },
	{ .op = "rolw", .type = INHERENT, .immediate = 0x1059 },
	{ .op = "decw", .type = INHERENT, .immediate = 0x105a },
	{ .op = "incw", .type = INHERENT, .immediate = 0x105c },
	{ .op = "tstw", .type = INHERENT, .immediate = 0x105d },
	{ .op = "clrw", .type = INHERENT, .immediate = 0x105f },

	{ .op = "subw", .type = IMM16|MEM,  .immediate = 0x1080, .direct = 0x1090, .indexed = 0x10a0, .extended = 0x10b0 },
	{ .op = "cmpw", .type = IMM16|MEM,  .immediate = 0x1081, .direct = 0x1091, .indexed = 0x10a1, .extended = 0x10b1 },
	{ .op = "sbcd", .type = IMM16|MEM,  .immediate = 0x1082, .direct = 0x1092, .indexed = 0x10a2, .extended = 0x10b2 },
	{ .op = "andd", .type = IMM16|MEM,  .immediate = 0x1084, .direct = 0x1094, .indexed = 0x10a4, .extended = 0x10b4 },
	{ .op = "bitd", .type = IMM16|MEM,  .immediate = 0x1085, .direct = 0x1095, .indexed = 0x10a5, .extended = 0x10b5 },
	{ .op = "ldw",  .type = IMM16|MEM,  .immediate = 0x1086, .direct = 0x1096, .indexed = 0x10a6, .extended = 0x10b6 },
	{ .op = "stw",  .type = MEM,                             .direct = 0x1097, .indexed = 0x10a7, .extended = 0x10b7 },
	{ .op = "eord", .type = IMM16|MEM,  .immediate = 0x1088, .direct = 0x1098, .indexed = 0x10a8, .extended = 0x10b8 },
	{ .op = "adcd", .type = IMM16|MEM,  .immediate = 0x1089, .direct = 0x1099, .indexed = 0x10a9, .extended = 0x10b9 },
	{ .op = "ord",  .type = IMM16|MEM,  .immediate = 0x108a, .direct = 0x109a, .indexed = 0x10aa, .extended = 0x10ba },
	{ .op = "addw", .type = IMM16|MEM,  .immediate = 0x108b, .direct = 0x109b, .indexed = 0x10ab, .extended = 0x10bb },

	{ .op = "stq", .type = MEM, .direct = 0x10dd, .indexed = 0x10ed, .extended = 0x10fd },

	{ .op = "band",  .type = REG_MEM, .direct = 0x1130 },
	{ .op = "biand", .type = REG_MEM, .direct = 0x1131 },
	{ .op = "bor",   .type = REG_MEM, .direct = 0x1132 },
	{ .op = "bior",  .type = REG_MEM, .direct = 0x1133 },
	{ .op = "beor",  .type = REG_MEM, .direct = 0x1134 },
	{ .op = "bieor", .type = REG_MEM, .direct = 0x1135 },
	{ .op = "ldbt",  .type = REG_MEM, .direct = 0x1136 },
	{ .op = "stbt",  .type = REG_MEM, .direct = 0x1137 },
	{ .op = "tfm", .type = TFM, .immediate = 0x1138 },
	{ .op = "bitmd", .type = IMM8, .immediate = 0x113c },
	{ .op = "ldmd", .type = IMM8, .immediate = 0x113d },

	{ .op = "come", .type = INHERENT, .immediate = 0x1143 },
	{ .op = "dece", .type = INHERENT, .immediate = 0x114a },
	{ .op = "ince", .type = INHERENT, .immediate = 0x114c },
	{ .op = "tste", .type = INHERENT, .immediate = 0x114d },
	{ .op = "clre", .type = INHERENT, .immediate = 0x114f },

	{ .op = "comf", .type = INHERENT, .immediate = 0x1153 },
	{ .op = "decf", .type = INHERENT, .immediate = 0x115a },
	{ .op = "incf", .type = INHERENT, .immediate = 0x115c },
	{ .op = "tstf", .type = INHERENT, .immediate = 0x115d },
	{ .op = "clrf", .type = INHERENT, .immediate = 0x115f },

	{ .op = "sube", .type = IMM8|MEM, .immediate = 0x1180, .direct = 0x1190, .indexed = 0x11a0, .extended = 0x11b0 },
	{ .op = "cmpe", .type = IMM8|MEM, .immediate = 0x1181, .direct = 0x1191, .indexed = 0x11a1, .extended = 0x11b1 },
	{ .op = "lde",  .type = IMM8|MEM, .immediate = 0x1186, .direct = 0x1196, .indexed = 0x11a6, .extended = 0x11b6 },
	{ .op = "ste",  .type = MEM,                           .direct = 0x1197, .indexed = 0x11a7, .extended = 0x11b7 },
	{ .op = "adde", .type = IMM8|MEM, .immediate = 0x118b, .direct = 0x119b, .indexed = 0x11ab, .extended = 0x11bb },
	{ .op = "divd", .type = IMM8|MEM,  .immediate = 0x118d, .direct = 0x119d, .indexed = 0x11ad, .extended = 0x11bd },
	{ .op = "divq", .type = IMM16|MEM, .immediate = 0x118e, .direct = 0x119e, .indexed = 0x11ae, .extended = 0x11be },
	{ .op = "muld", .type = IMM16|MEM, .immediate = 0x118f, .direct = 0x119f, .indexed = 0x11af, .extended = 0x11bf },

	{ .op = "subf", .type = IMM8|MEM, .immediate = 0x11c0, .direct = 0x11d0, .indexed = 0x11e0, .extended = 0x11f0 },
	{ .op = "cmpf", .type = IMM8|MEM, .immediate = 0x11c1, .direct = 0x11d1, .indexed = 0x11e1, .extended = 0x11f1 },
	{ .op = "ldf",  .type = IMM8|MEM, .immediate = 0x11c6, .direct = 0x11d6, .indexed = 0x11e6, .extended = 0x11f6 },
	{ .op = "stf",  .type = MEM,                           .direct = 0x11d7, .indexed = 0x11e7, .extended = 0x11f7 },
	{ .op = "addf", .type = IMM8|MEM, .immediate = 0x11cb, .direct = 0x11db, .indexed = 0x11eb, .extended = 0x11fb },

};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static struct dict *opcodes = NULL;

void opcode_init(void) {
	opcodes = dict_new(dict_str_hash_case, dict_str_equal_case);
	switch (asm6809_options.isa) {
	case asm6809_isa_6309:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(opcodes_6309); i++) {
			dict_insert(opcodes, (void *)opcodes_6309[i].op, (void *)&opcodes_6309[i]);
		}
		/* fall through */
	case asm6809_isa_6809:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(opcodes_6809); i++) {
			dict_insert(opcodes, (void *)opcodes_6809[i].op, (void *)&opcodes_6809[i]);
		}
		break;
	default:
		break;
	}
}

void opcode_free_all(void) {
	if (!opcodes)
		return;
	dict_destroy(opcodes);
	opcodes = NULL;
}

struct opcode const *opcode_by_name(const char *name) {
	return dict_lookup(opcodes, name);
}
