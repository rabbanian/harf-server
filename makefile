CC=g++
CFLAGS=--std=c++17 -c -Wall
LDFLAGS=-O3 -lpthread
SOURCES=main.cpp list.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=harf

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
