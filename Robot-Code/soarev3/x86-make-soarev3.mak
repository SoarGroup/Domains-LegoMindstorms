PROGRAM=../lib/x86/libsoarev3.so
SOURCES=soarev3.cxx
OBJECTS=$(SOURCES:.cxx=.o)

LIBRARIES=-llcmlite -lSoar -ldl -lpthread

OS := $(shell uname)
ifeq ($(OS),Darwin) 
	TOOLPREFIX=clang
else
	TOOLPREFIX=g
	LIBRARIES := -lrt
endif
CC=$(TOOLPREFIX)++

FLAGS=-shared -fpic

INCLUDEPATH=-I$(EV3)/soarev3 -I$(EV3)/lcmlite -I$(EV3)/include -I$(SOAR_HOME)/include 
LIBRARYPATH=-L$(SOAR_HOME) -L$(EV3)/lib/x86

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) *.o comm/*.o ev3/*.o soar/*.o 

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

