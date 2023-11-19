CC := gcc
FLAGS := -Wall
HDRS := $(wildcard src/*.h)
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=tmp/%.o) $(wildcard Gfx/*.o) $(wildcard Sfx/*.o)
EXEC := ttt

ifeq ($(OS),Windows_NT)
FLAGS += -std=c17 -I.\include\SDL2\include -L.\include\SDL2\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
FLAGS += -lSDL2 -lSDL2_ttf
endif
ifeq ($(UNAME_S),Darwin)
ifeq ($(filter %86,$(UNAME_P)),)
FLAGS += -I./include/SDL2/include -lSDL2 -lSDL2_ttf -lm
else
FLAGS += -I./include/SDL2/include -lSDL2 -lSDL2_ttf -lm
endif
endif
endif

.PHONY: all

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS)
	$(CC) -o $@ $(OBJS) $(FLAGS) && echo "EXEC [OK]  $@"


tmp/%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@ && echo "tmp/%.o: %.c [OK]  $@"


.PHONY: clean, clear

clean clear:
	@rm -f ttt && echo "[CL]  out/"
	@rm -f tmp/* && echo "[CL]  tmp/"