#+++
# 
# Copyright (c) 2021 JulesIMF, MIPT
#
# Module name:
#
#		makefile
#
# Abstract:
#
#		My default makefile. Allows to compile the project
#		changing only FILES (and other flags if needed).
#		
#		Build this project with "make" or "make mol".
#		
#
#---

#
# Defines
#

ifeq ($(ASAN), 1)
ASAN =-fsanitize=address
else
ASAN = 
endif

CXX	    = g++ -std=c++17
INC	    = -Iincludes -IJG/includes
SFML    = -lsfml-graphics -lsfml-window -lsfml-system
OPT	    = -O2
DEF		= -DNDEBUG
CFLAGS	= $(OPT) $(DEF) $(ASAN) -g $(INC)
OBJ	    = obj/

FILES   = $(OBJ)main.o \
		  $(OBJ)common/messages.o \
		  $(OBJ)common/colors.o \
		  $(OBJ)editor/Editor.o \
		  $(OBJ)editor/Palette.o \
		  $(OBJ)editor/Tool.o \
		  $(OBJ)editor/ToolPicker.o \
		  $(OBJ)editor/LayerSwitcher.o \
		  $(OBJ)editor/Layer.o \
		  $(OBJ)file/BMP.o

#
# Rules
#

jimp: $(FILES) JG/libJG.a
	$(CXX) $(CFLAGS) $(FILES) JG/libJG.a -pthread $(SFML) -lX11 -o $@
#	(make clean;)

JG/libJG.a: 
	(cd JG; OPT=$(OPT) ASAN=$(ASAN) make $@)

obj/main.o: main.cpp UI/UI.h
	$(CXX) $(CFLAGS) main.cpp -c
	mkdir -p $(@D)
	mv $(@F) $@

obj/%.o: %.cpp
	$(CXX) $(CFLAGS) $^ -c
	mkdir -p $(@D)
	mv $(@F) $@

clean-there:
	rm $(FILES)

clean:
	(make clean-there)
	(cd JG; make clean)

%: %.cpp
	(make obj/$@.o;)