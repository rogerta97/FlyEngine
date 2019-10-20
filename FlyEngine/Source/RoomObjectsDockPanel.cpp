#include "RoomObjectsDockPanel.h"
#include "ModuleWorldManager.h"
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
		for (auto& it : App->moduleWorldManager->GetSelectedRoom()->objectsInRoom) {	
			ImGui::Selectable((it)->GetName().c_str()); 
		}
	}

	ImGui::End();
	return true;
}

