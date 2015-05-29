PROGRAM=bin/ev3_standalone
SOURCES=src/sources_ev3_standalone.cxx
OBJECTS=$(SOURCES:.cxx=.o)

TOOLPREFIX=arm-linux-gnueabi-
CC=$(TOOLPREFIX)g++

FLAGS=-Wl,-rpath,/media/card/lib

INCLUDEPATH=-I$(EV3)/src -I$(SOAR_HOME)/include 

LIBRARYPATH=-L$(EV3)/native #-L$(ARM_GCC)/arm-none-linux-gnueabi/libc/lib 
LIBRARIES=-lSoar -lrt -ldl -lpthread

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) src/*.o src/comm/*.o src/ev3/*.o src/soar/*.o

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o$@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) 

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)


