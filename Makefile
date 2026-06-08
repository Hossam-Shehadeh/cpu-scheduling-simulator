CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET   = scheduler
SRCDIR   = src
SOURCES  = $(SRCDIR)/main.cpp $(SRCDIR)/scheduler.cpp $(SRCDIR)/process.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) examples/basic.txt 4

.PHONY: all clean run