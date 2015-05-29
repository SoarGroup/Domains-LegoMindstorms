PROGRAM=bin/ev3_server
SOURCES=src/sources_ev3_server.cxx
OBJECTS=$(SOURCES:.cxx=.o)

TOOLPREFIX=arm-linux-gnueabi-
CC=$(TOOLPREFIX)g++

FLAGS=

LIBRARIES=-lpthread

INCLUDEPATH=-I$(EV3)/src -I$(SOAR_HOME)/include

LIBRARYPATH=

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) src/*.o src/comm/*.o src/ev3/*.o src/soar/*.o

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) 

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)


