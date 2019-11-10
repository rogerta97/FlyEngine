#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "imgui.h"
#include "ModuleRoomManager.h"
#include "ResourceManager.h"
#include "Texture.h"
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
			Texture* objectIconTextue = (Texture*)ResourceManager::getInstance()->GetResource("ObjectIcon");
			ImGui::Image((ImTextureID)objectIconTextue->GetTextureID(), ImVec2(35, 35));

			ImGui::Separator();

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45, ImGui::GetCursorPosY() - 27));

			ImGui::PushFont(App->moduleImGui->rudaBoldFontBig);
			ImGui::Text("%s", selectedObject->GetName().c_str());
			ImGui::PopFont();

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushFont(App->moduleImGui->rudaBoldFont);
			if (ImGui::CollapsingHeader("Object Placement")) 
			{
				float showPosition[2] = { selectedObject->transform->GetPosition().x, selectedObject->transform->GetPosition().y};
				float showRotation[2] = { selectedObject->transform->GetRotation().x, selectedObject->transform->GetRotation().y};
				float showScale[2] = { selectedObject->transform->GetScale().x, selectedObject->transform->GetScale().y};

				PUSH_FONT(App->moduleImGui->rudaBlackFont);
				if (ImGui::DragFloat2("Position", showPosition, 0.5f)) 
				{
					selectedObject->transform->SetPosition(float2(showPosition[0], showPosition[1])); 
				}

				if (ImGui::DragFloat2("Rotation", showRotation))
				{
					selectedObject->transform->SetRotationEuler(float2(showRotation[0], showRotation[1]));
				}
			
				if (ImGui::DragFloat2("Scale", showScale, 0.1f))
				{
					selectedObject->transform->SetScale(float2(showScale[0], showScale[1]));
				}
				POP_FONT;
			}

			POP_FONT; 
		}
		else
		{
			ImGui::Text("No Object Selected"); 
		}
	}

	ImGui::End();
	return true;
}
