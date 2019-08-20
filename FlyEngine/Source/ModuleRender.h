#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#include "Module.h"
#include "Globals.h"

class ModuleRender : public Module
{
public:
	ModuleRender(bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:

	SDL_GLContext context;

};

#endif