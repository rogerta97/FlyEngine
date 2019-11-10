#include "RoomObjectsDockPanel.h"
#include "ModuleRoomManager.h"
#include "ModuleImGui.h"
#include "Application.h"
#include "Texture.h"
#include "Room.h"
#include "FlyObject.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "mmgr.h"

RoomObjectsDockPanel::RoomObjectsDockPanel(bool isVisible) : DockPanel("Room Objects", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_ROOM_OBJECTS;
}

RoomObjectsDockPanel::~RoomObjectsDockPanel()
{
}

bool RoomObjectsDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		// Searcher ---
		static char searchToolBuffer[256] = "";
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchToolBuffer, IM_ARRAYSIZE(searchToolBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		// Objects List -------
		DrawObjectHierarchy();
	}

	ImGui::End();
	return true;
}

void RoomObjectsDockPanel::DrawObjectHierarchy()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.5f));

	ImGui::BeginChild("##ObjectsInRoom", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y - 45));
	
	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::Spacing();

	Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
	FlyObject* selectedObject = selectedRoom->GetSelectedObject(); 

	for (auto& it : selectedRoom->objectsInRoom) {

		PUSH_FONT(App->moduleImGui->rudaBlackMid);

		bool objectSelected = false;

		if(selectedObject != nullptr) 
			objectSelected = (selectedObject->GetName() == (it)->GetName());

		if (ImGui::Selectable(string("  " + (it)->GetName()).c_str(), objectSelected, ImGuiSelectableFlags_AllowDoubleClick))
		{
			(it)->isSelected = true;
			App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(it);
		}
		POP_FONT;
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	DrawControlButtons();
}

void RoomObjectsDockPanel::DrawControlButtons()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	Texture* plusObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	Texture* minusObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	Texture* arrowUpObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowUpWhite");
	Texture* arrowDownObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowDownWhite");

	ImGui::ImageButton((ImTextureID)plusObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0); ImGui::SameLine();
	ImGui::ImageButton((ImTextureID)minusObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0); ImGui::SameLine();
	ImGui::ImageButton((ImTextureID)arrowUpObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0); ImGui::SameLine();
	ImGui::ImageButton((ImTextureID)arrowDownObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0);

	ImGui::PopStyleColor();
}

