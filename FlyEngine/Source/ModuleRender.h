#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "OpenGL.h"

class Application;
class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleRender();

public:

	//SDL_GLContext* context;

};

#endif // __ModuleWindow_H__