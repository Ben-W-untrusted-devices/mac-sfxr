# macOS Makefile for sfxr with SDL2
CFLAGS=-ggdb -std=c++11
CXXFLAGS=$(CFLAGS) `sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`

sfxr: main.cpp tools.h sdlkit.h
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f sfxr
