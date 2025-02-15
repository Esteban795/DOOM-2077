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
CFLAGS = -Wall -Wextra -Wvla -std=gnu17 -pedantic
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS = -lm 
CDEBUG = -g -O3 -D 'SERVER_ADDR="0.0.0.0"' -D SERVER_PORT=9999
# END OF CONFIGURABLE PARAMETERS  #

ALL_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags sdl2 jansson) $(CDEBUG)
ALL_LDFLAGS = $(LDFLAGS) $(shell pkg-config --libs sdl2 jansson)

# -  TARGETS  -  #
CLIENT_SRC = audio/mixer.c audio/emitter.c WAD/blockmap.c core/bsp.c util/byte_reader.c WAD/color.c \
    core/engine.c core/events.c WAD/flat.c core/game_states.c \
	util/geometry.c WAD/header.c core/keybindings.c WAD/linedef.c WAD/lump.c main.c render/map_renderer.c WAD/node.c \
	WAD/patch.c core/player.c remote.c WAD/sector.c WAD/segment.c render/segment_handler.c WAD/sidedef.c WAD/sound.c WAD/subsector.c \
	WAD/texture.c WAD/thing.c util/timer.c util/util.c WAD/vertex.c WAD/wad_data.c core/weapons.c core/lift.c core/door.c render/drawseg.c render/vssprite.c \
	render/player_animation.c core/spawnpoints.c core/controller.c \
	ui/def.c ui/module.c ui/common.c ui/label.c ui/button.c ui/image.c ui/textbox.c ui/event_handler.c ui/feed.c shared.c \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/component/*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/system/client/*.c))

CLIENT_OBJ = $(CLIENT_SRC:%.c=%.o)
CLIENT_LIB = libnet.a libevent.a libecs.a libcollection.a
CLIENT_LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_net

SERVER_SRC = server.c \
	WAD/blockmap.c util/byte_reader.c WAD/color.c core/door.c WAD/flat.c util/geometry.c WAD/header.c core/lift.c \
	WAD/linedef.c WAD/lump.c WAD/node.c WAD/patch.c WAD/sector.c WAD/segment.c WAD/sidedef.c WAD/sound.c \
	WAD/subsector.c WAD/texture.c WAD/thing.c util/util.c WAD/vertex.c WAD/wad_data.c render/drawseg.c render/vssprite.c core/spawnpoints.c \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/server/*.c)) \
 	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/event/*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/system/server/*.c)) \
	$(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/component/*.c)) 
	
SERVER_OBJ = $(SERVER_SRC:%.c=%.o)
SERVER_LIB = libnet.a libevent.a libecs.a libcollection.a
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

LIBAUDIO_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/audio/*.c))
LIBAUDIO_OBJ = $(LIBAUDIO_SRC:%.c=%.o)

LIBWAD_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/WAD/*.c))
LIBWAD_OBJ = $(LIBWAD_SRC:%.c=%.o)

LIBCORE_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/core/*.c))
LIBCORE_OBJ = $(LIBCORE_SRC:%.c=%.o)

LIBRENDER_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/render/*.c))
LIBRENDER_OBJ = $(LIBRENDER_SRC:%.c=%.o)

LIBUI_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/ui/*.c))
LIBUI_OBJ = $(LIBUI_SRC:%.c=%.o)

LIBUTIL_SRC = $(patsubst $(srcdir)/%, %, $(wildcard $(srcdir)/util/*.c))
LIBUTIL_OBJ = $(LIBUTIL_SRC:%.c=%.o)
# - END OF TARGETS  -  #

.PHONY: all clean test before_build run_server run_client build_server build_client test test_collection \
	test_ecs doc open_doc
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

# server - build + link target
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

doc:
	doxygen

open_doc: doc
	xdg-open ./build/doc/html/index.html