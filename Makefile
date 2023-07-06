CFLAGS += -Wall -Wextra
#CFLAGS += -fsanitize=address -ggdb
#LDFLAGS +=
LDLIBS += -lncurses -lm
ALL_H = $(wildcard *.h)
ROOT_C = $(wildcard *.c)
WORLDGEN_C = $(wildcard worldgen/*.c)
RENDERING_C = $(wildcard rendering/*.c)
INTERACT_C = $(wildcard interact/*.c)
ALL_C = $(WORLDGEN_C) $(RENDERING_C) $(INTERACT_C) $(ROOT_C)
ALL_O = $(ALL_C:.c=.o)

all: voxel_cli

$(ALL_O): $(ALL_H)

voxel_cli: $(ALL_O)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(ALL_O)
	rm -f voxel_cli

