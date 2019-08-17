#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "SDL.h"

class Application;
class ModuleRender : public Module
{
public:

	ModuleRender(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleRender();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:

	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__