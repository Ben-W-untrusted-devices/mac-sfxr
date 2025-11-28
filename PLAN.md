# Plan: Getting sfxr Running on Mac

## Overview
sfxr is a sound effect generator originally written for Windows, with a Linux SDL port available. The goal is to get it running on macOS desktop with native technologies.

## Current State Analysis
- **Source code**: sfxr-sdl-1.2.1 (SDL/Linux port from 2012)
- **Dependencies**: SDL 1.2, GTK+ 2.0/3.0 (for file dialogs)
- **Code**: Single C++ file (main.cpp), custom SDL wrapper (sdlkit.h)
- **License**: MIT
- **Homebrew**: No existing formula for sfxr

## Key Challenges
1. Uses SDL 1.2 (legacy version from 2012)
2. Currently system has SDL2 installed
3. Uses GTK+ for file dialogs (not native to macOS)
4. Hardcoded Linux paths (e.g., `/usr/share/sfxr/`)

## Approach: Native macOS Port
Port to modern macOS-native technologies for best user experience.

**Pros**: Native look and feel, no X11 required, uses already-installed SDL2
**Cons**: Requires code modifications

## Implementation Steps

### Phase 1: Environment Setup
- [ ] Verify SDL2 installation (`sdl2-config --version`)
- [ ] Copy source files to project directory
- [ ] Create macOS-compatible Makefile

### Phase 2: Code Modernization
- [ ] Update SDL 1.2 API calls to SDL2 equivalents:
  - [ ] `SDL_SetVideoMode` → `SDL_CreateWindow` + `SDL_CreateRenderer`
  - [ ] `SDL_WM_SetCaption` → `SDL_SetWindowTitle`
  - [ ] `SDL_Flip` → `SDL_RenderPresent`
  - [ ] Surface-based rendering → Texture-based rendering
  - [ ] `SDL_LoadBMP` → Update for SDL2
  - [ ] Audio initialization for SDL2
- [ ] Replace GTK+ file dialogs with macOS native dialogs:
  - [ ] Use SDL2's native file dialog (if available)
  - [ ] Or use Objective-C/Cocoa dialogs with C++ wrapper
  - [ ] Or use simple command-line fallback
- [ ] Fix resource path handling:
  - [ ] Remove hardcoded `/usr/share/sfxr/` paths
  - [ ] Use relative paths or app bundle resources
  - [ ] Add macOS bundle support

### Phase 3: Build System
- [ ] Create new Makefile with:
  - [ ] SDL2 flags (`sdl2-config --cflags --libs`)
  - [ ] macOS frameworks (`-framework Cocoa` if needed)
  - [ ] Proper CXXFLAGS for modern C++
- [ ] Add build target for macOS app bundle (optional)

### Phase 4: Testing
- [ ] Compile and fix any errors
- [ ] Test basic functionality:
  - [ ] Sound generation (all preset buttons)
  - [ ] WAV export
  - [ ] Save/Load settings
  - [ ] All sliders and controls
- [ ] Verify file dialogs work
- [ ] Test on different macOS versions if possible

### Phase 5: Distribution
- [ ] Create simple build instructions
- [ ] Add launch script if needed
- [ ] Optionally create .app bundle
- [ ] Optionally create Homebrew formula

## Estimated Complexity
Medium - Several hours of work for full SDL2 port with native dialogs
