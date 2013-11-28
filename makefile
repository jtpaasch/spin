# Which compiler?
CC = gcc

# Flags for the compiler.
FLAGS = -Wall

# The build directory.
BUILD_DIRECTORY = build

# The files to compile.
FILES = spin.c utilities.c

# The executable to create.
OUTPUT = $(BUILD_DIRECTORY)/spin

# Compile the executable.
build: $(FILES)
	@mkdir -p $(BUILD_DIRECTORY)
	@$(CC) $(FLAGS) -o $(OUTPUT) $(FILES)

# Clean up the files for a fresh start.
clean:
	rm -fr $(BUILD_DIRECTORY)

# Clean and build.
rebuild: clean build

# Install
install:
	@echo "Installing..."
	@echo "-- install spin /usr/local/bin"
	@sudo install $(OUTPUT) /usr/local/bin
	@if [ ! -d $(HOME)/.spin ]; then echo "-- copy config to $(HOME)/.spin"; mkdir $(HOME)/.spin; fi;
	@sudo cp -r config $(HOME)/.spin/
	@echo "-- done"