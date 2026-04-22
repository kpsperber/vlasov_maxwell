CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Isrc
EIGEN_INCLUDE = -I"C:\Users\kpspe\C_Libraries\eigen"
# EIGEN_INCLUDE =  -IC:\Users\bselv\Cpp_Libraries\eigen-5.0.0

TARGET = solver
SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
EXE = $(BUILD_DIR)/$(TARGET).exe

.PHONY: all build run clean

all: build run clean

build: $(EXE)

$(EXE): $(OBJECTS)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(EIGEN_INCLUDE) -c $< -o $@

run:
	@$(EXE) $(ARGS)

clean:
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)