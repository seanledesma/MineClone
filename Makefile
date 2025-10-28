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

# --- Native Build Configuration ---

# macOS-specific
ifeq ($(OS_NAME),Darwin)
    COMPILER = clang
    NATIVE_LIBS = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
endif

# Linux-specific
ifeq ($(OS_NAME),Linux)
    COMPILER = gcc
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
	$(COMPILER) $(CFILES) $(INCLUDES) -o $(NATIVE_OUTPUT) $(NATIVE_LIBS)

# Build WebAssembly files (game.js, game.wasm, game.data)
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
	@echo "3. Run a simple web server. If you have Python 3:"
	@echo "   python3 -m http.server"
	@echo "4. Open your browser and go to:"
	@echo "   http://localhost:8000/index.html"

# --- Clean Target ---
clean:
	@echo "Cleaning build files..."
	rm -f $(NATIVE_OUTPUT)
	rm -f web/game.js web/game.wasm web/game.data
	# Also remove emscripten-generated html, just in case
	rm -f web/game.html 
