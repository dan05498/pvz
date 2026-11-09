gcc src/*.c -o pvz $(pkg-config --cflags --libs sdl3 sdl3-image sdl3-mixer)
