# Next Steps for sfxr macOS

## Current Status
- ✅ Successfully ported to macOS with SDL2
- ✅ Native macOS file dialogs (Cocoa)
- ✅ Working from both terminal and Finder (double-click)
- ✅ All functionality working: UI, sound generation, save/load, WAV export

## Future Enhancement Options

### Option 1: Convert to Xcode/Swift App

**Difficulty: Medium to Hard**

#### Approach A: Wrapper (Medium - 2-4 hours)
- Keep the C++ rendering/audio code as-is
- Create Swift wrapper around it using Objective-C++ bridging
- Replace SDL2 window/rendering with native AppKit views
- Create a proper Swift/AppKit UI that renders to a buffer the C++ code can draw to
- Similar to what we did with native file dialogs, but more extensive

**Pros:**
- Native macOS app bundle
- Better integration with macOS
- Could add proper menu bar, preferences, etc.

**Cons:**
- Still has C++ core
- Mixed language project complexity

#### Approach B: Full Rewrite (Hard - 8-16 hours)
- Port all sound synthesis algorithms to Swift
- Use AVFoundation for audio instead of SDL2
- Create a proper SwiftUI or AppKit interface
- Maintain all the same sound parameters and generation logic

**Pros:**
- Pure Swift codebase
- Modern macOS patterns
- Easier to maintain long-term

**Cons:**
- Complete rewrite required
- Lots of testing needed to ensure synthesis sounds identical

#### Key Challenges:
- Code heavily relies on direct pixel buffer manipulation (`ddkscreen32` array)
- Lots of C-style code (global variables, raw pointers, manual memory management)
- SDL2 audio callback system would need conversion to AVAudioEngine
- Sprite/font rendering is all custom bitmap manipulation

#### What's Already Easy:
- We already have native macOS file dialogs working
- Sound synthesis math is straightforward to port
- No complex dependencies besides SDL2

### Option 2: Add Command-Line Interface

**Difficulty: Easy to Medium**

**IMPORTANT:** Keep all existing GUI code intact. The CLI would be an additional mode, not a replacement.

#### Basic Version (Easy - 30 minutes to 1 hour)
- Add command-line argument detection (check `argc`)
- If CLI args present, skip GUI initialization
- Parse arguments for sound parameters
- Export WAV file directly without GUI
- Example: `sfxr --type laser --frequency 0.5 --attack 0.1 output.wav`

**Implementation approach:**
- Check for CLI args in `main()` before calling `sdlinit()`
- If args detected, call synthesis functions directly
- Otherwise, launch GUI as normal

#### Medium Version (Medium - 2-3 hours)
- Support loading .sfxr parameter files from command line
- Add preset generators (pickup, laser, explosion, etc.) as CLI flags
- Support randomization with optional seed
- Parameter ranges for batch generation
- Example: `sfxr --preset explosion --randomize --seed 42 boom.wav`
- Example: `sfxr --load mysound.sfxr --output mysound.wav`

#### Full Version (Medium - 4-6 hours)
- Interactive REPL mode to adjust parameters
- Real-time preview playback (using SDL audio only, no window)
- Parameter templates and saving
- Batch processing mode
- Example: `sfxr --interactive` with live preview while tweaking values

#### Key Advantages:
1. **Very easy parts:**
   - Synthesis code in `SynthSample()` and `ExportWAV()` already works headless
   - Parameter structure is already well-defined
   - WAV export is completely decoupled from GUI
   - Can reuse existing preset functions and `LoadSettings()`

2. **Slightly tricky parts:**
   - Mapping all 40+ sliders/parameters to sensible CLI arguments
   - Real-time audio preview without GUI (would still need SDL audio init)
   - Making parameter names user-friendly

3. **Not needed:**
   - No changes to SDL video/window/rendering
   - No changes to font rendering or mouse input
   - No changes to existing GUI code at all

#### Example Usage Patterns:
```bash
# GUI mode (default)
./sfxr

# Generate from preset
sfxr --preset laser --output laser.wav

# Load and export existing sound
sfxr --load mysound.sfxr --output mysound.wav

# Randomize and export
sfxr --preset explosion --randomize --output boom.wav

# Batch generation
sfxr --preset pickup --count 10 --output-dir ./sounds/

# Interactive mode with preview
sfxr --interactive
```

## Recommendation

**Easiest next step:** Add basic CLI support (30-60 minutes of work)
- Very high value-to-effort ratio
- Enables automation and batch generation
- Doesn't affect existing GUI functionality at all
- Great for game developers who want to generate sounds in build scripts

**Best long-term:** Wrapper approach for Xcode/Swift (2-4 hours)
- Proper macOS app that feels native
- Keep proven synthesis engine
- Can still maintain CLI mode
- Easier to distribute (no SDL2 dependency for end users)
