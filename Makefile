CC := gcc
CLFLAGS := -Wall
CLHDRS := $(wildcard src/client/*.h)
CLSRCS := $(wildcard src/client/*.c)
CLOBJS := $(CLSRCS:%.c=tmp/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
CLEXEC := ttt_client

SVFLAGS := -Wall
SVHDRS := $(wildcard src/server/*.h)
SVSRCS := $(wildcard src/server/*.c)
SVOBJS := $(SVSRCS:%.c=tmp/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
SVEXEC := ttt_server

ifeq ($(OS),Windows_NT)
CLFLAGS += -std=c17 -I./include/SDL2/include -L./include/SDL2/lib -Llib -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CLFLAGS += -lSDL2 -lSDL2_ttf
endif
ifeq ($(UNAME_S),Darwin)
ifeq ($(filter %86,$(UNAME_P)),)
CLFLAGS += -I./include/SDL2/include -lSDL2 -lSDL2_ttf -lm
else
CLFLAGS += -I./include/SDL2/include -lSDL2 -lSDL2_ttf -lm
endif
endif
endif


.PHONY: all, client

all, client: $(CLEXEC)

$(CLEXEC): $(CLOBJS) $(CLHDRS)
	$(CC) -o $@ $(CLOBJS) $(CLFLAGS) && echo "EXEC [OK]  $@"

tmp/%.o: %.c
	@$(CC) $(CLFLAGS) -c $< -o $@ && echo "tmp/%.o: %.c [OK]  $@"

.PHONY: server

server: $(SVEXEC)

$(SVEXEC): $(SVOBJS) $(SVHDRS)
	$(CC) -o $@ $(SVOBJS) $(SVFLAGS) && echo "EXEC [OK]  $@"

tmp/%.o: %.c
	@$(CC) $(SVFLAGS) -c $< -o $@ && echo "tmp/%.o: %.c [OK]  $@"


.PHONY: clean, clear

clean clear:
	@rm -f ttt_client && echo "[CL] out/ttt_client"
	@rm -f tmp/src/client/* && echo "[CL] tmp/client"
	@rm -f ttt_server && echo "[CL] out/ttt_server"
	@rm -f tmp/src/server/* && echo "[CL] tmp/server"