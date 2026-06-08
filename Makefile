CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = scheduler
SOURCES = main.cpp scheduler.cpp process.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean