SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CC = gcc
FLAGS = -lSDL2 -lSDL2_net -lm $(shell sdl2-config --cflags --libs)
DEBUGFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address -g -O3
DEPENDENCIES = $(SRCDIR)/keybindings.c $(SRCDIR)/sidedef.c $(SRCDIR)/sector.c $(SRCDIR)/blockmap.c $(SRCDIR)/bsp.c $(SRCDIR)/byte_reader.c $(SRCDIR)/engine.c $(SRCDIR)/header.c $(SRCDIR)/linedef.c $(SRCDIR)/lump.c $(SRCDIR)/map_renderer.c $(SRCDIR)/node.c $(SRCDIR)/player.c $(SRCDIR)/segment.c $(SRCDIR)/subsector.c $(SRCDIR)/thing.c $(SRCDIR)/vertex.c $(SRCDIR)/wad_data.c
SERVER_PORT = 9999

build: before_build
	$(CC) $(SRCDIR)/geometry.c $(SRCDIR)/main.c -o ./build/doomlike $(DEBUGFLAGS) $(DEPENDENCIES) $(FLAGS) 

run:
	./build/doomlike

clean:
	rm -rf build

all: build run 

run_server: build_server
	./build/server $(SERVER_PORT)

build_server: before_build
	$(CC) $(SRCDIR)/server.c -o ./build/server $(DEBUGFLAGS) $(FLAGS)

#build_lib_server: before_build
#TODO

before_build:
	mkdir -p build
	mkdir -p build/deps