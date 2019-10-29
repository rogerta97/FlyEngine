#include "GameViewportDockPanel.h"
#include "imgui.h"
#include "Application.h"
#include "TextureMSAA.h"
#include "OpenGL.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "mmgr.h"
#include "ViewportManager.h"
#include "FlyObject.h"


GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_GAME_VIEWPORT;

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
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_MenuBar)) {
		
		DrawTopBar();

		regionSize = vec2(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y - topBarWidth);

		vec2 screenCenter = vec2(regionSize.x / 2, regionSize.y / 2); 
		ImGui::SetCursorPos(ImVec2(screenCenter.x - viewportSize.x / 2, screenCenter.y - viewportSize.y / 2));

		ImGui::Image((ImTextureID)ViewportManager::getInstance()->viewportTexture->GetTextureID(), ImVec2(viewportSize.x, viewportSize.y));
	}

	ImGui::End();
	return true; 
}

void GameViewportDockPanel::ReceiveEvent(FlyEngineEvent eventType)
{
	
	switch (eventType)
	{
		case WINDOW_RESIZED:
		{
			viewportSize.x = regionSize.x; 
			viewportSize.y = ViewportManager::getInstance()->GetHeightFromWidth(viewportSize.x); 

			if (viewportSize.y > regionSize.y) 
			{
				viewportSize.y = regionSize.y; 
				viewportSize.x = ViewportManager::getInstance()->GetWidthFromHeight(viewportSize.y);
			}

			FLY_LOG("Region Size: %f , %f", regionSize.x, regionSize.y);
			FLY_LOG("Viewport Size: %f , %f", viewportSize.x, viewportSize.y);
			break;
		}
	}


}

void GameViewportDockPanel::DrawTopBar()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::PushItemWidth(150);
		if (ImGui::Button("Create Object")) {
			ImGui::OpenPopup("create_flyobject");
		}
		ImGui::PopStyleColor(); 

		ImGui::PushItemWidth(150);
		static int resolutionSelected = 0; 
		if (ImGui::Combo("", &resolutionSelected, "4:3\0")) {

		}
		ImGui::PopItemWidth(); 


		ObjectCreatorPopup();
		ImGui::EndMenuBar();
	}
}

void GameViewportDockPanel::ObjectCreatorPopup()
{
	if (ImGui::BeginPopup("create_flyobject"))
	{
		ImGui::PushFont(App->moduleImGui->headerFont);
		ImGui::Text("Object Generator: ");
		ImGui::Separator(); 
		ImGui::PopFont(); 

		static char newObjectName[256] = "Name...";
		ImGui::InputText("Name", newObjectName, 256 * sizeof(char));

		ImGui::PushFont(App->moduleImGui->headerFont);
		ImGui::Separator();
		ImGui::Text("Attributes: "); 
		ImGui::PopFont();

		static bool containsAttributeImage;
		ImGui::Checkbox("Image", &containsAttributeImage);

		if (containsAttributeImage) 
		{
			ImGui::Separator();
			if(ImGui::CollapsingHeader("Attribute Image Configuration")) 
			{
				static float dimensions[2]; 
				ImGui::InputFloat2("width", dimensions);
			}
		}

		ImGui::Separator(); 
		if (ImGui::Button("Create")) 
		{
			FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);

			if (containsAttributeImage) 
				newObject->AddAttributeImage("Path"); 

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

glm::vec2 GameViewportDockPanel::GetRegionSize() const
{
	return regionSize;
}

glm::vec2 GameViewportDockPanel::GetViewportSize() const
{
	return viewportSize;
}
