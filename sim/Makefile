UNAME := $(shell uname)

COMPILER = gcc


FLAGS= -I. -I../firmware --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes  -fsingle-precision-constant
LDFLAGS=
BINEXT=

ifeq ($(UNAME), Darwin)
	COMPILER = clang
	FLAGS   += -Qunused-arguments

	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs`
endif

ifeq ($(UNAME), Linux)
	FLAGS   += `sdl-config --cflags`
	LDFLAGS += `sdl-config --libs`
endif

ifeq ($(UNAME), MINGW32_NT-6.1)
	FLAGS   += -I/MinGW/msys/1.0/include/
	LDFLAGS += -L/MinGW/msys/1.0/lib -lmingw32 -lSDLmain -lSDL
	BINEXT = .exe
endif

SOURCES=$(wildcard ../animations/*.c)
BINARIES=$(SOURCES:../animations/%.c=%$(BINEXT))

LIB_SOURCES=$(wildcard ../firmware/libs/math.c ../firmware/libs/hsv2rgb.c)
LIB_HEADERS=$(wildcard ../firmware/libs/math.h ../firmware/libs/hsv2rgb.h)

all: $(BINARIES) _all_animations

clean:
	rm -f ${BINARIES} _all_animations

%$(BINEXT): ../animations/%.c $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo -e "  \033[1;34mCompile\033[0m $< -> \033[1;33m $@\033[0m "
	@$(COMPILER)  $(FLAGS) main.c $(LIB_SOURCES) $(LDFLAGS) $< -o $@ -I. 

_all_animations: $(SOURCES) $(LIB_SOURCES) $(LIB_HEADERS) main.c main.h Makefile 
	@echo -e "  \033[1;34mCompile all animations \033[0m -> \033[1;33m _all_animations \033[0m"
	@$(COMPILER) -DFULL=1 $(FLAGS) $(LIB_SOURCES) $(SOURCES) main.c $(LDFLAGS) -o _all_animations -I. 

.PHONY : clean all

