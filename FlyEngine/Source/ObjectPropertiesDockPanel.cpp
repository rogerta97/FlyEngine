#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "imgui.h"
#include "ModuleRoomManager.h"
#include "ResourceManager.h"
#include "ObjectCreator.h"
#include "Texture.h"
#include "FlyObject.h"
#include "Room.h"
#include "mmgr.h"

ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_PROPERTIES;

	objectCreator = new ObjectCreator(); 
}

ObjectPropertiesDockPanel::~ObjectPropertiesDockPanel()
{
	delete objectCreator; 
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

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45, ImGui::GetCursorPosY() - 35));

			ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
			ImGui::Text("%s", selectedObject->GetName().c_str());
			ImGui::PopFont();

			ImGui::Spacing();
			ImGui::Spacing();

			DrawObjectPlacementCH(selectedObject);

			ImGui::Spacing();
			ImGui::Separator(); 
			ImGui::Spacing();

			DrawObjectTools(selectedObject);
		}
		else
		{
			ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.2f, 1.0f), "No Object Selected");
		}
	}

	ImGui::End();
	return true;
}

void ObjectPropertiesDockPanel::DrawObjectTools(FlyObject* selectedObject)
{

	// Draw Objects List
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	for(auto& currentTool: selectedObject->GetToolsList()){
		objectCreator->DrawToolSelectable(App->moduleManager->GetToolNameDescription(currentTool->toolName));
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	ImGui::Separator(); 

	PUSH_FONT(App->moduleImGui->rudaBoldMid);
	for (auto& currentTool : selectedObject->GetToolsList())
	{
		if (ImGui::CollapsingHeader(currentTool->GetToolName().c_str()))
		{
			objectCreator->DrawToolProperties(currentTool->GetToolType());
		}

	}
	POP_FONT;
}

void ObjectPropertiesDockPanel::DrawObjectPlacementCH(FlyObject* selectedObject)
{

	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::CollapsingHeader("Object Placement"))
	{
		ImGui::Spacing(); 

		float showPosition[2] = { selectedObject->transform->GetPosition().x, selectedObject->transform->GetPosition().y };
		float showRotation[2] = { selectedObject->transform->GetRotation().x, selectedObject->transform->GetRotation().y };
		float showScale[2] = { selectedObject->transform->GetScale().x, selectedObject->transform->GetScale().y };

		PUSH_FONT(App->moduleImGui->rudaRegularMid);
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
