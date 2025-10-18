OS_NAME := $(shell uname -s)

# Common
CFILES = src/*.c
SOURCE_LIBS = -Ilib/
OUTPUT = bin/build

# macOS-specific
ifeq ($(OS_NAME),Darwin)
	COMPILER = clang
	OS_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
endif

# Linux-specific
ifeq ($(OS_NAME),Linux)
	COMPILER = gcc
	OS_OPT = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Build Target
build:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) -o $(OUTPUT) $(OS_OPT)

# Run Target
run: build
	./$(OUTPUT)

# Clean Target
clean:
	rm -f $(OUTPUT)
