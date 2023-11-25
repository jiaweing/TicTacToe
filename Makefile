CC := gcc
CLFLAGS += -Wall
CLHDRS := $(wildcard src/client/*.h)
CLSRCS := $(wildcard src/client/*.c)
CLOBJS := $(CLSRCS:src/client/%.c=.out/client/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
CLEXEC := .out/ttt_client

SVFLAGS := -Wall
SVHDRS := $(wildcard src/server/*.h)
SVSRCS := $(wildcard src/server/*.c)
SVOBJS := $(SVSRCS:src/server/%.c=.out/server/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
SVEXEC := .out/ttt_server

ifeq ($(OS),Windows_NT)
CLFLAGS += -I./include/SDL2/include
LDFLAGS := -std=c17 -L./include/SDL2/lib -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lws2_32 -lm
SVFLAGS += -lws2_32
MKDIR_P := mkdir
RM := del /f
RMDIR := rmdir /s /q
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CLFLAGS += -I./include/SDL2/include
LDFLAGS := -L./include/SDL2/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lpthread
SVFLAGS += -lpthread
endif
ifeq ($(UNAME_S),Darwin)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
CLFLAGS += -I./include/SDL2/include
LDFLAGS := -L./include/SDL2/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lm
else ifeq ($(UNAME_M),arm64)
CLFLAGS += -I/opt/homebrew/include
LDFLAGS := -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lm
endif
endif
MKDIR_P := mkdir -p
RM := rm -f
RMDIR := rm -rf
endif


.PHONY: client

client: $(CLEXEC)

ifeq ($(OS),Windows_NT)
	@echo "Copying Windows dependencies to .out folder..."
	@copy include\SDL2\bin\SDL2.dll .out\SDL2.dll
	@copy include\SDL2\bin\SDL2_ttf.dll .out\SDL2_ttf.dll
	@copy include\SDL2\bin\SDL2_image.dll .out\SDL2_image.dll
	@copy include\ws2_32.dll .out\ws2_32.dll
	@echo "Copying assets to .out folder..."
	@xcopy /E /I assets .out\assets
	@echo "Copying data to .out folder..."
	@xcopy /E /I data .out\data
else
	@echo "Copying assets to .out folder..."
	@cp -r assets .out/
	@echo "Copying data to .out folder..."
	@cp -r data .out/
endif

$(CLEXEC): $(CLOBJS) $(CLHDRS)
	@echo "$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS)"
	$(CC) -o $@ $(CLOBJS) $(CLFLAGS) $(LDFLAGS) && echo "EXEC [OK]  $@"

.out/client/%.o: src/client/%.c
	-@$(MKDIR_P) "$(@D)"
	@echo "$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS)"
	@$(CC) $(CLFLAGS) -c $< -o $@ $(LDFLAGS) && echo ".out/client/%.o: src/client/%.c [OK]  $@"
 
 
.PHONY: server

server: $(SVEXEC)

$(SVEXEC): $(SVOBJS) $(SVHDRS)
	@echo "$(CC) -o $@ $(SVOBJS) $(SVFLAGS)"
	$(CC) -o $@ $(SVOBJS) $(SVFLAGS) && echo "EXEC [OK]  $@"

.out/server/%.o: src/server/%.c
	-@$(MKDIR_P) "$(@D)"
	@echo "$(CC) $(SVFLAGS) -c $< -o $@"
	@$(CC) $(SVFLAGS) -c $< -o $@ && echo ".out/server/%.o: src/server/%.c [OK]  $@"
 
 
.PHONY: clean, clear

clean clear:
ifeq ($(OS),Windows_NT)
	-@$(RM) .out\src\client\* && echo "[CL] .out\client"
	-@$(RM) .out\src\server\* && echo "[CL] .out\server"
	-@$(RM) .out\ttt_client.exe && echo "[CL] .out\ttt_client.exe"
	-@$(RM) .out\ttt_server.exe && echo "[CL] .out\ttt_server.exe"
	-@$(RMDIR) .out && echo "[CL] .out"
else
	-@$(RM) .out/src/client/* && echo "[CL] .out/client"
	-@$(RM) .out/src/server/* && echo "[CL] .out/server"
	-@$(RM) .out/ttt_client && echo "[CL] .out/ttt_client"
	-@$(RM) .out/ttt_server && echo "[CL] .out/ttt_server"
	-@$(RMDIR) .out && echo "[CL] .out"
endif