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

		Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		if (ImGui::ImageButton((ImTextureID)searchIcon->GetTextureID(), ImVec2(18, 18))) {

		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Objects List -------
		for (auto& it : App->moduleRoomManager->GetSelectedRoom()->objectsInRoom) {

			PUSH_FONT(App->moduleImGui->rudaRegularMid);

			if (ImGui::Selectable(string("- " + (it)->GetName()).c_str())) 
			{
				(it)->isSelected = true;
				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(it);
			}
			POP_FONT;
		}
	}

	ImGui::End();
	return true;
}

