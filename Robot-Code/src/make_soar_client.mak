PROGRAM=../bin/soar_client
SOURCES=sources_soar_client.cxx
OBJECTS=$(SOURCES:.cxx=.o)

OS := $(shell uname)
ifeq ($(OS),Darwin) 
	TOOLPREFIX=clang
	LIBRARIES=-lSoar -ldl -lpthread
else
	TOOLPREFIX=g
	LIBRARIES=-lSoar -lrt -ldl -lpthread
endif
CC=$(TOOLPREFIX)++

FLAGS=

INCLUDEPATH=-I$(EV3)/soarev3 -I$(EV3)/include -I$(SOAR_HOME)/include 
LIBRARYPATH=-L$(SOAR_HOME)

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) *.o comm/*.o ev3/*.o soar/*.o 

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

