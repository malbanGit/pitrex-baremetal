# this makefile MUST be invoked from the parent directory
# because it is important that 6809dasm is up to date...

all: ctestall
	@./bootstrap
	@./ctestall --dry-run > /dev/null 2> /dev/null
	@rm -f *.o ctestall tests.out tests.err
	@cc -Wall -Wno-unused-label -g -DSBT -c instructions.c
	@cc -Wall -Wno-unused-label -g -DSBT -c initregs.c
	@cc -Wall -Wno-unused-label -g -DSBT -o ctestall framework.c instructions.o initregs.o
	-@./ctestall > tests.out 2> tests.err
	@if [ "`cat tests.err`" != "************ opcode tests successful *************" ] ; then tail -8 tests.out | head -4 ; cat tests.err ; exit 1 ; fi
	@cat tests.err; cp code/*.c lastcode/

ctestall-fallback: framework.c instructions-em.o initregs.o 6809-fallback.o ../monitor-fallback.o
	cc -Wall -Wno-unused-label -g -DEMULATOR -o ctestall-fallback framework.c instructions-em.o initregs.o 6809-fallback.o ../monitor-fallback.o
	-./ctestall-fallback > tests.out 2> tests.err
	@if [ "`cat tests.err`" != "************ opcode tests successful *************" ] ; then tail -8 tests.out | head -4 ; cat tests.err ; exit 1 ; fi
	cat tests.err

6809-fallback.o: ../6809.c ../6809-fallback.c ../6809-fallback.h
	cc -Wall -g -c -DSOREN_ROUG_TEST -I.. ../6809-fallback.c

instructions.o: instructions.c cputest.h initregs.h framework.h
	@./bootstrap
	@cc -Wall -Wno-unused-label -g -DSBT -c instructions.c

instructions-em.o: instructions.c cputest.h initregs.h framework.h
	@rm -f code/*.c
	@./bootstrap
	@cc -Wall -Wno-unused-label -g -DEMULATOR -c -o instructions-em.o instructions.c

initregs.o: initregs.c cputest.h
	@cc -Wall -Wno-unused-label -g -DSBT -c initregs.c

framework.o: framework.c cputest.h framework.h
	@cc -Wall -Wno-unused-label -g -DSBT -c framework.c

ctestall: instructions.o framework.o initregs.o
	@cc -Wall -g -o ctestall instructions.o framework.o initregs.o

clean:
	@rm -f *~ *.o ctestall ctestall-fallback code/*.c

veryclean:
	@rm -f *.o ctestall code/*.c lastcode/*.o

upload:
	scp *.[hc] Makefile bootstrap README.md gtoal@gtoal.com:gtoal.com/SBTPROJECT/6809sbt/sorenroug-cpu-tests/
