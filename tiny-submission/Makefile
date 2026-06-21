CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = main

SRC = main.cpp \
      src/Vector.cpp \
      src/Matrix.cpp \
      src/LinearSystem.cpp \
      src/Regression.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)