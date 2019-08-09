#ifndef __ModuleSDL_H__
#define __ModuleSDL_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleSDL : public Module
{
public:

	ModuleSDL(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleSDL();

	bool Init();
	bool CleanUp();

};

#endif //