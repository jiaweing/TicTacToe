CC := gcc
CLFLAGS += -Wall
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
CLFLAGS += -I./include/SDL2/include
LDFLAGS := -std=c17 -L./include/SDL2/lib -llib -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS := -lSDL2 -lSDL2_ttf
endif
ifeq ($(UNAME_S),Darwin)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
CLFLAGS += -I/usr/local/include
LDFLAGS := -L /usr/local/lib -lSDL2 -lSDL2_ttf -lm
else ifeq ($(UNAME_M),arm64)
CLFLAGS += -I/opt/homebrew/include
LDFLAGS := -L /opt/homebrew/lib -lSDL2 -lSDL2_ttf -lm
endif
endif
endif

.PHONY: server

server: $(SVEXEC)

$(SVEXEC): $(SVOBJS) $(SVHDRS)
	@echo "$(CC) -o $@ $(SVOBJS) $(SVFLAGS)"
	$(CC) -o $@ $(SVOBJS) $(SVFLAGS) && echo "EXEC [OK]  $@"

tmp/%.o: %.c
	@mkdir -p $(@D)
	@echo "$(CC) $(SVFLAGS) -c $< -o $@"
	@$(CC) $(SVFLAGS) -c $< -o $@ && echo "tmp/%.o: %.c [OK]  $@"
	
	
.PHONY: all, client

all, client: $(CLEXEC)

$(CLEXEC): $(CLOBJS) $(CLHDRS)
	@echo "$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS)"
	$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS) && echo "EXEC [OK]  $@"

tmp/%.o: %.c
	@mkdir -p $(@D)
	@echo "$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS)"
	@$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS) && echo "tmp/%.o: %.c [OK]  $@"


.PHONY: clean, clear

clean clear:
	@rm -f ttt_client && echo "[CL] out/ttt_client"
	@rm -f tmp/src/client/* && echo "[CL] tmp/client"
	@rm -f ttt_server && echo "[CL] out/ttt_server"
	@rm -f tmp/src/server/* && echo "[CL] tmp/server"
	@rm -rf tmp && echo "[CL] tmp"