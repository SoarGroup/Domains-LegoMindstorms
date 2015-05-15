PROGRAM=../bin/soar_ev3
DOBJECTS=soar_ev3.cpp

INCLUDEPATH=-I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/soarev3 -I$(SOAR_HOME)/include
LIBRARYPATH=-L$(EV3)/lib/x86 -L$(SOAR_HOME)
LIBRARIES=-lsoarev3 -llcmlite -lSoar -ldl -lpthread

OS := $(shell uname)
ifeq ($(OS),Darwin) 
	TOOLPREFIX=clang
else
	TOOLPREFIX=g
endif
CC=$(TOOLPREFIX)++

FLAGS=

all: $(DOBJECTS) $(PROGRAM)

clean:
	rm -f *.o *.ppu *.rst $(PROGRAM)

# how to link executable
$(PROGRAM): $(DOBJECTS)
	$(CC) $(FLAGS) -o$@ $(DOBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) 

