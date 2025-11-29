# macOS Makefile for sfxr with SDL2
CFLAGS=-ggdb -std=c++11
CXXFLAGS=$(CFLAGS) `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs` -framework Cocoa

sfxr: main.cpp macos_dialogs.o tools.h sdlkit.h macos_dialogs.h
	$(CXX) main.cpp macos_dialogs.o $(CXXFLAGS) $(LDFLAGS) -o $@

macos_dialogs.o: macos_dialogs.m macos_dialogs.h
	$(CC) -c macos_dialogs.m -o macos_dialogs.o

clean:
	rm -f sfxr macos_dialogs.o
