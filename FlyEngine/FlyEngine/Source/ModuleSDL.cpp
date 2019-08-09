#include "Globals.h"
#include "Application.h"
#include "ModuleSDL.h"

ModuleSDL::ModuleSDL(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

// Destructor
ModuleSDL::~ModuleSDL()
{
	
}

// Called before render is available
bool ModuleSDL::Init()
{
	Uint32 flags = SDL_INIT_EVERYTHING;
	SDL_Init(flags);

	return true;
}

// Called before quitting
bool ModuleSDL::CleanUp()
{
	return true;
}
