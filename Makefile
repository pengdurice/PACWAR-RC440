#
# This is a makefile for a version of wish (myWish) which will
# run the comp440 PacWar Simulator.  You can run it directly via
#
#                  myWish -f PacWar.tcl
#
# or you can run myWish without any switches; at the '%' propmpt you
# can enter
#
#                  source PacWar.tcl
#
# to get things started.
#

#
# Note that the object file PacWarGuts.o can be used to test a species
# and/or run a duel without using the graphics.  This makes it ideal
# for integrating into a program that searches the space of possible
# genes.
#

CC = gcc
CFLAGS = -g -Wall 

.c.o:
	$(CC) -c $(CC_SWITCHES) $<

EXE = myWish
OBJS = tkAppInit.o PacWarGuts.o PacWarTk.o

PREFIX = /opt/local/share
TK_DIR = $(PREFIX)/tk-8.0
TCL_DIR = $(PREFIX)/tcl-8.0

X11_INCLUDE = -I/opt/X11/include 
X11_LIBS =    -L/opt/X11/lib -lX11

AC_FLAGS = -DHAVE_UNISTD_H=1 -DNO_STDLIB_H=1

TCLTK_INCLUDE = -I$(TCL_DIR)/include -I$(TK_DIR)/include
TCLTK_LIBS =    -L/usr/lib -L/usr/lib -ltk -ltcl

LIBS    = $(X11_LIBS)  -lm -ldl $(TCLTK_LIBS)
INCLUDE = $(X11_INCLUDE) $(TCLTK_INCLUDE)

CC_SWITCHES = ${CFLAGS} ${INCLUDE} ${AC_FLAGS} \
	      -DTK_LIBRARY=\"/usr/lib\"

# These are other files that aren't needed to compile but ARE needed to execute
SIM_AUX = PacWar.tcl

all: 	$(EXE)
$(EXE):	$(OBJS)
	${CC} ${CC_SWITCHES} ${OBJS} ${LIBS} -o $(EXE)

clean:  
	rm -rf $(EXE) $(OBJS)

check:	myWish ${SIM_AUX}






