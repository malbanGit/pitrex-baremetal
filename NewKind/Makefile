BUILD_FOR ?= zero1
BASE_DIR = ..
TARGET_BASE = newkind
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS = $(BUILD_DIR)alg_gfx.o $(BUILD_DIR)alg_main.o $(BUILD_DIR)docked.o $(BUILD_DIR)elite.o \
	$(BUILD_DIR)intro.o $(BUILD_DIR)planet.o $(BUILD_DIR)shipdata.o $(BUILD_DIR)shipface.o $(BUILD_DIR)sound.o $(BUILD_DIR)space.o \
	$(BUILD_DIR)swat.o $(BUILD_DIR)threed.o $(BUILD_DIR)vector.o $(BUILD_DIR)random.o $(BUILD_DIR)trade.o $(BUILD_DIR)options.o \
	$(BUILD_DIR)stars.o $(BUILD_DIR)missions.o $(BUILD_DIR)pilot.o $(BUILD_DIR)file.o $(BUILD_DIR)keyboard.o $(BUILD_DIR)allegro_dummy.o

$(BUILD_DIR)allegro_dummy.o: allegro_dummy.c allegro_dummy.h
	$(CC) $(CFLAGS) $(allegro_dummy) -c allegro_dummy.c -o $(BUILD_DIR)allegro_dummy.o

$(BUILD_DIR)alg_gfx.o: alg_gfx.c alg_data.h config.h elite.h planet.h gfx.h
	$(CC) $(CFLAGS) $(alg_gfx) -c alg_gfx.c -o $(BUILD_DIR)alg_gfx.o

$(BUILD_DIR)alg_main.o: alg_main.c alg_data.h config.h elite.h planet.h gfx.h docked.h\
	intro.h shipdata.h shipface.h space.h main.h pilot.h file.h keyboard.h
	$(CC) $(CFLAGS) -c alg_main.c -o $(BUILD_DIR)alg_main.o

$(BUILD_DIR)docked.o: docked.c config.h elite.h planet.h gfx.h
	$(CC) $(CFLAGS) -c docked.c -o $(BUILD_DIR)docked.o

$(BUILD_DIR)elite.o: elite.c config.h elite.h planet.h vector.h shipdata.h
	$(CC) $(CFLAGS) -c elite.c -o $(BUILD_DIR)elite.o

$(BUILD_DIR)intro.o: intro.c space.h config.h elite.h planet.h gfx.h vector.h\
	shipdata.h shipface.h threed.h
	$(CC) $(CFLAGS) -c intro.c -o $(BUILD_DIR)intro.o

$(BUILD_DIR)planet.o: planet.c config.h elite.h planet.h
	$(CC) $(CFLAGS) -c planet.c -o $(BUILD_DIR)planet.o

$(BUILD_DIR)shipdata.o: shipdata.c shipdata.h vector.h
	$(CC) $(CFLAGS) -c shipdata.c -o $(BUILD_DIR)shipdata.o

$(BUILD_DIR)shipface.o: shipface.c config.h elite.h planet.h shipface.h gfx.h
	$(CC) $(CFLAGS) -c shipface.c -o $(BUILD_DIR)shipface.o

$(BUILD_DIR)threed.o: threed.c space.h config.h elite.h planet.h gfx.h vector.h shipdata.h\
	shipface.h threed.h
	$(CC) $(CFLAGS) -c threed.c -o $(BUILD_DIR)threed.o

$(BUILD_DIR)vector.o: vector.c config.h vector.h
	$(CC) $(CFLAGS) -c vector.c -o $(BUILD_DIR)vector.o

$(BUILD_DIR)sound.o: sound.c sound.h
	$(CC) $(CFLAGS) -c sound.c -o $(BUILD_DIR)sound.o

$(BUILD_DIR)space.o: space.c space.h vector.h alg_data.h config.h elite.h planet.h\
	gfx.h docked.h intro.h shipdata.h shipface.h main.h random.h
	$(CC) $(CFLAGS) -c space.c -o $(BUILD_DIR)space.o

$(BUILD_DIR)swat.o: swat.c swat.h elite.h config.h main.h gfx.h alg_data.h shipdata.h\
	random.h pilot.h
	$(CC) $(CFLAGS) -c swat.c -o $(BUILD_DIR)swat.o

$(BUILD_DIR)random.o: random.c random.h
	$(CC) $(CFLAGS) -c random.c -o $(BUILD_DIR)random.o

$(BUILD_DIR)trade.o: trade.c trade.h elite.h config.h
	$(CC) $(CFLAGS) -c trade.c -o $(BUILD_DIR)trade.o

$(BUILD_DIR)options.o: options.c options.h elite.h config.h gfx.h file.h
	$(CC) $(CFLAGS) -c options.c -o $(BUILD_DIR)options.o

$(BUILD_DIR)stars.o: stars.c stars.h elite.h config.h gfx.h random.h
	$(CC) $(CFLAGS) -c stars.c -o $(BUILD_DIR)stars.o

$(BUILD_DIR)missions.o: missions.c missions.h config.h elite.h gfx.h planet.h main.h\
	vector.h space.h
	$(CC) $(CFLAGS) -c missions.c -o $(BUILD_DIR)missions.o

$(BUILD_DIR)pilot.o: pilot.c pilot.h config.h elite.h gfx.h vector.h space.h main.h
	$(CC) $(CFLAGS) -c pilot.c -o $(BUILD_DIR)pilot.o

$(BUILD_DIR)file.o: file.c file.h config.h elite.h
	$(CC) $(CFLAGS) -c file.c -o $(BUILD_DIR)file.o

$(BUILD_DIR)keyboard.o: keyboard.c keyboard.h
	$(CC) $(CFLAGS) -c keyboard.c -o $(BUILD_DIR)keyboard.o

$(TARGET): $(OBJS) $(LIBS_CHECK)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBS) 
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
