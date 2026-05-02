CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I../src
EIGEN_INCLUDE = -IC:\Users\bselv\Cpp_Libraries\eigen-5.0.0

SRC_DIR   = ../src
BUILD_DIR = build

# All src .cpp files except main.cpp
SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SRC_OBJS  = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/src_%.o, $(SRC_FILES))

TESTS     = test_poisson
TEST_EXES = $(addprefix $(BUILD_DIR)/, $(addsuffix .exe, $(TESTS)))

.PHONY: all run clean

all: $(TEST_EXES)

# Link each test executable
$(BUILD_DIR)/test_poisson.exe: $(BUILD_DIR)/test_poisson.o $(SRC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile test sources
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(EIGEN_INCLUDE) -c $< -o $@

# Compile library sources
$(BUILD_DIR)/src_%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(EIGEN_INCLUDE) -c $< -o $@

$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

run: all
	@for %%e in ($(TEST_EXES)) do ( \
		echo Running %%e ... & \
		%%e & \
		if errorlevel 1 ( echo FAILED: %%e ) \
	)

clean:
	if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
