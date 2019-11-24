#include "ViewportManager.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "GameViewportDockPanel.h"
#include "ModuleWindow.h"
#include "OpenGL.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "FlyObject.h"
#include "Texture.h"
#include "Gizmos.h"
#include "TextureMSAA.h"
#include "mmgr.h"

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
	instance->viewportTexture->CleanUp();

	delete instance->viewportTexture;
	delete instance; 
}

void ViewportManager::ResizeViewport()
{
}

list<FlyObject*> ViewportManager::RaycastMouseClick()
{
	list<FlyObject*> objectCandidates = list<FlyObject*>();
	for (auto& currentObject : App->moduleRoomManager->GetSelectedRoom()->objectsInRoom)
	{
		if (currentObject->IsMouseOver())
			objectCandidates.push_back(currentObject);
	}

	return objectCandidates;
}

float ViewportManager::GetWidthFromHeight(float viewportHeight)
{
	switch (instance->viewportAspectRatio)
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

	switch (instance->viewportAspectRatio)
	{
		case AR_4_3:
			return (viewportWidth * 3) / 4;

		case AR_1_1:
			return viewportWidth;
	}

	return -1;
}
void ViewportManager::SetTextureSize(float windowWidth, float windowHeight)
{
	instance->viewportTexture->SetWidth(windowWidth);
	instance->viewportTexture->SetHeight(windowHeight);
}

float2& ViewportManager::ScreenToWorld(float2 screenPos)
{
	return App->moduleImGui->gameViewportDockPanel->ScreenToWorld(screenPos);
}

float2& ViewportManager::ScreenToWorld(float screenPosX, float screenPosY)
{
	return App->moduleImGui->gameViewportDockPanel->ScreenToWorld(screenPosX, screenPosY);
}

float ViewportManager::GetAspectRatio()
{
	return App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
}

ViewportAspectRatio ViewportManager::GetAspectRatioType()
{
	return instance->viewportAspectRatio;
}

void ViewportManager::SetAspectRatioType(ViewportAspectRatio newAR)
{
	instance->viewportAspectRatio = newAR;

	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	App->moduleImGui->gameViewportDockPanel->aspectRatioChanged = true; 

	FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject(); 

	if (selectedObject != nullptr)
	{
		selectedObject->CalculateAllGizmos(); 
	}
}
