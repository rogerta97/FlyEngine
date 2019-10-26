#include "ViewportManager.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "ModuleWindow.h"
#include "Room.h"
#include "TextureMSAA.h"

ViewportManager* ViewportManager::instance = 0;

ViewportManager::ViewportManager()
{
	viewportTexture = new TextureMSAA();
	viewportTexture->Create(App->moduleWindow->screen_surface->w, App->moduleWindow->screen_surface->h, 2);
}

ViewportManager* ViewportManager::getInstance()
{
	if (instance == nullptr)
		instance = new ViewportManager(); 

	return instance;
}

ViewportManager::~ViewportManager()
{
}

void ViewportManager::Delete()
{
	delete instance; 
}

void ViewportManager::DrawRoomViewport()
{
	// Draw needed FlyObjects 


	// Update MSAA texture 
}
