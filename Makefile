CC=D:\LLVM\bin\clang.exe 
CPPCHECK=D:\CPPCHECK\cppcheck.exe
CFLAGS=-Wall -Werror -Wno-pragma-pack -Wno-unused-function -IC:\Users\marco\Desktop\SDL2-2.0.9\include
LDFLAGS=-lSDL2 -L. 
BINARY=main

ifeq ($(OS),Windows_NT)
	BINARY:=$(BINARY).exe
	BINARY_TESTS:=$(BINARY_TESTS).exe
endif

main: main.o aiv_rasterizer.o aiv_renderer.o aiv_math.o
	$(CC) -o $(BINARY) $(LDFLAGS) $^

main.o: main.c aiv_renderer.h
	$(CC) -c -o $@ $(CFLAGS) $<

aiv_math.o: aiv_math.c aiv_math.h
	$(CC) -c -o $@ $(CFLAGS) $<


aiv_rasterizer.o: aiv_rasterizer.c aiv_rasterizer.h aiv_math.h
	$(CC) -c -o $@ $(CFLAGS) $<


aiv_renderer.o: aiv_renderer.c aiv_renderer.h aiv_rasterizer.h
	$(CC) -c -o $@ $(CFLAGS) $<


