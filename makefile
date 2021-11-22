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

UI		= UI/UI.h UI/VistaButton.h UI/VistaPanel.h UI/VistaSlider.h UI/VistaTextBox.h UI/Spline.h UI/VistaMenu.h includes/editor/Curves.h
GREEN   = @echo "\033[0;32m";
DFLT    = @echo "\033[0;m";


#
# Rules
#

jimp: $(FILES) JG/libJG.a
	$(GREEN)
	@echo 'Linking $@ (changed $?)';
	$(DFLT)
	@$(CXX) $(CFLAGS) $(FILES) JG/libJG.a -pthread $(SFML) -lX11 -o $@
#	(make clean;)

JG/libJG.a: 
	@(cd JG; OPT=$(OPT) ASAN=$(ASAN) make $@)

lib:
	@(cd JG; OPT=$(OPT) ASAN=$(ASAN) make JG/libJG.a)

obj/main.o: main.cpp $(UI)
	$(GREEN)
	@echo 'Building $@ (changed $?)';
	$(DFLT)
	@$(CXX) $(CFLAGS) main.cpp -c
	@mkdir -p $(@D)
	@mv $(@F) $@

obj/%.o: %.cpp
	$(GREEN)
	@echo 'Building $@ (changed $?)';
	$(DFLT)
	@$(CXX) $(CFLAGS) $^ -c
	@mkdir -p $(@D)
	@mv $(@F) $@

clean-there:
	$(GREEN)
	@echo 'Cleaning JIMP project';
	$(DFLT)
	@rm $(FILES)

clean:
	@(make clean-there)
	@(cd JG; make clean)

%: %.cpp
	@(make obj/$@.o;)
