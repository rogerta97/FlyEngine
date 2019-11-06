#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "imgui.h"
#include "ModuleRoomManager.h"
#include "FlyObject.h"
#include "Room.h"
#include "mmgr.h"

ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_PROPERTIES;
}

ObjectPropertiesDockPanel::~ObjectPropertiesDockPanel()
{
}

bool ObjectPropertiesDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {

		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();

		if (selectedObject != nullptr)
		{
			ImGui::Image(0, ImVec2(25, 25));
			ImGui::SameLine();

			ImGui::PushFont(App->moduleImGui->headerFont);
			ImGui::Text("%s", selectedObject->GetName().c_str());
			ImGui::PopFont();

			ImGui::Separator();

			float showPosition[2] = { selectedObject->transform->GetPosition().x, selectedObject->transform->GetPosition().y};

			if (ImGui::DragFloat2("Position", showPosition)) 
			{
				selectedObject->transform->SetPosition(float2(showPosition[0], showPosition[1])); 
			}

			if (ImGui::DragFloat2("Rotation", showPosition))
			{
				selectedObject->transform->SetPosition(float2(showPosition[0], showPosition[1]));
			}
			
			if (ImGui::DragFloat2("Scale", showPosition))
			{
				selectedObject->transform->SetPosition(float2(showPosition[0], showPosition[1]));
			}
		}
		else
		{
			ImGui::Text("No Object Selected"); 
		}
	}

	ImGui::End();
	return true;
}
