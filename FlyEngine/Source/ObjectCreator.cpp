#include "ObjectCreator.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "FlyObject.h"

#include "imgui.h"

ObjectCreator::ObjectCreator()
{
	temporalObject = new FlyObject("ToCopy"); 
}

ObjectCreator::~ObjectCreator()
{
	delete temporalObject; 
}

void ObjectCreator::Draw()
{
	// Name Bar --------
	ImGui::Spacing();

	DrawObjectNameBar(); 

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	DrawToolsList();

	DrawAddAndDeleteButtons();
	DrawPopups(); 

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 80.0f);

	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
	if (ImGui::Button("Create", ImVec2(80, 30)))
	{
		//FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);

		//if (containsAttributeImage)
			//newObject->AddAttributeImage("Path");

		ImGui::CloseCurrentPopup();
	}

	ImGui::PopFont();
}

void ObjectCreator::DrawObjectNameBar()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
	static char newObjectName[256] = "";
	ImGui::InputText("Object Name", newObjectName, 256 * sizeof(char));
	ImGui::PopFont();
}

void ObjectCreator::DrawToolsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldFont);
	ImGui::Text("Object Tools: ");
	ImGui::PopFont();

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200), true);

	ImGui::Selectable("Example");
	ImGui::Text("Hello");

	ImGui::EndChild();
}

void ObjectCreator::DrawAddAndDeleteButtons()
{
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
	{
		ImGui::OpenPopup("plusIconClicked");
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
	{

	}
}

void ObjectCreator::DrawPopups()
{
	if (ImGui::BeginPopup("plusIconClicked"))
	{
		// Search Bar ---------------
		ImGui::InputText("##SearchTool", searchToolBuffer, IM_ARRAYSIZE(searchToolBuffer));
		ImGui::SameLine();

		Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		if (ImGui::ImageButton((ImTextureID)searchIcon->GetTextureID(), ImVec2(18, 18))) {

		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Tools Dictonary ----------
		for (int i = 0; i < App->moduleManager->GetToolsAmount(); i++)
		{
			ToolSelectableInfo newToolInfo = App->moduleManager->GetToolNameDescription(i);
			DrawToolSelectable(newToolInfo); 
		}

		ImGui::EndPopup();
	}
}

void ObjectCreator::OnNewToolButtonClicked()
{
}

void ObjectCreator::DrawToolSelectable(ToolSelectableInfo newToolInfo)
{
	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
	if (ImGui::Selectable(newToolInfo.toolName.c_str(), true, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {

		switch (newToolInfo.toolType)
		{
		case AT_IMAGE:
			//Add
			break;

		case AT_null:
			break;
		}
	}

	ImGui::PopFont();

	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 18);
	ImGui::PushFont(App->moduleImGui->rudaCommentFont);
	ImGui::TextWrapped(newToolInfo.toolDescription.c_str());
	ImGui::PopFont();
}
