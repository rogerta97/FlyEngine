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
	gizmoMode = GIZMO_null; 
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
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09f, 0.11f, 0.13f, 1.0f));

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

		regionSize = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		regionSize.y += (titleBarHeight + menuBarHeight);

		float2 screenCenter = float2(regionSize.x / 2, regionSize.y / 2);
		viewportCenterGlobalPos = float2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + screenCenter;

		ImGui::SetCursorPos(ImVec2(screenCenter.x - viewportSize.x / 2, screenCenter.y - (viewportSize.y / 2) + menuBarHeight));
		ImGui::Image((ImTextureID)ViewportManager::getInstance()->viewportTexture->GetTextureID(), ImVec2(viewportSize.x - 1, viewportSize.y - 2));

		//FLY_WARNING("Viewport Center: %f %f", viewportCenterGlobalPos.x, viewportCenterGlobalPos.y);

	}

	glDisable(GL_TEXTURE_2D);

	GetMouseRelativePosition();

	ImGui::End();
	ImGui::PopStyleColor(); 
	ImGui::PopStyleVar();
	 
	return true; 
}

void GameViewportDockPanel::ReceiveEvent(FlyEngineEvent eventType)
{	
	switch (eventType)
	{
		case WINDOW_RESIZED:
		{
			FitViewportToRegion();
			break;
		}
	}
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

	float aspectRatio = viewportSize.x / viewportSize.y; 
	glViewport(0, 0, viewportSize.x, viewportSize.y); 
	glOrtho(-400.0 * aspectRatio, 400.0 * aspectRatio, -400.0, 400.0, 1.0, -1.0);
}

void GameViewportDockPanel::DrawTopBar()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0, 0, 0, 0.2f)); 
	ImGui::BeginMenuBar();

	Texture* arrowSelect = (Texture*)ResourceManager::getInstance()->GetResource("SelectArrow");

	bool currentMode = false; 
	if (gizmoMode == GIZMO_SELECT) {
		currentMode = true; 
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(35, 35, 37, 1.0f));
	}

	if (ImGui::ImageButton((ImTextureID)arrowSelect->GetTextureID(), ImVec2(17, 17)))
	{
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject(); 
		if (selectedObject != nullptr)
		{
			selectedObject->gizmos->FitBoxToObject(); 
		}	
		gizmoMode = GIZMO_SELECT;
	}

	if (currentMode) {
		currentMode = false; 
		ImGui::PopStyleColor(); 
	}

	currentMode = false;
	if (gizmoMode == GIZMO_MOVE) {
		currentMode = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(35, 35, 37, 1.0f));
	}

	ImGui::SameLine();
	Texture* moveArrows = (Texture*)ResourceManager::getInstance()->GetResource("MoveOption");
	if (ImGui::ImageButton((ImTextureID)moveArrows->GetTextureID(), ImVec2(17, 17)))
	{
		gizmoMode = GIZMO_MOVE;
	}

	if (currentMode) {
		currentMode = false;
		ImGui::PopStyleColor();
	}

	ImGui::EndMenuBar();
	ImGui::PopStyleColor();
}

float2 GameViewportDockPanel::GetRegionSize() const
{
	return regionSize;
}

float2 GameViewportDockPanel::GetViewportSize() const
{
	return viewportSize;
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
	float2 mouseRelativePos = float2(ImGui::GetMousePos().x, ImGui::GetMousePos().y) - viewportCenterGlobalPos; 
	//FLY_LOG("Relative Mouse Position: %f, %f", mouseRelativePos.x, mouseRelativePos.y); 
	return mouseRelativePos; 
}
