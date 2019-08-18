#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "OpenGL.h"

#pragma comment (lib, "glu32.lib")							/* link OpenGL Utility lib */
#pragma comment (lib, "opengl32.lib")						/* link Microsoft OpenGL lib */
#pragma comment (lib, "3rdParty/Glew/libx86/glew32.lib")    /* link GLEW lib */

#define BACKGROUND_COLOR 0.1f, 0.1f, 0.1f, 1.0f

class Application;
class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	virtual ~ModuleRender();

	// Flow 
	bool Init(); 
	update_status PostUpdate(float dt); 
	bool CleanUp(); 

public:

	SDL_GLContext context;

};

#endif // __ModuleWindow_H__