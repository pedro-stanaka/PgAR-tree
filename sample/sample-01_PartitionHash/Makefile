#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

PROJECT = MainTest
CC = g++

ifeq ($(CFG),Debug)
  OBJ_DIR = .
  OUTPUT_DIR = .
  TARGET = MainTest
  C_INCLUDE_DIRS = -I"../../include" 
  C_PREPROC = -D__stDEBUG__ 
  CFLAGS = -pipe  -w -frtti -fexceptions 
  LIB_DIRS = -L"../../lib" 
  LIBS = -larboretum 
  LDFLAGS = -pipe 
endif

ifeq ($(CFG),Release)
  OBJ_DIR = .
  OUTPUT_DIR = .
  TARGET = MainTest
  C_INCLUDE_DIRS = -I"../../include" 
  C_PREPROC = -D__stDEBUG__
  CFLAGS = -pipe  -Wall -frtti -fexceptions 
  LIB_DIRS = -L"../../lib" 
  LIBS = -larboretum 
  LDFLAGS = -pipe -s 
endif

SRC_OBJS = \
  $(OBJ_DIR)/app.o	\
  $(OBJ_DIR)/mainTest.o	\
  $(OBJ_DIR)/myobject.o

define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)/$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

cleanall:
	@echo Deleting intermediate files for 'MainTest - $(CFG)'
	-@rm -rf *.o
	-@rm -rf "$(TARGET)"

clean:
	@echo Deleting intermediate files for 'MainTest - $(CFG)'
	-@rm -rf *.o

print_header:
	@echo ----------Configuration: MainTest - $(CFG)----------

directories:
	-@if [ ! -d "$(OUTPUT_DIR)" ]; then mkdir "$(OUTPUT_DIR)"; fi
	-@if [ ! -d "$(OBJ_DIR)" ]; then mkdir "$(OBJ_DIR)"; fi

$(OBJ_DIR)/app.o: app.cpp	\
app.h
	$(compile_source)

$(OBJ_DIR)/mainTest.o: mainTest.cpp	\
app.h
	$(compile_source)

$(OBJ_DIR)/myobject.o: myobject.cpp	\
myobject.h
	$(compile_source)



