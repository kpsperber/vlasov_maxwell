CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I../src
EIGEN_INCLUDE = -IC:\Users\bselv\Cpp_Libraries\eigen-5.0.0

SRC_DIR     = ../src
BUILD_DIR   = build
BUILD_STAMP = $(BUILD_DIR)/.stamp

# All src .cpp files except main.cpp
SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SRC_OBJS  = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/src_%.o, $(SRC_FILES))

TESTS       = test_poisson
TEST_EXES   = $(addprefix $(BUILD_DIR)/, $(addsuffix .exe, $(TESTS)))
RUN_EXES    = $(subst /,\,$(TEST_EXES))
SILENT_DIRS = $(MAKE) --no-print-directory

.PHONY: all build run clean

# Build everything, run all tests, then remove build/
all:
	@echo building...
	@$(SILENT_DIRS) build
	@echo running tests...
	@$(SILENT_DIRS) run
	@echo cleaning...
	@$(SILENT_DIRS) clean

build: $(TEST_EXES)

# Link each test executable
$(BUILD_DIR)/test_poisson.exe: $(BUILD_DIR)/test_poisson.o $(SRC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile test sources
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $(EIGEN_INCLUDE) -c $< -o $@

# Compile library sources
$(BUILD_DIR)/src_%.o: $(SRC_DIR)/%.cpp | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $(EIGEN_INCLUDE) -c $< -o $@

# Sentinel only (phony target "build" must not share the name "build" with this prereq).
$(BUILD_STAMP):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@cmd /c type nul > "$(BUILD_STAMP)"

run: build
	@for %%e in ($(RUN_EXES)) do ( \
		".\%%e" & \
		if errorlevel 1 ( echo FAILED: %%e ) \
	)

clean:
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
