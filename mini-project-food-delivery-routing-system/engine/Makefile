CXX = g++
CXXFLAGS = -O2 -std=c++17
TARGET = core.exe
SRC = core.cpp main.cpp
HDR = core.h

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
