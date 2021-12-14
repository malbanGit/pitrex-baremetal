all:
	@mv opcode-test.c opcode-test.c.PREV
	@mv opcode-test.asm opcode-test.asm.PREV
	@../6809dasm opcode-test.vec opcode-test.dat > opcode-test.asm
	@../6809dasm --sbt --ss --timing opcode-test.vec opcode-test.dat > opcode-test.c
	@ 1>&2 diff opcode-test.asm opcode-test.asm.PREV ; exit 0
	@ 1>&2 diff opcode-test.c opcode-test.c.PREV ; exit 0
