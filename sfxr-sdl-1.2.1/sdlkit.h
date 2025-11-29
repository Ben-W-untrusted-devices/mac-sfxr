#ifndef SDLKIT_H
#define SDLKIT_H

#include "SDL.h"
#define ERROR(x) error(__FILE__, __LINE__, #x)
#define VERIFY(x) do { if (!(x)) ERROR(x); } while (0)
#include <stdio.h>
#include <string.h>

static void error (const char *file, unsigned int line, const char *msg)
{
	fprintf(stderr, "[!] %s:%u  %s\n", file, line, msg);
	exit(1);
}

typedef Uint32 DWORD;
typedef Uint16 WORD;

#define DIK_SPACE SDLK_SPACE
#define DIK_RETURN SDLK_RETURN
#define DDK_WINDOW 0

#define hWndMain 0
#define hInstanceMain 0

#define Sleep(x) SDL_Delay(x)

// SDL2: Use SDL_NUM_SCANCODES instead of SDLK_LAST
static bool keys[SDL_NUM_SCANCODES];

void ddkInit();      // Will be called on startup
bool ddkCalcFrame(); // Will be called every frame, return true to continue running or false to quit
void ddkFree();      // Will be called on shutdown

class DPInput {
public:
	DPInput(int,int) {}
	~DPInput() {}
	static void Update () {}

	static bool KeyPressed(SDL_Keycode key)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
		bool r = keys[scancode];
		keys[scancode] = false;
		return r;
	}

};

static Uint32 *ddkscreen32;
static Uint16 *ddkscreen16;
static int ddkpitch;
static int mouse_x, mouse_y, mouse_px, mouse_py;
static bool mouse_left = false, mouse_right = false, mouse_middle = false;
static bool mouse_leftclick = false, mouse_rightclick = false, mouse_middleclick = false;

// SDL2: Use window, renderer, and texture instead of surface
static SDL_Window *sdlwindow = NULL;
static SDL_Renderer *sdlrenderer = NULL;
static SDL_Texture *sdltexture = NULL;
static SDL_Surface *sdlscreen = NULL;

static void sdlupdate ()
{
	mouse_px = mouse_x;
	mouse_py = mouse_y;
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	bool mouse_left_p = mouse_left;
	bool mouse_right_p = mouse_right;
	bool mouse_middle_p = mouse_middle;
	mouse_left = buttons & SDL_BUTTON(1);
	mouse_right = buttons & SDL_BUTTON(3);
	mouse_middle = buttons & SDL_BUTTON(2);
	mouse_leftclick = mouse_left && !mouse_left_p;
	mouse_rightclick = mouse_right && !mouse_right_p;
	mouse_middleclick = mouse_middle && !mouse_middle_p;
}

static bool ddkLock ()
{
	SDL_LockSurface(sdlscreen);
	ddkpitch = sdlscreen->pitch / (sdlscreen->format->BitsPerPixel == 32 ? 4 : 2);
	ddkscreen16 = (Uint16*)(sdlscreen->pixels);
	ddkscreen32 = (Uint32*)(sdlscreen->pixels);
	return true;
}

static void ddkUnlock ()
{
	SDL_UnlockSurface(sdlscreen);
	// Update texture with the surface data
	SDL_UpdateTexture(sdltexture, NULL, sdlscreen->pixels, sdlscreen->pitch);
	// Clear renderer
	SDL_RenderClear(sdlrenderer);
	// Copy texture to renderer
	SDL_RenderCopy(sdlrenderer, sdltexture, NULL, NULL);
	// Present renderer
	SDL_RenderPresent(sdlrenderer);
}

static void ddkSetMode (int width, int height, int bpp, int refreshrate, int fullscreen, const char *title)
{
	// Create window
	Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
	sdlwindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                              width, height, flags);
	VERIFY(sdlwindow);

	// Create renderer
	sdlrenderer = SDL_CreateRenderer(sdlwindow, -1, SDL_RENDERER_ACCELERATED);
	VERIFY(sdlrenderer);

	// Create a surface for drawing (compatible with old code)
	Uint32 pixelFormat = (bpp == 32) ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_RGB565;
	sdlscreen = SDL_CreateRGBSurfaceWithFormat(0, width, height, bpp, pixelFormat);
	VERIFY(sdlscreen);

	// Create texture
	sdltexture = SDL_CreateTexture(sdlrenderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
	VERIFY(sdltexture);
}

// Native macOS file dialogs
#include "macos_dialogs.h"

static bool load_file (char *fname)
{
	return macos_open_file_dialog(fname, 256) != 0;
}

static bool save_file (char *fname)
{
	return macos_save_file_dialog(fname, 256) != 0;
}

#define FileSelectorLoad(x,file,y) load_file(file)
#define FileSelectorSave(x,file,y) save_file(file)

static void sdlquit ()
{
	ddkFree();
	if (sdltexture) SDL_DestroyTexture(sdltexture);
	if (sdlscreen) SDL_FreeSurface(sdlscreen);
	if (sdlrenderer) SDL_DestroyRenderer(sdlrenderer);
	if (sdlwindow) SDL_DestroyWindow(sdlwindow);
	SDL_Quit();
}

static void sdlinit ()
{
	SDL_Surface *icon;
	VERIFY(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));

	// Try to load icon from current directory (remove hardcoded path)
	icon = SDL_LoadBMP("sfxr.bmp");
	if (icon)
		SDL_SetWindowIcon(sdlwindow, icon);

	atexit(sdlquit);
	memset(keys, 0, sizeof(keys));
	ddkInit();

	// Set icon after window is created
	if (icon && sdlwindow)
		SDL_SetWindowIcon(sdlwindow, icon);
}

static void loop (void)
{
	SDL_Event e;
	while (true)
	{
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					return;

				case SDL_KEYDOWN:
					keys[e.key.keysym.scancode] = true;
					break;

				default: break;
			}
		}

		sdlupdate();

		if (!ddkCalcFrame())
			return;
	}
}

int main (int argc, char *argv[])
{
	sdlinit();
	loop();
	return 0;
}

#endif
