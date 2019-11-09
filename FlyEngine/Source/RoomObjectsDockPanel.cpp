#include "RoomObjectsDockPanel.h"
#include "ModuleRoomManager.h"
#include "ModuleImGui.h"
#include "Application.h"
#include "Room.h"
#include "FlyObject.h"
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

		//ImGui::PushFont(App->moduleImGui->rudaBoldFont);
		//ImGui::Text("Bold Font");
		//ImGui::PopFont();
		//
		//ImGui::PushFont(App->moduleImGui->rudaBlackFont);
		//ImGui::Text("Black Font"); 
		//ImGui::PopFont();
		//
		//ImGui::PushFont(App->moduleImGui->rudaRegularFont);
		//ImGui::Text("Regular Font");
		//ImGui::PopFont();

		for (auto& it : App->moduleRoomManager->GetSelectedRoom()->objectsInRoom) {	

			if (ImGui::Selectable((it)->GetName().c_str())) {
				(it)->isSelected = true; 
				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(it);
			}
		}
	}

	ImGui::End();
	return true;
}

