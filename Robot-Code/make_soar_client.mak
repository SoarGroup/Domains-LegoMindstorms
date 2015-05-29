PROGRAM=bin/soar_client
SOURCES=src/sources_soar_client.cxx
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

INCLUDEPATH=-Isrc -I$(SOAR_HOME)/include 
LIBRARYPATH=-L$(SOAR_HOME)

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) src/*.o src/comm/*.o src/ev3/*.o src/soar/*.o 

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) -g

%.o: %.cxx
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) -g

