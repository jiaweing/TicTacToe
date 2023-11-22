CC := gcc
CLFLAGS += -Wall
CLHDRS := $(wildcard src/client/*.h)
CLSRCS := $(wildcard src/client/*.c)
CLOBJS := $(CLSRCS:src/client/%.c=tmp/client/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
CLEXEC := ttt_client

SVFLAGS := -Wall
SVHDRS := $(wildcard src/server/*.h)
SVSRCS := $(wildcard src/server/*.c)
SVOBJS := $(SVSRCS:src/server/%.c=tmp/server/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
SVEXEC := ttt_server

ifeq ($(OS),Windows_NT)
CLFLAGS += -I./include/SDL2/include
LDFLAGS := -std=c17 -L./include/SDL2/lib -Wl,-subsystem,windows -lmingw32 -lws2_32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -lm
MKDIR_P := mkdir
RM := del /f
RMDIR := rmdir /s /q
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
MKDIR_P := mkdir -p
RM := rm -f
RMDIR := rm -rf
endif


.PHONY: client

client: $(CLEXEC)

$(CLEXEC): $(CLOBJS) $(CLHDRS)
	@echo "$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS)"
	$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS) && echo "EXEC [OK]  $@"

tmp/client/%.o: src/client/%.c
	-@$(MKDIR_P) "$(@D)"
	@echo "$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS)"
	@$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS) && echo "tmp/client/%.o: src/client/%.c [OK]  $@"
	
	
.PHONY: server

server: $(SVEXEC)

$(SVEXEC): $(SVOBJS) $(SVHDRS)
	@echo "$(CC) -o $@ $(SVOBJS) $(SVFLAGS) $(LDFLAGS)"
	$(CC) -o $@ $(SVOBJS) $(SVFLAGS) $(LDFLAGS) && echo "EXEC [OK]  $@"

tmp/server/%.o: src/server/%.c
	-@$(MKDIR_P) "$(@D)"
	@echo "$(CC) $(SVFLAGS) -c $< -o $@ $(LDFLAGS)"
	@$(CC) $(SVFLAGS) -c $< -o $@ $(LDFLAGS) && echo "tmp/server/%.o: src/server/%.c [OK]  $@"
	
	
.PHONY: clean, clear

clean clear:
ifeq ($(OS),Windows_NT)
	-@$(RM) tmp\src\client\* && echo "[CL] tmp\client"
	-@$(RM) tmp\src\server\* && echo "[CL] tmp\server"
	-@$(RM) ttt_client.exe && echo "[CL] ttt_client.exe"
	-@$(RM) ttt_server.exe && echo "[CL] ttt_server.exe"
	-@$(RMDIR) tmp && echo "[CL] tmp"
else
	-@$(RM) tmp/src/client/* && echo "[CL] tmp/client"
	-@$(RM) tmp/src/server/* && echo "[CL] tmp/server"
	-@$(RM) ttt_client && echo "[CL] ttt_client"
	-@$(RM) ttt_server && echo "[CL] ttt_server"
	-@$(RMDIR) tmp && echo "[CL] tmp"
endif