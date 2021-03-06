#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ViewportManager.h"
#include "SDL.h"
#include "mmgr.h"

ModuleWindow::ModuleWindow(bool start_enabled)
{
	mainWindow = NULL;
	screen_surface = NULL;
	moduleType = MODULE_WINDOW;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

void ModuleWindow::SetIcon(const char* image_path)
{
	if (image_path)
	{
		SDL_Surface* surface = SDL_LoadBMP(image_path);
		SDL_SetWindowIcon(mainWindow, surface);
		SDL_FreeSurface(surface);
	}
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (FLY_FULLSCREEN == true)
		{
			SDL_DisplayMode DM;
			SDL_GetDesktopDisplayMode(0, &DM);
			width = DM.w - 10;
			height = DM.h - 90;
		}

		mainWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(mainWindow == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
			screen_surface = SDL_GetWindowSurface(mainWindow);
		
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(mainWindow != NULL)
		SDL_DestroyWindow(mainWindow);
	
	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::ReceiveEvent(FlyEngineEvent newEvent)
{
	switch (newEvent)
	{
	case WINDOW_RESIZED:	
		SDL_GetWindowSize(this->mainWindow, &width, &height); 
		screen_surface->w = width; 
		screen_surface->h = height; 

		break;
	}
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(mainWindow, title);
}

int ModuleWindow::GetWidth() const
{
	return width; 
}

int ModuleWindow::GetHeight() const
{
	return height;
}

float ModuleWindow::GetAspectRatio() const
{
	return (float)width/ (float)height;
}
