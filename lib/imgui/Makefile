
# ╭╮╱╱╱╱╱╱╭╮
# ┣╋━━┳━┳┳╋┫
# ┃┃┃┃┃╋┃┃┃┃
# ╰┻┻┻╋╮┣━┻╯
# ╱╱╱╱╰━╯

###################################
# Hyper Adrenaline ImGui Makefile #
###################################
#---------------------------------#
# USAGE 						  #
#---------------------------------#

	### QUICK GUIDE
	##  First time: 		make libs && make (to compile libraries in your computer then our program)
	##  Subsequent times: 	make			  (to compile our program using already compiled libraries)

	### OPTIONS
	## make				- Compiles the project (not the libraries)
	## make clean		- Removes compilation junk files
	## make cleanall	- Removes compilation junk files AND the main executable compiled
	## make libs		- Compiles the static libraries, thus generating the .a files
	## make libsclean	- Cleans library compilation junk
	## make libscleanall- Cleans library compilation junk AND library compilation results

#---------------------------#
# MACROS 					#
#---------------------------#
### COMPILE
### Compiles a given source file to an object file
# $(1) : Compiler
# $(2) : Object output file
# $(3) : Source input  file
# $(4) : Additional dependencies
# $(5) : Compiler flags
define COMPILE
$(2): $(3) $(4)
	$(1) $(3) -c -o $(2) $(5)
endef

### C20
### Gets a source file name and gets its object file name (.cpp/.c to .o, and src/ to obj/)
# $(1) : Source file input
define C20
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

### C2H
### Gets a source file name and gets its header file name (.cpp to .hpp, .c to .h)
# $(1) : Source file input
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

#---------------------------#
# VARIABLES 				#
#---------------------------#
## Application Name
APP			:= libimgui.a

## Compiler data
CC			:= g++
C			:= gcc
CCFLAGS		:= -Wall -pedantic -std=c++11 -O3
CFLAGS		:= -Wall -pedantic
AR 			:= ar
ARFLAGS 	:= -crs
RANLIB		:= ranlib
## Base directories
SRC			:= imgui
LIB			:= lib
OBJ			:= build
## Found files
ALLCPPS		:= $(shell find $(SRC)/ -type f -iname *.cpp)
ALLCS		:= $(shell find $(SRC)/ -type f -iname *.c)
ALLOBJS		:= $(foreach F,$(ALLCPPS) $(ALLCS),$(call C20,$(F)))
## Found directories
SUBDIRS		:= $(shell find $(SRC) -type d)
OBJSUBDIRS	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

## Libraries and include paths
OPENLG_LINK	:= -lglut -lGLU -lGL -lGLEW -lglfw -lX11
LINK_FLAGS	:= $(OPENLG_LINK)
LIBS		:= $(LINK_FLAGS)
SHARED		:= 
INCDIRS		:= -I$(LIB) -I$(SRC) -I/usr/include/GL/ -I/usr/include/GLFW/ -I/usr/local/include -L/usr/local/lib 

## Tools
MKDIR		:= mkdir -p

#---------------------------------#
# DEPENDECIES AND GENERATED RULES #
#---------------------------------#
.PHONY: info

$(APP) : $(OBJSUBDIRS) $(ALLOBJS)
	$(AR) $(ARFLAGS) $(APP) $(ALLOBJS)
	$(RANLIB) $(APP)

# Compile all .cpp and .c files into .o files
# Don't depend on being a cpp for all h, that's why no call C2H
$(foreach F, $(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C20,$(F)),$(F),,$(CCFLAGS) $(INCDIRS))))
$(foreach F, $(ALLCS),$(eval $(call COMPILE,$(C),$(call C20,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS) $(INCDIRS))))

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLOBJS))

$(OBJSUBDIRS):
	$(MKDIR) $(OBJSUBDIRS)

clean:
	$(RM) -r "./$(OBJ)"
cleanall: clean
	$(RM) -r "./$(APP)"
