#include "ViewportManager.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "GameViewportDockPanel.h"
#include "ModuleWindow.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "TextureMSAA.h"

ViewportManager* ViewportManager::instance = 0;

ViewportManager::ViewportManager()
{
	viewportTexture = new TextureMSAA();
	viewportTexture->Create(App->moduleWindow->screen_surface->w, App->moduleWindow->screen_surface->h, 2);
	
	viewportAspectRatio = AR_4_3; 
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

void ViewportManager::ResizeViewport()
{
}

void ViewportManager::DrawRoomViewport()
{
	// Draw needed FlyObjects 


	// Update MSAA texture 
}
float ViewportManager::GetWidthFromHeight(float viewportHeight)
{
	switch (viewportAspectRatio)
	{
		case AR_4_3:
			return (viewportHeight * 4) / 3; 

		case AR_1_1:
			return viewportHeight;
	}

	return -1; 
}
float ViewportManager::GetHeightFromWidth(float viewportWidth)
{
	float2 regionSize = App->moduleImGui->gameViewportDockPanel->GetRegionSize(); 

	switch (viewportAspectRatio)
	{
		case AR_4_3:
			return (viewportWidth * 3) / 4;

		case AR_1_1:
			return viewportWidth;
	}

	return -1;
}
ViewportAspectRatio ViewportManager::GetAspectRatioType() const
{
	return viewportAspectRatio;
}

void ViewportManager::SetAspectRatioType(ViewportAspectRatio newAR)
{
	viewportAspectRatio = newAR;
	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	App->moduleImGui->ReceiveEvent(WINDOW_RESIZED); 
}
