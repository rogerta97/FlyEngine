#include "FlyObject.h"
#include "RandomNumberGenerator.h"
#include "ModuleImGui.h"
#include "ImageTool.h"
#include "Texture.h"
#include "TinyFileDialog.h"
#include "Quad.h"
#include "ImageImporter.h"
#include "GameViewportDockPanel.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "MyFileSystem.h"
#include "Gizmos.h"
#include "mmgr.h"


#include <string>

FlyObject::FlyObject(std::string _name)
{
	name = _name; 
	id = RandomNumberGenerator::GenerateUID(); 
	isSelected = false; 

	transform = new Transform();

	objectBorderBox = new BoundingBox(this); 
	objectBorderBox->showCornerDots = false; 

	gizmos = new Gizmos(this);
}

FlyObject::~FlyObject()
{

}

void FlyObject::Update()
{
	gizmos->Update(); 
}

void FlyObject::Draw()
{
	for (auto& it : toolsList) {
		(it)->Draw(); 
	}

	if(isSelected)
		gizmos->Draw();
}

void FlyObject::CleanUp()
{
	for (auto& it : toolsList)
	{
		(it)->CleanUp(); 
	}

	toolsList.clear(); 
	delete transform; 
}

std::string FlyObject::GetName() const
{
	return name;
}

void FlyObject::SetName(std::string newName)
{
	name = newName; 
}

void FlyObject::Move()
{

}

void FlyObject::Rotate()
{
}

void FlyObject::Scale()
{
}

float2 FlyObject::GetObjectVisualDimensions()
{
	float2 objectVisualSize; 

	// For now we will just take into account ImageTool, if you are deleting this you advanced really. 
	ImageTool* imageTool = (ImageTool*)GetTool(AT_IMAGE); 
	if (imageTool != nullptr)
	{
		objectVisualSize = float2(imageTool->GetQuad()->quadWidth, imageTool->GetQuad()->quadHeight); 
	}

	return objectVisualSize; 
}

ImageTool* FlyObject::AddImageTool(const char* imageTexturePath)
{
	if (GetTool(AT_IMAGE) == nullptr)
	{
		ImageTool* newAtrImage = new ImageTool(this);
		newAtrImage->CreateImage(imageTexturePath);

		toolsList.push_back(newAtrImage);

		// Addapt Gizmo Rect to new Image
		gizmos->FitBoxToObject();

		return newAtrImage;
	}

	return (ImageTool*)GetTool(AT_IMAGE); 	
}

Tool* FlyObject::GetTool(std::string toolName)
{
	for (auto& currentTool : toolsList)
	{
		if (currentTool->GetToolName() == toolName)
			return currentTool;
	}

	return nullptr; 
}

Tool* FlyObject::GetTool(ToolType toolType)
{
	for (auto& currentTool : toolsList)
	{
		if (currentTool->GetToolType() == toolType)
			return currentTool;
	}

	return nullptr;
}

std::list<Tool*> FlyObject::GetToolsList() const
{
	return toolsList;
}

void FlyObject::DeleteTool(std::string toolNameToDelete)
{
	for (auto currentTool = toolsList.begin(); currentTool != toolsList.end(); currentTool++)
	{
		if ((*currentTool)->GetToolName() == toolNameToDelete)
		{
			(*currentTool)->CleanUp();
			delete (*currentTool);
			currentTool = toolsList.erase(currentTool); 

			if (toolsList.size() == 0)
				return;
		}
	}
}

void FlyObject::DrawImageToolSettings()
{
	ImageTool* imageTool = (ImageTool*)GetTool("Image"); 
	Texture* imageTexture = imageTool->GetTexture(); 

	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";
		if (imageTexture == nullptr)
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageNull");
		else
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource(imageTool->GetTexture()->GetName());

		float aspect_ratio = imageTexture->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)imageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", imageTexture->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetHeight());

		Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)searchTexture->GetTextureID(), ImVec2(24, 24)))
		{
			char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

			if (path != NULL)
			{
				if (!ResourceManager::getInstance()->ExistResourcePath(path))
				{
					imageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(imageTexture, imageTexture->GetName());
				}
				else
				{
					imageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
				}

				strcpy(buf, path);
				FLY_LOG("Player Opened %s", path);
			}

		}

		ImGui::PopStyleColor(2);
		ImGui::EndChild();
		ImGui::PopFont();
	}
}

//void FlyObject::/*DrawToolsList*/()
//{
//	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
//	ImGui::Text("Add Object Actions: ");
//	ImGui::PopFont();
//
//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
//
//	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));
//
//	if (showImageTool)
//	{
//		ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription("Image");
//		if (DrawToolSelectable(toolInfo)) selectedInList = AT_IMAGE;
//	}
//
//	ImGui::EndChild();
//
//	ImGui::PopStyleVar();
//	ImGui::PopStyleColor();
//}
