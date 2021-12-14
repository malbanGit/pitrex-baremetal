


all: starwars

starwars: starwars-bank0.c starwars-unbanked.c starwars.c
	cc -Wall -g -Wno-unused-label -o starwars starwars.c

starwars-bank0.bin:
	split -b 8192 --additional-suffix .bin --suffix-length 1 -d roms/136021.214 starwars-bank

starwars-unbanked.bin: 
	cat roms/136021.102 roms/136021.203 roms/136021.104 roms/136021.206 >starwars-unbanked.bin

starwars-sound.bin:	
	cat roms/136021.107 roms/136021.208 >starwars-sound.bin

starwars-bank0.c: starwars-bank0.bin
#	../6809dasm --sbt --ss --timing --org 6000 starwars-bank0.bin starwars-bank0.dat > starwars-bank0.c
	../6809dasm --sbt  -i2  --timing --org 6000 starwars-bank0.bin starwars-bank0.dat > starwars-bank0.c
	../6809dasm --sbt -l -i2   --timing --org 6000 starwars-bank1.bin starwars-bank1.dat > starwars-bank1.c
#	../6809dasm --sbt  --ss  --timing --org 6000 starwars-bank0.bin starwars-bank0.dat > starwars-bank0.c
#	../6809dasm --sbt -l --ss   --timing --org 6000 starwars-bank1.bin starwars-bank1.dat > starwars-bank1.c
	../6809dasm                      --org 6000 starwars-bank0.bin starwars-bank0.dat > starwars-bank0.asm
	../6809dasm                      --org 6000 starwars-bank1.bin starwars-bank1.dat > starwars-bank1.asm

starwars-unbanked.c: starwars-unbanked.bin
#	../6809dasm --sbt --ss --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked.c
	../6809dasm --sbt    -i2   --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked.c
	../6809dasm --sbt -l -i2   --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked1.c
#	../6809dasm --sbt    --ss   --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked.c
#	../6809dasm --sbt -l --ss   --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked1.c
	../6809dasm                     --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked.asm
#	../6809dasm --sbt  --ss --timing --org 8000 starwars-unbanked.bin starwars-unbanked.dat > starwars-unbanked1.c
	
clean: 
	$(RM) starwars-unbanked.bin
	$(RM) starwars-unbanked.c
	$(RM) starwars-unbanked1.c
	$(RM) starwars-unbanked.asm
	$(RM) starwars-unbanked.dis
	$(RM) starwars-unbanked.tags
	$(RM) starwars-bank0.bin
	$(RM) starwars-bank0.c
	$(RM) starwars-bank0.asm
	$(RM) starwars-bank0.dis
	$(RM) starwars-bank0.tags
	$(RM) starwars-bank1.bin
	$(RM) starwars-bank1.c
	$(RM) starwars-bank1.asm
	$(RM) starwars-bank1.dis
	$(RM) starwars-bank1.tags
	$(RM) starwars-sound.bin
	$(RM) starwars-sound.c
	$(RM) starwars-sound.asm
	$(RM) starwars-sound.dis
	$(RM) starwars-sound.tags

upload:
	scp starwars-bank0.c starwars-bank0.asm starwars-bank1.c starwars-bank1.asm starwars-unbanked.c starwars-unbanked.asm starwars.c gtoal@gtoal.com:gtoal.com/for_malban/starwars/
