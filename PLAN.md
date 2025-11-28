# Plan: Getting sfxr Running on Mac

## Overview
sfxr is a sound effect generator originally written for Windows, with a Linux SDL port available. The goal is to get it running on macOS desktop.

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

## Solution Options

### Option A: Quick Native macOS Port (Recommended)
Port to modern macOS-native technologies for best user experience.

- [ ] Replace SDL 1.2 with SDL2 (already installed via Homebrew)
- [ ] Replace GTK+ file dialogs with native macOS file dialogs (using Cocoa/AppKit or SDL2's native dialog)
- [ ] Update Makefile for macOS (use `sdl2-config` instead of `sdl-config`)
- [ ] Fix hardcoded paths to work with macOS directory structure
- [ ] Test compilation and basic functionality
- [ ] Package as macOS .app bundle (optional but recommended)
- [ ] Create simple launch script or Homebrew formula

**Pros**: Native look and feel, no X11 required, uses already-installed SDL2
**Cons**: Requires code modifications

### Option B: Build with Legacy Dependencies
Install SDL 1.2 and GTK+ to build as-is.

- [ ] Install SDL 1.2 via Homebrew (`brew install sdl`)
- [ ] Install GTK+ 3 via Homebrew (`brew install gtk+3`)
- [ ] Install pkg-config dependencies
- [ ] Modify Makefile for macOS paths
- [ ] Update hardcoded resource paths
- [ ] Build and test
- [ ] Create wrapper script to set up environment

**Pros**: Minimal code changes
**Cons**: Requires X11/XQuartz, non-native UI, legacy dependencies

### Option C: Use Existing Web/Electron Version
Search for and use existing sfxr ports (jsfxr, etc.).

- [ ] Research available sfxr ports/clones
- [ ] Evaluate web-based versions (jsfxr, sfxr.me)
- [ ] Test if they meet requirements
- [ ] Package as Electron app if needed

**Pros**: No compilation needed, actively maintained
**Cons**: May not be exact port, different UI/UX

### Option D: Create Homebrew Formula for Original
Package the original SDL version for easy installation.

- [ ] Install legacy dependencies (SDL 1.2, GTK+3)
- [ ] Create Homebrew formula
- [ ] Patch source for macOS compatibility
- [ ] Test installation process
- [ ] Submit to Homebrew or maintain locally

**Pros**: Reproducible builds, standard macOS packaging
**Cons**: Still requires legacy dependencies and X11

## Recommended Approach: Option A

### Detailed Steps for Option A

#### Phase 1: Environment Setup
- [ ] Verify SDL2 installation (`sdl2-config --version`)
- [ ] Copy source files to project directory
- [ ] Create macOS-compatible Makefile

#### Phase 2: Code Modernization
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

#### Phase 3: Build System
- [ ] Create new Makefile with:
  - [ ] SDL2 flags (`sdl2-config --cflags --libs`)
  - [ ] macOS frameworks (`-framework Cocoa` if needed)
  - [ ] Proper CXXFLAGS for modern C++
- [ ] Add build target for macOS app bundle (optional)

#### Phase 4: Testing
- [ ] Compile and fix any errors
- [ ] Test basic functionality:
  - [ ] Sound generation (all preset buttons)
  - [ ] WAV export
  - [ ] Save/Load settings
  - [ ] All sliders and controls
- [ ] Verify file dialogs work
- [ ] Test on different macOS versions if possible

#### Phase 5: Distribution
- [ ] Create simple build instructions
- [ ] Add launch script if needed
- [ ] Optionally create .app bundle
- [ ] Optionally create Homebrew formula

## Quick Start Alternative: Minimal Changes Approach

If quick compilation is priority over native experience:

- [ ] Install SDL 1.2: `brew install sdl`
- [ ] Install GTK+3: `brew install gtk+3`
- [ ] Copy source to project directory
- [ ] Update Makefile: change `sdl-config` flags, remove install target
- [ ] Change hardcoded paths in `sdlkit.h:198` to relative path
- [ ] Compile: `make`
- [ ] Run: `./sfxr`

## Estimated Complexity
- **Option A (SDL2 port)**: Medium - Several hours of work
- **Option B (Legacy deps)**: Low - 30-60 minutes
- **Option C (Existing port)**: Very Low - Minutes to find and test
- **Option D (Homebrew)**: Medium - Package maintenance required

## Next Steps
1. Decide on which option to pursue
2. Set up development environment
3. Begin implementation based on chosen approach
