#  -  CONFIGURABLE PARAMETERS  -  #
SHELL = /bin/sh 

srcdir = ./src
testdir = ./test
builddir = ./build
includedir = ./include
depsdir = $(builddir)/deps
testdepsdir = $(builddir)/test_deps

AR = ar
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu17 -pedantic
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS = -lm 
CDEBUG = -g -O3
# END OF CONFIGURABLE PARAMETERS  #

ALL_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags sdl2) $(CDEBUG)
ALL_LDFLAGS = $(LDFLAGS) $(shell pkg-config --libs sdl2)

# -  TARGETS  -  #
CLIENT_SRC = sound.c blockmap.c bsp.c byte_reader.c color.c engine.c geometry.c header.c \
	keybindings.c linedef.c lump.c main.c map_renderer.c node.c player.c sector.c segment.c \
	segment_handler.c sidedef.c subsector.c thing.c timer.c util.c vertex.c wad_data.c weapons.c hitscan.c\
	audio/mixer.c audio/emitter.c ui/def.c ui/module.c ui/common.c ui/label.c ui/button.c ui/image.c ui/textbox.c ui/event_handler.c ui/feed.c \
	game_states.c events.c flat.c texture.c patch.c
CLIENT_OBJ = $(CLIENT_SRC:%.c=%.o)
CLIENT_LIB = 
CLIENT_LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

SERVER_SRC = server.c
SERVER_OBJ = $(SERVER_SRC:%.c=%.o)
SERVER_LIB = libnet.a libevent.a
SERVER_LDFLAGS = -lSDL2 -lSDL2_net

LIBNET_SRC = net/util.c net/tracked_connection.c net/packet/client.c net/packet/server.c
LIBNET_OBJ = $(LIBNET_SRC:%.c=%.o)
LIBNET_LIB =

LIBEVENT_SRC = event/event.c \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/server_*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/client_*.c))
LIBEVENT_OBJ = $(LIBEVENT_SRC:%.c=%.o)
LIBEVENT_LIB =

LIBCOLLECTION_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/collection/*.c))
LIBCOLLECTION_OBJ = $(LIBCOLLECTION_SRC:%.c=%.o)
LIBCOLLECTION_TEST_SRC = $(patsubst $(testdir)/%, %, $(wildcard $(testdir)/collection/*.c))
LIBCOLLECTION_TEST_OBJ = $(LIBCOLLECTION_TEST_SRC:%.c=%.o)
LIBCOLLECTION_LIB =
LIBCOLLECTION_LDFLAGS = 

LIBECS_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/ecs/*.c))
LIBECS_OBJ = $(LIBECS_SRC:%.c=%.o)
LIBECS_TEST_SRC = $(patsubst $(testdir)/%, %, $(wildcard $(testdir)/ecs/*.c))
LIBECS_TEST_OBJ = $(LIBECS_TEST_SRC:%.c=%.o)
LIBECS_LIB = libcollection.a
# - END OF TARGETS  -  #

.PHONY: all clean test before_build run_server run_client build_server build_client test test_collection \
	test_ecs
all: $(builddir)/server $(builddir)/client

# helper targets
run_server: build_server
	$(builddir)/server 9999
run_client: build_client
	$(builddir)/client
test: test_collection test_ecs
test_collection: $(builddir)/test_collection
	@$(builddir)/test_collection
test_ecs: $(builddir)/test_ecs
	@$(builddir)/test_ecs
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

# libcollection - build archive target
$(depsdir)/libcollection.a: $(addprefix $(depsdir)/, $(LIBCOLLECTION_OBJ)) $(addprefix $(depsdir)/, $(LIBCOLLECTION_LIB)) | before_build
	@echo "Building libcollection..."
	$(AR) rcs $@ $^

# libcollection - test target
$(builddir)/test_collection: $(addprefix $(testdepsdir)/, $(LIBCOLLECTION_TEST_OBJ)) $(addprefix $(depsdir)/, $(LIBCOLLECTION_LIB)) $(depsdir)/libcollection.a | before_build
	@echo "Building test_collection..."
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(ALL_LDFLAGS) $(LIBCOLLECTION_LDFLAGS)

# libecs - build archive target
$(depsdir)/libecs.a: $(addprefix $(depsdir)/, $(LIBECS_OBJ)) | before_build
	@echo "Building libecs..."
	$(AR) rcs $@ $^

# libecs - test target
$(builddir)/test_ecs: $(addprefix $(testdepsdir)/, $(LIBECS_TEST_OBJ)) $(depsdir)/libecs.a $(addprefix $(depsdir)/, $(LIBECS_LIB)) | before_build
	@echo "Building test_ecs..."
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(ALL_LDFLAGS)

# compilation target
$(depsdir)/%.o: $(srcdir)/%.c | before_build
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

# compilation target for tests
$(testdepsdir)/%.o: $(testdir)/%.c | before_build
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

# other targets
before_build:
	@mkdir -p $(builddir)
	@mkdir -p $(depsdir)

clean:
	-rm -rf $(builddir)

