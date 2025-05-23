BUILD_FOR ?= zero1
TARGET_BASE = bz2
BASE_DIR = ..
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

# get rid of __dso_handle (dynamic shared object, destructor)
CFLAGS := $(CFLAGS) -fno-use-cxa-atexit

OBJS :=  $(BUILD_DIR)BZ2.o $(BUILD_DIR)scene.o $(BUILD_DIR)controls.o $(BUILD_DIR)enemies.o $(BUILD_DIR)enemy.o $(BUILD_DIR)entity.o  \
	 $(BUILD_DIR)entityObstruction.o \
	 $(BUILD_DIR)entityObstructionCube.o \
	 $(BUILD_DIR)entityObstructionPyramid.o \
	 $(BUILD_DIR)entityObstructionShortCube.o \
	 $(BUILD_DIR)entityProjectile.o \
	 $(BUILD_DIR)entitySaucer.o \
	 $(BUILD_DIR)entitySupertank.o \
	 $(BUILD_DIR)entityTank.o \
	 $(BUILD_DIR)entityWreckage.o \
	 $(BUILD_DIR)entityLogo.o  \
	 $(BUILD_DIR)entityMissile.o \
	 $(BUILD_DIR)wreckage.o $(BUILD_DIR)vector.o \
	 $(BUILD_DIR)spawner.o $(BUILD_DIR)sound.o $(BUILD_DIR)projectiles.o $(BUILD_DIR)player.o $(BUILD_DIR)players.o \
	 $(BUILD_DIR)particle2d.o $(BUILD_DIR)particle.o $(BUILD_DIR)obstructions.o $(BUILD_DIR)objloader.o $(BUILD_DIR)mountains.o $(BUILD_DIR)model.o \
	 $(BUILD_DIR)mathutils.o $(BUILD_DIR)hud.o $(BUILD_DIR)highscore.o $(BUILD_DIR)font.o $(BUILD_DIR)game.o $(BUILD_DIR)camera.o
	 
$(BUILD_DIR)BZ2.o: BZ2.cpp  BZ2.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)BZ2.o -c BZ2.cpp 

$(BUILD_DIR)entityObstructionPyramid.o: entityObstructionPyramid.cpp  entityObstructionPyramid.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityObstructionPyramid.o -c entityObstructionPyramid.cpp 

$(BUILD_DIR)wreckage.o: wreckage.cpp  wreckage.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)wreckage.o -c wreckage.cpp 

$(BUILD_DIR)vector.o: vector.cpp  vector.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)vector.o -c vector.cpp 

$(BUILD_DIR)spawner.o: spawner.cpp  spawner.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)spawner.o -c spawner.cpp 

$(BUILD_DIR)sound.o: sound.cpp  sound.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)sound.o -c sound.cpp 

$(BUILD_DIR)projectiles.o: projectiles.cpp  projectiles.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)projectiles.o -c projectiles.cpp 

$(BUILD_DIR)players.o: players.cpp  players.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)players.o -c players.cpp 

$(BUILD_DIR)player.o: player.cpp  player.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)player.o -c player.cpp 

$(BUILD_DIR)particle2d.o: particle2d.cpp  particle2d.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)particle2d.o -c particle2d.cpp 

$(BUILD_DIR)particle.o: particle.cpp  particle.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)particle.o -c particle.cpp 

$(BUILD_DIR)obstructions.o: obstructions.cpp  obstructions.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)obstructions.o -c obstructions.cpp 

$(BUILD_DIR)objloader.o: objloader.cpp  objloader.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)objloader.o -c objloader.cpp 

$(BUILD_DIR)mountains.o: mountains.cpp  mountains.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)mountains.o -c mountains.cpp 

$(BUILD_DIR)model.o: model.cpp  model.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)model.o -c model.cpp 

$(BUILD_DIR)mathutils.o: mathutils.cpp  mathutils.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)mathutils.o -c mathutils.cpp 

$(BUILD_DIR)hud.o: hud.cpp  hud.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)hud.o -c hud.cpp 

$(BUILD_DIR)highscore.o: highscore.cpp  highscore.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)highscore.o -c highscore.cpp 

$(BUILD_DIR)font.o: font.cpp  font.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)font.o -c font.cpp 

$(BUILD_DIR)scene.o: scene.cpp  scene.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)scene.o -c scene.cpp 

$(BUILD_DIR)game.o: game.cpp  game.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)game.o -c game.cpp 

$(BUILD_DIR)camera.o: camera.cpp  camera.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)camera.o -c camera.cpp 

$(BUILD_DIR)enemies.o: enemies.cpp  enemies.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)enemies.o -c enemies.cpp 

$(BUILD_DIR)enemy.o: enemy.cpp  enemy.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)enemy.o -c enemy.cpp 

$(BUILD_DIR)entity.o: entity.cpp  entity.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entity.o -c entity.cpp 

$(BUILD_DIR)entityCollection.o: entityCollection.cpp
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityCollection.o -c entityCollection.cpp 

$(BUILD_DIR)entityCollectionTank.o: entityCollectionTank.cpp
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityCollectionTank.o -c entityCollectionTank.cpp 

$(BUILD_DIR)entityLogo.o: entityLogo.cpp  entityLogo.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityLogo.o -c entityLogo.cpp 

$(BUILD_DIR)entityMissile.o: entityMissile.cpp  entityMissile.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityMissile.o -c entityMissile.cpp 

$(BUILD_DIR)entityObstruction.o: entityObstruction.cpp  entityObstruction.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityObstruction.o -c entityObstruction.cpp 

$(BUILD_DIR)entityObstructionCube.o: entityObstructionCube.cpp  entityObstructionCube.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityObstructionCube.o -c entityObstructionCube.cpp 

$(BUILD_DIR)entityObstructionShortCube.o: entityObstructionShortCube.cpp  entityObstructionShortCube.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityObstructionShortCube.o -c entityObstructionShortCube.cpp 

$(BUILD_DIR)entityProjectile.o: entityProjectile.cpp  entityProjectile.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityProjectile.o -c entityProjectile.cpp 

$(BUILD_DIR)entitySaucer.o: entitySaucer.cpp  entitySaucer.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entitySaucer.o -c entitySaucer.cpp 

$(BUILD_DIR)entitySupertank.o: entitySupertank.cpp  entitySupertank.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entitySupertank.o -c entitySupertank.cpp 

$(BUILD_DIR)entityTank.o: entityTank.cpp  entityTank.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityTank.o -c entityTank.cpp 

$(BUILD_DIR)entityWreckage.o: entityWreckage.cpp  entityWreckage.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)entityWreckage.o -c entityWreckage.cpp 

$(BUILD_DIR)controls.o: controls.cpp  controls.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)controls.o -c controls.cpp 

$(TARGET): $(OBJS)  $(LIBS_CHECK)
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS) $(LIBSGCC)	
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)$(TARGET_BASE).elf.img > $(BUILD_DIR)$(TARGET_BASE).elf.img.s
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
