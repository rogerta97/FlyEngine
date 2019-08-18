#include "ModuleRender.h"
#include "Application.h"

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleRender::~ModuleRender()
{

}

bool ModuleRender::Init() {

	bool ret = true; 

	context = SDL_GL_CreateContext(App->moduleWindow->mainWindow); 

	if (context == NULL) {
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	glewInit();
	SDL_GL_SetSwapInterval(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glClearColor(BACKGROUND_COLOR);

	return ret; 
}

update_status ModuleRender::PostUpdate(float dt) {

	SDL_GL_SwapWindow(App->moduleWindow->mainWindow); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glClearColor(BACKGROUND_COLOR);

	return UPDATE_CONTINUE; 
}

bool ModuleRender::CleanUp() {

	SDL_GL_DeleteContext(context); 
	return true; 
}


