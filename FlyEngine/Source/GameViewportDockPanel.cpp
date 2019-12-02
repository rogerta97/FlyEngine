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
#include "ModuleRoomManager.h"
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

	if (App->moduleRoomManager->GetSelectedRoom() == nullptr)
		return false; 

#pragma endregion

	glEnable(GL_TEXTURE_2D);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColor);

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_MenuBar)) 
	{
		DrawTopBar(); 
		
		float2 regionSizeThisTick = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		if (!regionSizeThisTick.Equals(regionSize) && regionSize.x != -1.0f)
		{
			regionSize = regionSizeThisTick;
			FitViewportToRegion();
		}

		float titleBarHeight = ImGui::GetCurrentWindow()->TitleBarHeight(); 
		float menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

		verticalOffset = titleBarHeight + menuBarHeight;
		regionSize = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		regionSize.y += verticalOffset;

		float2 screenCenter = float2(regionSize.x / 2, regionSize.y / 2);
		viewportCenterGlobalPos = float2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + screenCenter;

		ImGui::SetCursorPos(ImVec2(screenCenter.x - viewportSize.x / 2, screenCenter.y - (viewportSize.y / 2) + (verticalOffset / 2)));
		ImGui::Image((ImTextureID)ViewportManager::getInstance()->viewportTexture->GetTextureID(), ImVec2(viewportSize.x - 1, viewportSize.y - 2));
	}

	glDisable(GL_TEXTURE_2D);

	ImGui::End();
	ImGui::PopStyleColor(); 
	ImGui::PopStyleVar();
	 
	return true; 
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

void GameViewportDockPanel::DrawTopBar()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0, 0, 0, 0.2f)); 
	ImGui::BeginMenuBar();

	// Gizmos ------------------
	Texture* arrowSelect = (Texture*)ResourceManager::getInstance()->GetResource("SelectArrow");

	bool currentMode = false; 
	if (gizmoMode == GIZMO_SELECT) {
		currentMode = true; 
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(2, 2, 2, 0.2f));
	}

	if (ImGui::ImageButton((ImTextureID)arrowSelect->GetTextureID(), ImVec2(16, 16)))
	{
		gizmoMode = GIZMO_SELECT;
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject(); 
		if (selectedObject != nullptr)
		{
			selectedObject->CalculateCurrentGizmo();
		}	
	}

	if (currentMode) {
		currentMode = false; 
		ImGui::PopStyleColor(); 
	}

	currentMode = false;
	if (gizmoMode == GIZMO_MOVE) {
		currentMode = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(2, 2, 2, 0.2f));
	}

	ImGui::SameLine();
	Texture* moveArrows = (Texture*)ResourceManager::getInstance()->GetResource("MoveOption");
	if (ImGui::ImageButton((ImTextureID)moveArrows->GetTextureID(), ImVec2(16, 16)))
	{
		gizmoMode = GIZMO_MOVE;
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();
		if (selectedObject != nullptr)
		{
			selectedObject->CalculateCurrentGizmo();
		}
	}

	if (currentMode) {
		currentMode = false;
		ImGui::PopStyleColor();
	}

	ImGui::EndMenuBar();
	ImGui::PopStyleColor();
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

float2& GameViewportDockPanel::GetViewportCenterGlobal()
{
	return viewportCenterGlobalPos; 
}

float2& GameViewportDockPanel::GetMouseRelativePosition()
{
	float2 mouseRelativePos = float2(ImGui::GetMousePos().x - viewportCenterGlobalPos.x, ImGui::GetMousePos().y - viewportCenterGlobalPos.y - (verticalOffset / 2));
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
