#include "ObjectCreator.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "TinyFileDialog.h"
#include "FlyObject.h"

#include "imgui.h"

ObjectCreator::ObjectCreator()
{

}

ObjectCreator::~ObjectCreator()
{

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
	DrawSelectedToolProperties(); 

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

		CleanBooleans(); 
		ImGui::CloseCurrentPopup();
	}

	ImGui::PopFont();
}

void ObjectCreator::DrawObjectNameBar()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
	static char newObjectName[256] = "";
	ImGui::InputTextWithHint("Object Name", "Name...", newObjectName, 256 * sizeof(char));
	ImGui::PopFont();
}

void ObjectCreator::DrawToolsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldFont);
	ImGui::Text("Add Object Tools: ");
	ImGui::PopFont();

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200), true);

	if (showImageTool) 
	{
		ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription("Image");

		if (DrawToolSelectable(toolInfo)) selectedInList = AT_IMAGE;
		
	}

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

void ObjectCreator::DrawSelectedToolProperties()
{
	switch (selectedInList)
	{
	case AT_IMAGE:

		ImGui::Spacing(); 
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushFont(App->moduleImGui->rudaBoldFont);
		if(ImGui::CollapsingHeader("Image Tool Adjustments:")) 
		{
			static char buf[256] = ""; 

			ImGui::Spacing();
			ImGui::PushFont(App->moduleImGui->rudaRegularFont);
			ImGui::InputTextWithHint("", "C:/",buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopFont(); 

			ImGui::PushFont(App->moduleImGui->rudaBlackFont);
			ImGui::SameLine(); 

			if (ImGui::Button("Browse"))
			{
				char const* lFilterPatterns[2] = { "*.png", "*.jpg" };
				const char* path = tinyfd_openFileDialog("Load Image...", NULL, 1, lFilterPatterns, NULL, 0);

				if (path != NULL) 
				{	
					if (!ResourceManager::getInstance()->ExistResourcePath(path)) 
					{
						previewImageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
						ResourceManager::getInstance()->AddResource(previewImageTexture, previewImageTexture->GetName());
					}
					else
					{
						previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path); 
					}

					strcpy(buf, path); 
					FLY_LOG("Player Opened %s", path);
				}
			}
			POP_FONT;

			if (previewImageTexture == nullptr)			
				previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageNull");
			else	
				previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource(previewImageTexture->GetName());

			float aspect_ratio = previewImageTexture->GetTextureAspectRatio();
			float previewQuadWidth = 150;
			float previewQuadHeight = previewQuadWidth / aspect_ratio;
	
			PUSH_FONT(App->moduleImGui->rudaRegularFont);
			ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18), true);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, previewQuadWidth + 10);

			ImGui::Image((ImTextureID)previewImageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

			ImGui::NextColumn(); 

			ImGui::Text("Name: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", previewImageTexture->GetName().c_str());

			ImGui::Text("Width: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetWidth());

			ImGui::Text("Height: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetHeight());

			ImGui::EndChild();
			POP_FONT; 
		}

		ImGui::PopFont(); 

		break;
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

void ObjectCreator::CleanBooleans()
{
	showImageTool = false; 
}

void ObjectCreator::OnNewToolButtonClicked()
{

}

bool ObjectCreator::DrawToolSelectable(ToolSelectableInfo newToolInfo)
{
	bool clicked = false; 
	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
	if (ImGui::Selectable(newToolInfo.toolName.c_str(), true, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {

		clicked = true; 
		switch (newToolInfo.toolType)
		{
		case AT_IMAGE:
			showImageTool = true; 
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

	return clicked; 

}
