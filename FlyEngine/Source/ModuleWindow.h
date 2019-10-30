#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();


	// Utility
	void SetTitle(const char* title);
	int GetWidth() const; 
	int GetHeight() const;
	float GetAspectRatio() const; 

public:
	SDL_Window* mainWindow;
	SDL_Surface* screen_surface;

private:

	int width; 
	int height; 
};

#endif // __ModuleWindow_H__