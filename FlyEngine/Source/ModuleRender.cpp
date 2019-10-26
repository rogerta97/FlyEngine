#include "Application.h"
#include "Globals.h"

#include "OpenGL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui.h"

#include "TextureMSAA.h"
#include "ModuleRender.h"
#include "Room.h"

#include "ModuleWindow.h"
#include "ModuleRoomManager.h"
#include "ViewportManager.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "3rdParty/Glew/libx86/glew32.lib")
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#include "mmgr.h"

ModuleRender::ModuleRender(bool start_enabled)
{
	moduleType = MODULE_RENDER;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	context = SDL_GL_CreateContext(App->moduleWindow->mainWindow);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false; 
	}

	ImGui::CreateContext();

	if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

	glPolygonMode(GL_FRONT, GL_LINE);
	glOrtho(-1, 1, -1, 1, -1, 1);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	//ViewportManager::getInstance()->viewportTexture->Bind();

	glClearColor(0.2f, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	GLenum err = glGetError(); 
	if (err != GL_NO_ERROR)
		FLY_ERROR("Open GL Error: %d", err); 


	SDL_GL_SwapWindow(App->moduleWindow->mainWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	SDL_GL_DeleteContext(context);
	return true;
}
