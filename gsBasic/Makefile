APP_NAME := gsbasic

BUILD_FOR ?= zero1
BASE_DIR = ..
VECTREX32_DIR := ./vectrex32/
TARGET_BASE = $(APP_NAME)
TARGET	= $(TARGET_BASE).img

include ../Rules.mk

OBJS_BASIC :=  $(BUILD_DIR)cppWrapper.o $(BUILD_DIR)GSBString.o $(BUILD_DIR)Error.o $(BUILD_DIR)Utilities.o \
	$(BUILD_DIR)AST.o $(BUILD_DIR)Value.o $(BUILD_DIR)BuiltInConstants.o $(BUILD_DIR)StructDef.o $(BUILD_DIR)Environment.o \
	$(BUILD_DIR)TypeBehaviors.o $(BUILD_DIR)VarSpec.o $(BUILD_DIR)CallSpec.o $(BUILD_DIR)BuiltInFunctions.o $(BUILD_DIR)Executive.o \
	$(BUILD_DIR)Lexer.o $(BUILD_DIR)Parser.o $(BUILD_DIR)Token.o $(BUILD_DIR)Statements.o $(BUILD_DIR)Expression.o $(BUILD_DIR)Constant.o \
	$(BUILD_DIR)ExpressionList.o $(BUILD_DIR)ArrayInitializer.o $(BUILD_DIR)Heap.o $(BUILD_DIR)PlacementNews.o $(BUILD_DIR)GSBasic.o \
	$(BUILD_DIR)stdafx.o $(BUILD_DIR)vio.o

OBJS_VECTREX32 :=   $(BUILD_DIR)BASICBuiltInFunctions.o $(BUILD_DIR)BASICBuiltInConstants.o $(BUILD_DIR)VectrexSound.o \
	$(BUILD_DIR)VectrexCommand.o $(BUILD_DIR)Matrix.o $(BUILD_DIR)Calculations.o $(BUILD_DIR)Camera.o $(BUILD_DIR)VectorFont.o 

$(BUILD_DIR)VectorFont.o: $(VECTREX32_DIR)VectorFont.cpp  $(VECTREX32_DIR)VectorFont.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)VectorFont.o -c $(VECTREX32_DIR)VectorFont.cpp 

$(BUILD_DIR)VectrexCommand.o: $(VECTREX32_DIR)VectrexCommand.cpp  $(VECTREX32_DIR)VectrexCommand.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)VectrexCommand.o -c $(VECTREX32_DIR)VectrexCommand.cpp 

$(BUILD_DIR)Calculations.o: $(VECTREX32_DIR)Calculations.cpp  $(VECTREX32_DIR)Calculations.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Calculations.o -c $(VECTREX32_DIR)Calculations.cpp 

$(BUILD_DIR)Matrix.o: $(VECTREX32_DIR)Matrix.cpp  $(VECTREX32_DIR)Matrix.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Matrix.o -c $(VECTREX32_DIR)Matrix.cpp 

$(BUILD_DIR)BASICBuiltInFunctions.o: $(VECTREX32_DIR)BASICBuiltInFunctions.cpp
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)BASICBuiltInFunctions.o -c $(VECTREX32_DIR)BASICBuiltInFunctions.cpp 

$(BUILD_DIR)BASICBuiltInConstants.o: $(VECTREX32_DIR)BASICBuiltInConstants.cpp $(VECTREX32_DIR)BASICBuiltInConstants.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)BASICBuiltInConstants.o -c $(VECTREX32_DIR)BASICBuiltInConstants.cpp 

$(BUILD_DIR)VectrexSound.o: $(VECTREX32_DIR)VectrexSound.cpp $(VECTREX32_DIR)VectrexSound.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)VectrexSound.o -c $(VECTREX32_DIR)VectrexSound.cpp 

$(BUILD_DIR)Camera.o: $(VECTREX32_DIR)Camera.cpp $(VECTREX32_DIR)Camera.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Camera.o -c $(VECTREX32_DIR)Camera.cpp 

$(BUILD_DIR)cppWrapper.o: cppWrapper.c cppWrapper.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)cppWrapper.o -c cppWrapper.c 

$(BUILD_DIR)vio.o: vio.c vio.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vio.o -c vio.c

$(BUILD_DIR)stdafx.o: stdafx.cpp 
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)stdafx.o -c stdafx.cpp

$(BUILD_DIR)GSBasic.o: GSBasic.cpp 
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)GSBasic.o -c GSBasic.cpp

$(BUILD_DIR)PlacementNews.o: PlacementNews.cpp 
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)PlacementNews.o -c PlacementNews.cpp

$(BUILD_DIR)Heap.o: Heap.cpp Heap.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Heap.o -c Heap.cpp

$(BUILD_DIR)ExpressionList.o: ExpressionList.cpp ExpressionList.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)ExpressionList.o -c ExpressionList.cpp

$(BUILD_DIR)ArrayInitializer.o: ArrayInitializer.cpp ArrayInitializer.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)ArrayInitializer.o -c ArrayInitializer.cpp

$(BUILD_DIR)Constant.o: Constant.cpp Constant.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Constant.o -c Constant.cpp

$(BUILD_DIR)Expression.o: Expression.cpp Expression.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Expression.o -c Expression.cpp

$(BUILD_DIR)Statements.o: Statements.cpp Statements.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Statements.o -c Statements.cpp

$(BUILD_DIR)Token.o: Token.cpp Token.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Token.o -c Token.cpp

$(BUILD_DIR)Parser.o: Parser.cpp Parser.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Parser.o -c Parser.cpp

$(BUILD_DIR)Lexer.o: Lexer.cpp Lexer.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Lexer.o -c Lexer.cpp

$(BUILD_DIR)Executive.o: Executive.cpp Executive.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Executive.o -c Executive.cpp

$(BUILD_DIR)CallSpec.o: CallSpec.cpp CallSpec.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)CallSpec.o -c CallSpec.cpp

$(BUILD_DIR)BuiltInFunctions.o: BuiltInFunctions.cpp BuiltInFunctions.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)BuiltInFunctions.o -c BuiltInFunctions.cpp

$(BUILD_DIR)VarSpec.o: VarSpec.cpp VarSpec.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)VarSpec.o -c VarSpec.cpp

$(BUILD_DIR)TypeBehaviors.o: TypeBehaviors.cpp TypeBehaviors.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)TypeBehaviors.o -c TypeBehaviors.cpp

$(BUILD_DIR)Environment.o: Environment.cpp Environment.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Environment.o -c Environment.cpp

$(BUILD_DIR)StructDef.o: StructDef.cpp StructDef.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)StructDef.o -c StructDef.cpp

$(BUILD_DIR)BuiltInConstants.o: BuiltInConstants.cpp BuiltInConstants.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)BuiltInConstants.o -c BuiltInConstants.cpp

$(BUILD_DIR)Value.o: Value.cpp Value.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Value.o -c Value.cpp

$(BUILD_DIR)AST.o: AST.cpp AST.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)AST.o -c AST.cpp

$(BUILD_DIR)Utilities.o: Utilities.cpp Utilities.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Utilities.o -c Utilities.cpp

$(BUILD_DIR)Error.o: Error.cpp Error.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)Error.o -c Error.cpp

$(BUILD_DIR)GSBString.o: GSBString.cpp GSBString.h
	$(CPP) $(CFLAGS) -o $(BUILD_DIR)GSBString.o -c GSBString.cpp

$(TARGET): $(OBJS_BASIC) $(OBJS_VECTREX32)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$(TARGET_BASE).elf.img $(OBJS_BASIC) $(OBJS_VECTREX32) $(LIBSGCC)	
	$(RM) $(TARGET)
	arm-none-eabi-objcopy $(BUILD_DIR)$(TARGET_BASE).elf.img -O binary $(TARGET)
	arm-none-eabi-objdump -l -drwCS -D $(BUILD_DIR)$(APP_NAME).elf.img > $(BUILD_DIR)$(APP_NAME).elf.img.s
ifdef MAKE_IMAGE
	cp $(TARGET) $(COPY_TARGET1)
	cp $(TARGET) $(COPY_TARGET2)
endif
