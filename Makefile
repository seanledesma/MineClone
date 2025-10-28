# --- Compiler and OS Detection ---
OS_NAME := $(shell uname -s)
EMCC = emcc

# --- Common Configuration ---
# Source files. Using wildcard is fine, but being explicit is safer.
CFILES = src/main.c src/blockmanager.c src/chunkmanager.c src/myhash.c

# Include paths for headers (e.g., raylib.h, include.h)
# Assumes 'include.h' is in 'src/' or 'lib/'. Adjust if it's in the root.
INCLUDES = -Ilib/ -Isrc/

# Output binary name for native build
NATIVE_OUTPUT = bin/build

# --- NEW: Aggressive Warning and Sanitizer Flags for Debugging ---
# This includes:
# -Wall -Wextra: The standard set of common and extra warnings.
# -Werror: Treats all warnings as errors, forcing clean code.
# -Wpedantic: Strictly adheres to the standard.
# -fsanitize=address: **Address Sanitizer (ASan)** - Catches use-after-free, buffer overflows, memory leaks, etc.
# -fsanitize=undefined: **Undefined Behavior Sanitizer (UBSan)** - Catches things like signed integer overflow, unaligned pointer cast, etc.
# -g: Generates debug information for use with tools like gdb or lldb.
C_DEBUG_FLAGS = -Wall -Wextra -Werror -Wpedantic -g -fsanitize=address,undefined

# --- Native Build Configuration ---

# macOS-specific
ifeq ($(OS_NAME),Darwin)
    COMPILER = clang
    # Apply the new debug flags here
    NATIVE_CFLAGS = $(C_DEBUG_FLAGS)
    NATIVE_LIBS = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
endif

# Linux-specific
ifeq ($(OS_NAME),Linux)
    COMPILER = gcc
    # Note: GCC supports most of these flags, but ASan may be slightly different.
    # We will use GCC's version of the sanitizers.
    NATIVE_CFLAGS = $(C_DEBUG_FLAGS)
    # Make sure you have libraylib.so or libraylib.a in your system path or /usr/lib
    NATIVE_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# --- Web (WASM) Build Configuration ---

# Output JS file. Emscripten will also create game.wasm and game.data
# We want game.js so our custom index.html can load it.
WEB_OUTPUT = web/game.js

# Web-specific libraries and paths
WEB_LIB_PATH = -Lweb/
WEB_LIB = web/libraylib.a

# Web compilation flags
# -Os: Optimize for size
# -DPLATFORM_WEB: Raylib flag for web platform
# --preload-file assets: Bundles the 'assets' directory into 'game.data'
WEB_FLAGS = -Os -Wall -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --preload-file assets

# --- Build Targets ---

# Default target: build native
all: build

# Build native executable
build:
	@mkdir -p bin
	$(COMPILER) $(CFILES) $(INCLUDES) $(NATIVE_CFLAGS) -o $(NATIVE_OUTPUT) $(NATIVE_LIBS)

# Build WebAssembly files (game.js, game.wasm, game.data)
# Note: Sanitizers are not used for the web build as they significantly increase file size and complexity.
# Emscripten has its own set of debug tools if needed.
build_web:
	@mkdir -p web
	$(EMCC) -o $(WEB_OUTPUT) $(CFILES) $(WEB_FLAGS) $(INCLUDES) $(WEB_LIB_PATH) $(WEB_LIB)

# --- Run Targets ---

# Run the native build
run: build
	./$(NATIVE_OUTPUT)

# Provides instructions for running the web build
run_web:
	@echo "--- To run the web build, you MUST use a local server ---"
	@echo "1. Make sure you have your 'index.html' file in the 'web' directory."
	@echo "2. Navigate to the 'web' directory:"
	@echo "   cd web"
	@echo "3. Run a simple HTTP server (e.g., 'python3 -m http.server' or 'npx http-server')."
	@echo "4. Open your browser to the server address (e.g., http://localhost:8000/index.html)."
	@echo "NOTE: This build uses Emscripten and needs to load files, so it can't run directly from file://."

# Clean up build files
clean:
	rm -rf bin web
