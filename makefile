#  -  CONFIGURABLE PARAMETERS  -  #
SHELL = /bin/sh 

srcdir = ./src
builddir = ./build
includedir = ./include
depsdir = $(builddir)/deps

AR = ar
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=gnu17 -pedantic
CFLAGS += -fsanitize=address
LDFLAGS = -lm 
CDEBUG = -g -O3
# END OF CONFIGURABLE PARAMETERS  #

ALL_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags sdl2) $(CDEBUG)
ALL_LDFLAGS = $(LDFLAGS) $(shell pkg-config --libs sdl2)

# -  TARGETS  -  #
CLIENT_SRC = main.c keybindings.c sidedef.c sector.c blockmap.c bsp.c byte_reader.c \
	engine.c header.c linedef.c lump.c map_renderer.c node.c player.c segment.c \
	subsector.c thing.c vertex.c wad_data.c
CLIENT_OBJ = $(CLIENT_SRC:%.c=%.o)
CLIENT_LIB = 
CLIENT_LDFLAGS = -lSDL2

SERVER_SRC = server.c
SERVER_OBJ = $(SERVER_SRC:%.c=%.o)
SERVER_LIB = libnet.a libevent.a
SERVER_LDFLAGS = -lSDL2 -lSDL2_net

LIBNET_SRC = net/util.c net/packet/client.c net/packet/server.c
LIBNET_OBJ = $(LIBNET_SRC:%.c=%.o)
LIBNET_LIB =

LIBEVENT_SRC = event/event.c \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/server_*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/client_*.c))
LIBEVENT_OBJ = $(LIBEVENT_SRC:%.c=%.o)
LIBEVENT_LIB =
# - END OF TARGETS  -  #

.PHONY: all clean before_build run_server run_client build_server build_client
all: $(builddir)/server $(builddir)/client

# helper targets
run_server: build_server
	$(builddir)/server 9999
run_client: build_client
	$(builddir)/client
build_client: $(builddir)/client
build_server: $(builddir)/server

# client - build + link target
$(builddir)/server: $(addprefix $(depsdir)/, $(SERVER_OBJ)) $(addprefix $(depsdir)/, $(SERVER_LIB)) | before_build
	@echo "Building server..."
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(ALL_LDFLAGS) $(SERVER_LDFLAGS)

# client - build + link target
$(builddir)/client: $(addprefix $(depsdir)/, $(CLIENT_OBJ)) $(addprefix $(depsdir)/, $(CLIENT_LIB)) | before_build
	@echo "Building client..."
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(ALL_LDFLAGS) $(CLIENT_LDFLAGS)

# libnet - build archive target
$(depsdir)/libnet.a: $(addprefix $(depsdir)/, $(LIBNET_OBJ)) $(addprefix $(depsdir)/, $(LIBNET_LIB)) | before_build
	@echo "Building libnet..."
	$(AR) rcs $@ $^

# libevent - build archive target
$(depsdir)/libevent.a: $(addprefix $(depsdir)/, $(LIBEVENT_OBJ)) $(addprefix $(depsdir)/, $(LIBEVENT_LIB)) | before_build
	@echo "Building libevent..."
	$(AR) rcs $@ $^

# compilation target
$(depsdir)/%.o: $(srcdir)/%.c | before_build
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

# other targets
before_build:
	@mkdir -p $(builddir)
	@mkdir -p $(depsdir)

clean:
	-rm -rf $(builddir)