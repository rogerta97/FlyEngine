#include "GameViewportDockPanel.h"
#include "imgui.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "OpenGL.h"
#include "Gizmos.h"
#include "Room.h"
#include "ViewportManager.h"
#include "ModuleImGui.h"
#include "ResourceManager.h"
#include "ModuleWorldManager.h"
#include "mmgr.h"
#include "Texture.h"
#include "ModuleWindow.h"
#include "ViewportManager.h"
#include "FlyObject.h"
#include "imgui_internal.h"

GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_GAME_VIEWPORT;

	regionSize = float2(-1.0f, -1.0f);
	viewportSize = float2(-1.0f, -1.0f);

	aspectRatioChanged = false; 
	gizmoMode = GIZMO_SELECT; 

	backgroundColor = ImVec4(0.09f, 0.11f, 0.13f, 1.0f); 
}

GameViewportDockPanel::~GameViewportDockPanel()
{
}

bool GameViewportDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;

	if (App->moduleWorldManager->GetSelectedRoom() == nullptr)
		return false; 

#pragma endregion

	glEnable(GL_TEXTURE_2D);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColor);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

	if (App->GetGameMode() == PLAY_MODE)
	{
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
	}

	if (ImGui::Begin(panelName.c_str(), &visible, windowFlags))
	{		
		if (App->GetGameMode() == PLAY_MODE)
		{
			if(!ImGui::GetCurrentWindow()->DockNode->IsHiddenTabBar())
				ImGui::GetCurrentWindow()->DockNode->WantHiddenTabBarToggle = true; 
		}

		CalculateViewport();

		float2 screenCenter = float2(regionSize.x / 2, regionSize.y / 2);
		viewportCenterGlobalPos = float2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + screenCenter;

		ImGui::SetCursorPos(ImVec2(screenCenter.x - viewportSize.x / 2, screenCenter.y - (viewportSize.y / 2) + (verticalOffset)));
		ImGui::Image((ImTextureID)ViewportManager::getInstance()->viewportTexture->GetTextureID(), ImVec2(viewportSize.x - 1, viewportSize.y - 2));

		if(!initialized)
			initialized = true;
	}

	if (ImGui::IsWindowFocused())	
		ViewportManager::getInstance()->blockInput = false;

	glDisable(GL_TEXTURE_2D);

	ImGui::End();
	ImGui::PopStyleColor(); 
	ImGui::PopStyleVar();
	 
	return true; 
}

void GameViewportDockPanel::CalculateViewport()
{
	float2 regionSizeThisTick = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	if (!regionSizeThisTick.Equals(regionSize) && regionSize.x != -1.0f)
	{
		regionSize = regionSizeThisTick;
		FitViewportToRegion();
	}

	float titleBarHeight = ImGui::GetCurrentWindow()->TitleBarHeight();
	float menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

	//verticalOffset = titleBarHeight + menuBarHeight;
	verticalOffset = titleBarHeight;
	regionSize = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	//regionSize.y += verticalOffset;

	if (fitViewportNextFrame)
	{
		FitViewportToRegion();
		fitViewportNextFrame = false;
	}
}

void GameViewportDockPanel::ReceiveEvent(FlyEngineEvent eventType)
{	
	switch (eventType)
	{
		case FlyEngineEvent::WINDOW_RESIZED:
		{
			FitViewportToRegion();
			break;
		}

		case FlyEngineEvent::ENGINE_PLAY:
		{
			// Change background color
			backgroundColor = ImVec4(0.50f, 0.50f, 0.80f, 0.25f);
			break;
		}

		case FlyEngineEvent::ENGINE_STOP:
		{
			// Change background color
			backgroundColor = ImVec4(0.09f, 0.11f, 0.13f, 1.0f);
			break;
		}	
	}
}

bool GameViewportDockPanel::IsMouseInViewport()
{
	float2 mousePositionRelative = GetMouseRelativePosition(); 
	if (mousePositionRelative.x < -viewportSize.x / 2 || mousePositionRelative.x > viewportSize.x / 2 ||
		mousePositionRelative.y > viewportSize.y / 2 || mousePositionRelative.y < -viewportSize.y / 2)
	{
		return false; 
	}

	return true; 
}

void GameViewportDockPanel::FitViewportToRegion()
{
	if (regionSize.x <= 0 || regionSize.y <= 0)
	{
		fitViewportNextFrame = true; 
		return;
	}

	viewportSize.x = regionSize.x;
	viewportSize.y = ViewportManager::getInstance()->GetHeightFromWidth(viewportSize.x);

	if (viewportSize.y > regionSize.y)
	{
		viewportSize.y = regionSize.y;
		viewportSize.x = ViewportManager::getInstance()->GetWidthFromHeight(viewportSize.y);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = viewportSize.x / viewportSize.y; 
	glViewport(0, 0, viewportSize.x, viewportSize.y); 
	glOrtho(-500.0 * aspectRatio, 500.0 * aspectRatio, -500.0, 500.0, 1.0, -1.0);
}



float2& GameViewportDockPanel::GetRegionSize() 
{
	return regionSize;
}

float2& GameViewportDockPanel::GetViewportSize() 
{
	return viewportSize;
}

float GameViewportDockPanel::GetAspectRatio()
{
	return viewportSize.x / viewportSize.y;
}

GizmoMode GameViewportDockPanel::GetGizmoMode() const
{
	return gizmoMode;
}

void GameViewportDockPanel::SetGizmoMode(GizmoMode _gizmoMode)
{
	gizmoMode = _gizmoMode; 
}

float2& GameViewportDockPanel::GetViewportCenterGlobal()
{
	return viewportCenterGlobalPos; 
}

float2& GameViewportDockPanel::GetMouseRelativePosition()
{
	float2 mouseRelativePos = float2(ImGui::GetMousePos().x - viewportCenterGlobalPos.x, ImGui::GetMousePos().y - viewportCenterGlobalPos.y - (verticalOffset));
	return mouseRelativePos; 
}

float2 GameViewportDockPanel::GetMouseGamePos()
{
	float2 ret = GetMouseRelativePosition(); 
	ret = float2((int)ScreenToWorld(ret).x, (int)ScreenToWorld(ret).y); 
	return ret; 
}

float2& GameViewportDockPanel::ScreenToWorld(float2 screenPos)
{
	return ScreenToWorld(screenPos.x, screenPos.y);
}

float2& GameViewportDockPanel::ScreenToWorld(float screenPosX, float screenPosY)
{
	float screenMaxX = viewportSize.x / 2; 
	float worldMaxX = 500.0f;

	float screenMaxY = viewportSize.y / 2;
	float worldMaxY = 500.0f;

	float2 returnV; 
	returnV.x = (screenPosX * worldMaxX) / screenMaxX;
	returnV.y = (screenPosY * worldMaxY) / screenMaxY;

	returnV.y /= viewportSize.x / viewportSize.y; 

	return returnV;
}

float2& GameViewportDockPanel::WorldToScreen(float screenPosX, float screenPosY)
{
	float screenMaxX = viewportSize.x / 2;
	float worldMaxX = 500.0f;

	float screenMaxY = viewportSize.y / 2;
	float worldMaxY = 500.0f;

	float2 returnV;
	returnV.x = (screenPosX / worldMaxX) * screenMaxX;
	returnV.y = (screenPosY / worldMaxY) * screenMaxY;

	returnV.y /= viewportSize.x / viewportSize.y;

	return returnV;
}
