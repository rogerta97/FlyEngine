#include "GameViewportDockPanel.h"
#include "imgui.h"
#include "Application.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "mmgr.h"
#include "FlyObject.h"

GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_GAME_VIEWPORT;
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
		
		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::Button("Create Object")) {
				ImGui::OpenPopup("create_flyobject"); 
			}

			ObjectCreatorPopup();

			ImGui::EndMenuBar(); 
		}

		ImGui::Image(0, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 60));
	}

	ImGui::End();
	return true; 
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
			if (containsAttributeImage) newObject->AddAttributeImage("Path"); 

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
