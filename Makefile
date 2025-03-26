CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

TARGET = dining_philosophers

# Nasze pliki źródłowe
SOURCES = main.cpp waiter.cpp philosopher.cpp
# Można dodać states.cpp, ale enum w states.hpp nie wymaga osobnego .cpp

OBJ = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
