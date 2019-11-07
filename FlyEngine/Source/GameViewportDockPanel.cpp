#include "GameViewportDockPanel.h"
#include "imgui.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "OpenGL.h"
#include "Room.h"
#include "ViewportManager.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "mmgr.h"
#include "ModuleWindow.h"
#include "ViewportManager.h"
#include "FlyObject.h"

GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_GAME_VIEWPORT;

	regionSize = float2(-1.0f, -1.0f);
	viewportSize = float2(-1.0f, -1.0f);

	aspectRatioChanged = false; 
	topBarWidth = 60; 
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
	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_MenuBar)) {
		
		float2 regionSizeThisTick = float2(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);

		if (!regionSizeThisTick.Equals(regionSize) && regionSize.x != -1.0f)
		{
			regionSize = regionSizeThisTick;
			FitViewportToRegion();

			//if (aspectRatioChanged)
			//{
			//	glMatrixMode(GL_PROJECTION);
			//	glLoadIdentity();

			//	float windowWidth = viewportSize.x;
			//	float windowHeight = viewportSize.y;

			//	glViewport(0, 0, windowWidth, windowHeight);

			//	aspectRatioChanged = false;
			//}
		}

		DrawTopBar();

		regionSize = float2(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);

		float2 screenCenter = float2(regionSize.x / 2, regionSize.y / 2);
		ImGui::SetCursorPos(ImVec2(screenCenter.x - viewportSize.x / 2, screenCenter.y - (viewportSize.y / 2)));

		ImGui::Image((ImTextureID)ViewportManager::getInstance()->viewportTexture->GetTextureID(), ImVec2(viewportSize.x - 1, viewportSize.y - 1), ImVec2(0, 1), ImVec2(1, 0));
	}

	glDisable(GL_TEXTURE_2D);

	ImGui::End();
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
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::PushItemWidth(150);
		if (ImGui::Button("Create Object")) 
		{
			ImGui::OpenPopup("create_flyobject");
			ResetAttributeSelectionBooleans();
		}
		ImGui::PopStyleColor(); 

		ImGui::PushItemWidth(150);

		ImGui::PopItemWidth(); 

		ObjectCreatorPopup();
		ImGui::EndMenuBar();
	}
}

void GameViewportDockPanel::ResetAttributeSelectionBooleans()
{
	//containsAttributeImage = false;
}

void GameViewportDockPanel::ObjectCreatorPopup()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
	if (ImGui::BeginPopup("create_flyobject"))
	{
		
	}
	ImGui::PopStyleVar();
}

float2 GameViewportDockPanel::GetRegionSize() const
{
	return regionSize;
}

float2 GameViewportDockPanel::GetViewportSize() const
{
	return viewportSize;
}
