#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "ModuleManager.h"
#include "ModuleInput.h"
#include "ResourceManager.h"
#include "ObjectCreatorDockPanel.h"
#include "ModuleImGui.h"

#include "RoomDockPanel.h"
#include "ImageImporter.h"
#include "TinyFileDialog.h"
#include "SaveAndLoad.h"
#include "MyFileSystem.h"

#include "Room.h"
#include "Texture.h"
#include "ViewportManager.h"
#include "FlyObject.h"
#include "mmgr.h"

RoomDockPanel::RoomDockPanel(bool isVisible) : DockPanel("Room", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_SCENE;
	playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayIcon");
}

RoomDockPanel::~RoomDockPanel()
{

}

bool RoomDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		DrawTopButtons();
		
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Room Object List"))
			{
				DrawRoomHierarchy(); 
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Viewport Settings")) 
			{
				ShowViewportSettingsTab();
				ImGui::EndTabItem();

			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
	return true;
}

void RoomDockPanel::DrawTopButtons()
{
	// Create Object Button ---
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.29f, 0.35f, 0.39f, 1.00f));
	static ImVec4 playButtonColor = ImVec4(0.35f, 0.39f, 0.50f, 1.00f);
	Texture* plusObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if(ImGui::ImageButton((ImTextureID)plusObjectTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		App->moduleImGui->objectCreatorDockPanel->ResetObjectData();
		App->moduleImGui->objectCreatorDockPanel->ToggleVisibility();
	}

	// Save Button ------------
	ImGui::SameLine();
	Texture* exportTexture = (Texture*)ResourceManager::getInstance()->GetResource("ExportIcon");
	if (ImGui::ImageButton((ImTextureID)exportTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		SaveAndLoad::getInstance()->SaveCurrentRoomData(); 
	}

	// Save Button ------------
	ImGui::SameLine();
	Texture* importIcon = (Texture*)ResourceManager::getInstance()->GetResource("ImportIcon");
	if (ImGui::ImageButton((ImTextureID)importIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		char const* lFilterPatterns[1] = { "*.json" };
		string defaultPath = MyFileSystem::getInstance()->GetSolutionDirectory(); 
		defaultPath += "Source\\Game\\Resources\\EngineSavedData\\"; 
		const char* path = tinyfd_openFileDialog("Load Image...", defaultPath.c_str(), 1, lFilterPatterns, NULL, 0);

		if (path != "")
		{
			// Clean Current Room Content
			Room* currentRoom = App->moduleRoomManager->GetSelectedRoom(); 
			currentRoom->CleanUp(); 
		
			
			// Load New Data
			SaveAndLoad::getInstance()->LoadDataToCurrentRoom(path); 
		}

	}

	// Play Button ------------
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, playButtonColor);
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 37); 
	if (ImGui::ImageButton((ImTextureID)playStopButtonTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		App->isEngineInPlayMode = !App->isEngineInPlayMode; 

		if (App->isEngineInPlayMode) {
			App->BroadCastEvent(FlyEngineEvent::ENGINE_PLAY);
			playButtonColor = ImVec4(0.50f, 0.50f, 0.80f, 1.0f);
			playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");
		}
		else {
			App->BroadCastEvent(FlyEngineEvent::ENGINE_STOP);
			playButtonColor = ImVec4(0.35f, 0.39f, 0.50f, 1.00f);
			playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayIcon");
		}
	}
	ImGui::PopStyleColor(2); 
}

void RoomDockPanel::DrawRoomHierarchy()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.5f));

	ImGui::BeginChild("##ObjectsInRoom", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y));

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::Spacing();

	if (App->moduleRoomManager->GetSelectedRoom()->GetObjectsInRoomAmount() == 0)
	{
		ImGui::TextColored(ImVec4(0,1,1,1), " There are no objects in this room"); 
	}
	else
	{
		Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		FlyObject* selectedObject = selectedRoom->GetSelectedObject();

		for (list<FlyObject*>::reverse_iterator it = selectedRoom->objectsInRoom.rbegin(); it != selectedRoom->objectsInRoom.rend(); it++) {

			PUSH_FONT(App->moduleImGui->rudaBlackMid);

			bool objectSelected = false;

			if (selectedObject != nullptr)
				objectSelected = (selectedObject->GetName() == (*it)->GetName());

			if (ImGui::Selectable(string("  " + (*it)->GetName()).c_str(), objectSelected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				(*it)->isSelected = true;
				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(*it);

				if (App->moduleInput->GetMouseButton(RIGHT_CLICK)) {
					FLY_LOG("CLICKED RIGHT BUTTON");
				}
			}

			// Move Layer Arrows
			DrawMoveLayerSelectableButtons();

			POP_FONT;
		}
	}

	ImGui::PopStyleVar();	
	ImGui::EndChild();
}

void RoomDockPanel::DrawMoveLayerSelectableButtons()
{
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 48);
	Texture* arrowUpObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowUpWhite");
	if (ImGui::ImageButton((ImTextureID)arrowUpObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr);
	}

	ImGui::SameLine();
	Texture* arrowDownObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowDownWhite");
	if (ImGui::ImageButton((ImTextureID)arrowDownObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr);
	}

	ImGui::PopStyleColor();
}

void RoomDockPanel::ShowViewportSettingsTab()
{
	ImGui::Spacing(); 

	static int resolutionSelected = 0;
	if (ImGui::Combo("Resolution", &resolutionSelected, "4:3\0 1:1\0")) {

		switch (resolutionSelected)
		{

		case 0:
			ViewportManager::getInstance()->SetAspectRatioType(AR_4_3);
			break;

		case 1:
		{
			ViewportManager::getInstance()->SetAspectRatioType(AR_1_1);
			break;
		}

		default:
			break;
		}
	}
}

