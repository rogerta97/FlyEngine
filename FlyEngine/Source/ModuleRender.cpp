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
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
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
{

}

void ModuleRender::ReceiveEvent(FlyEngineEvent eventType)
{
	switch (eventType)
	{
		case WINDOW_RESIZED:
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			float windowWidth = App->moduleImGui->gameViewportDockPanel->GetViewportSize().x;
			float windowHeight = App->moduleImGui->gameViewportDockPanel->GetViewportSize().y;

			float aspectRatio = windowWidth / windowHeight;

			glViewport(0, 0, windowWidth, windowHeight);
			glOrtho(aspectRatio, aspectRatio, -1.0f, 1.0f, 1.0, -1.0);
			
			break;
		}
	}
}

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

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glPolygonMode(GL_FRONT, GL_FILL);

	ImGui::CreateContext();

	if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

	float windowWidth = App->moduleWindow->GetWidth();
	float windowHeight = App->moduleWindow->GetHeight();

	float aspectRatio = windowWidth / windowHeight;
	glViewport(0, 0, windowWidth, windowHeight);
	glOrtho(-400.0 * aspectRatio, 400.0 * aspectRatio, -400.0, 400.0, 1.0, -1.0);

	
	FLY_WARNING("glOrtho resized in Init");

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
	//if (App->flySection == FLY_SECTION_ROOM_EDIT) {
	//	Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
	//	selectedRoom->DrawRoomObjects();
	//}

	ViewportManager::getInstance()->viewportTexture->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	if (App->flySection == FLY_SECTION_ROOM_EDIT) {
		Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		selectedRoom->DrawRoomObjects();
	}

	ViewportManager::getInstance()->viewportTexture->Render();
	ViewportManager::getInstance()->viewportTexture->Unbind();

	GLenum err = glGetError(); 
	
	if (err != GL_NO_ERROR)
		FLY_ERROR("Open GL Error: %s", glewGetErrorString(err));

	SDL_GL_SwapWindow(App->moduleWindow->mainWindow);
	
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	SDL_GL_DeleteContext(context);
	return true;
}
