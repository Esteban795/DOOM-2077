HEADDIR = include

CC = gcc
FLAGS = -lSDL2 -lSDL2_net -lm $(shell sdl2-config --cflags --libs)
DEBUGFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address -g -O3
DEPENDENCIES = ./src/keybindings.c ./src/sidedef.c ./src/sector.c ./src/blockmap.c ./src/bsp.c ./src/byte_reader.c ./src/engine.c ./src/header.c ./src/linedef.c ./src/lump.c ./src/map_renderer.c ./src/node.c ./src/player.c ./src/segment.c ./src/subsector.c ./src/thing.c ./src/vertex.c ./src/wad_data.c
SERVER_PORT = 9999

build: before_build
	$(CC) ./src/geometry.c ./src/main.c -o ./build/doomlike $(DEBUGFLAGS) $(DEPENDENCIES) $(FLAGS) 

run:
	./build/doomlike

clean:
	rm -rf build

all: build run 

run_server: build_server
	./build/server $(SERVER_PORT)

build_server: before_build build_lib_server ./src/server.c
	$(CC) ./src/server.c -o ./build/server ./build/deps/libnet.a $(DEBUGFLAGS) $(FLAGS)

build_lib_server: before_build build_lib_net
	echo "Compiling server's libs"

build_lib_net: before_build ./src/net/util.c ./src/net/packet/client.c ./src/net/packet/server.c
	echo "Compiling net's libs"
	$(CC) -c ./src/net/util.c -o ./build/deps/net_util.o $(DEBUGFLAGS) $(FLAGS)
	$(CC) -c ./src/net/packet/client.c -o ./build/deps/net_packet_client.o $(DEBUGFLAGS) $(FLAGS)
	$(CC) -c ./src/net/packet/server.c -o ./build/deps/net_packet_server.o $(DEBUGFLAGS) $(FLAGS)
	echo "Linking net's libs"
	ar rcs ./build/deps/libnet.a ./build/deps/net_util.o ./build/deps/net_packet_client.o ./build/deps/net_packet_server.o

before_build:
	mkdir -p build
	mkdir -p build/deps
