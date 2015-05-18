PROGRAM=../bin/ev3_server
SOURCES=sources_ev3_server.cxx
OBJECTS=$(SOURCES:.cxx=.o)

TOOLPREFIX=arm-linux-gnueabi-
CC=$(TOOLPREFIX)g++

FLAGS=

LIBRARIES=-lpthread

INCLUDEPATH=-I$(EV3)/soarev3 -I$(EV3)/include -I$(SOAR_HOME)/include #-I$(ARM_GCC)/arm-none-linux-gnueabi/include 

LIBRARYPATH=

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) *.o comm/*.o ev3/*.o soar/*.o

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) 

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)


