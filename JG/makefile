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

ifneq ($(ASAN), )
ASAN = -fsanitize=address
endif

CXX	    =  g++ -std=c++17
AR		=  ar
INC	    = -Iincludes
OBJ	    =  obj/
WARN	= -Wall -Wextra -Werror
SFML    = -lsfml-graphics -lsfml-window -lsfml-system
OPT	    = -O2
DEF		= -DNDEBUG
CFLAGS	= $(OPT) $(DEF) $(ASAN) $(WARN) -g -pthread $(INC)

JGFILES   = $(OBJ)Events.o \
		    $(OBJ)Shape.o \
		    $(OBJ)Window.o \
		    $(OBJ)Widget.o \
		    $(OBJ)Button.o \
		    $(OBJ)Canvas.o \
			$(OBJ)Panel.o

GREEN   = @echo "\033[0;32m";
DFLT    = @echo "\033[0;m";		

#
# Rules
#

JG/libJG.a: $(JGFILES)
	$(GREEN)
	@echo 'Compressing $@ (changed $?)';
	$(DFLT)	
	@$(AR) rcs libJG.a $(JGFILES)

obj/%.o: %.cpp
	$(GREEN)
	@echo 'Building $@ (changed $?)';
	$(DFLT)	
	@$(CXX) $(CFLAGS) $^ -c -lX11
	@mkdir -p $(@D)
	@mv $(@F) $@

clean:
	$(GREEN)
	@echo 'Cleaning JG project';
	$(DFLT)
	@rm $(JGFILES) libJG.a

%: %.cpp
	@(make obj/$@.o;)
