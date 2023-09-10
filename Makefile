CXX=g++
CXXFLAGS=-c -Wall -o2
LDFLAGS=-levdev
SOURCES=idletimer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=idletimer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
