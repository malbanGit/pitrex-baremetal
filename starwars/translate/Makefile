all: sbt fallback

sbt: 6809dasm asm6809-2.12/src/asm6809
	@./asm6809-2.12/src/asm6809 -B -o cputest.bin -l cputest.lis -s cputest.sym cputest.asm
	./6809dasm --sbt --ss -i1 cputest.bin cputest.dat > cputest.c
	@echo Did the generated code change\?:
	@ 1>&2 make -C tests > /dev/null
	@cc -o CPUtester CPUtester.c
	@echo CPUtester:
	@./CPUtester 2> cputest.out
	@echo Soren Roug tests:
	@make -C sorenroug-cpu-tests 2> /dev/null | grep -v "Error 1 (ignored)" | grep -v "make\[[0-9]*\]: [EL][a-z]*ing directory"
	@make -C sorenroug-cpu-tests ctestall-fallback
	@echo Build starwars:
	@make -C starwars > /dev/null
	@echo All up to date

fallback: CPUtester-fallback.c 6809dasm.h 6809-fallback.o tags.o monitor-fallback.o
	@./asm6809-2.12/src/asm6809 -B -o cputest.bin -l cputest.lis -s cputest.sym cputest.asm
	./6809dasm --sbt --ss cputest.bin cputest.dat > cputest.c
	cc -o CPUtester-fallback CPUtester-fallback.c tags.o 6809-fallback.o monitor-fallback.o
	@echo CPUtester-fallback:
	./CPUtester-fallback # 2> cputester-fallback.out 

CPUtester: CPUtester.c 6809dasm asm6809-2.12/src/asm6809
	@./asm6809-2.12/src/asm6809 -B -o cputest.bin -l cputest.lis -s cputest.sym cputest.asm
	./6809dasm --sbt --ss cputest.bin cputest.dat > cputest.c
	@cc -o CPUtester CPUtester.c
	@./CPUtester -d 0

asm6809-2.12/src/asm6809:
	make -C asm6809-2.12

6809dasm: 6809dasm.o tags.o 6809.o vectrex-regs.o dasm09/dasm09.o hook.o
	cc -g -o 6809dasm hook.o 6809dasm.o tags.o 6809.o vectrex-regs.o dasm09/dasm09.o

dasm09/dasm09.o: dasm09/dasm09.c
	cc -c -o dasm09/dasm09.o dasm09/dasm09.c

6809-fallback.o: 6809.c 6809-fallback.c 6809-fallback.h
	cc -Wall -g -c -DCPUTEST 6809-fallback.c

6809.o: 6809.c 6809.h
	cc -Wall -g -c 6809.c

hook.o: hook.c 6809.h
	cc -Wall -g -c hook.c

monitor.o: monitor.c monitor.h
	cc -Wall -g -c monitor.c

vectrex-regs.o: vectrex-regs.c 6809.h
	cc -Wall -g -c vectrex-regs.c

6809dasm.o: 6809dasm.c 6809.h 6809dasm.h
	cc -Wall -g -c 6809dasm.c

tags.o: tags.c 6809dasm.h
	cc -Wall -g -c tags.c

clean:
	rm -f *~ *.o 6809dasm CPUtester tests/*.new dasm09/dasm09.o sim6809 CPUtester-fallback
	make -C sorenroug-cpu-tests clean
	make -C asm6809-2.12 clean

upload: clean
	make -C sorenroug-cpu-tests upload
	./upload.sh
