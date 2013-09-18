# Which compiler?
CC=gcc

# The files to compile.
FILES = spin.c

# The executable to create.
OUTPUT = spin

# Flags for the compiler.
FLAGS = -Wall

# Compile the executable.
build: $(FILES)
	$(CC) $(FLAGS) -o $(OUTPUT) $(FILES)

# Clean up the files for a fresh start.
clean:
	rm -f $(OUTPUT)

# Clean and build.
rebuild: clean build

# Install
install:
	@echo "Installing..."
	@echo "-- install spin /usr/local/bin"
	@sudo install spin /usr/local/bin
	@if [ ! -d ~/.spin ]; then echo "-- copy config to ~/.spin"; mkdir ~/.spin; fi;
	@sudo cp -r config ~/.spin/
	@echo "-- done"
