#include "DisplayImageAction.h"
#include "Quad.h"
#include "Globals.h"
#include "OpenGL.h"
#include "MyFileSystem.h"
#include "Texture.h"
#include "FlyObject.h"
#include "TinyFileDialog.h"
#include "ImageImporter.h"
#include "Gizmos.h"
#include "ResourceManager.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

#include "mmgr.h"

DisplayImageAction::DisplayImageAction(FlyObject* _parentObject = nullptr)
{
	occ_SceneEnter = false;
	occ_SceneLeave = false;
	occ_ObjectClicked = false;
	occ_blackboardValue = false;
	occ_continuous = false;
	occ_mouseOver = false;

	actionType = ACTION_DISPLAY_IMAGE; 
	quadMesh = nullptr; 
	imageTexture = nullptr; 
	parentObject = _parentObject; 
	isVisual = true; 
	fromAnimation = false; 
	drawIfSequential = false; 

	if (parentObject == nullptr)
	{
		ownPosition = float2::zero;
		isSlotIcon = true;
	}

	SetActionName("Display Image"); 
	SetToolDescription("This should be the description of the image"); 
}

DisplayImageAction::~DisplayImageAction()
{
}

void DisplayImageAction::Init()
{

}

void DisplayImageAction::Draw()
{
	if (!isVisible)
		return; 

	glEnableClientState(GL_VERTEX_ARRAY); 
	glBindBuffer(GL_ARRAY_BUFFER, quadMesh->verticesID); 
	glVertexPointer(3, GL_FLOAT, 0, NULL); 

	if (isSlotIcon)
	{
		float2 appliedArPos = ownPosition;

		float4x4 new_mat = float4x4::identity;

		new_mat = new_mat * new_mat.Scale(float3(0.60f, 0.60f, 0.60f));
		new_mat.SetTranslatePart(float3(appliedArPos.x, appliedArPos.y, 0));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)new_mat.Transposed().v);

		//float2 unAppliedArPos = parentObject->transform->GetPosition() / App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
		//parentObject->transform->SetPosition(unAppliedArPos);

	}
	else if (parentObject->transform != nullptr)
	{
		float2 appliedArPos = parentObject->transform->GetPosition(true);
		parentObject->transform->SetPosition(appliedArPos);

		glMatrixMode(GL_MODELVIEW);	
		glLoadMatrixf((GLfloat*)((parentObject->transform->CalculateViewMatrix()).Transposed()).v);

		float2 unAppliedArPos = parentObject->transform->GetPosition() / App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
		parentObject->transform->SetPosition(unAppliedArPos);
	}

	if (imageTexture != nullptr) {

		glEnable(GL_TEXTURE_2D); 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, quadMesh->uvsID);

		imageTexture->Bind(); 

		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//if (alphaDrawIntensity)
	//{
	//	glColor4f(1, 1, 1, alphaDrawIntensity); 	
	//}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh->indicesID);
	glDrawElements(GL_TRIANGLES, quadMesh->numIndices, GL_UNSIGNED_INT, NULL); 

	//if (alphaDrawIntensity)
	//{
	//	glColor4f(1, 1, 1, 1);
	//	alphaDrawIntensity = 0.0f;
	//}

	if(imageTexture) 
	{	
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		imageTexture->UnBind();
		glDisable(GL_TEXTURE_2D);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (imageTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	SetActionCompleted(true); 
}

void DisplayImageAction::PushAlphaDraw(float alphaIntensity)
{
	if(alphaIntensity != 0)
		alphaDrawIntensity = alphaIntensity;
}

void DisplayImageAction::DoAction()
{
	if (IsActionSequential() && holdingData == false)
	{
		DisplayImageAction* fixedDisplayImageAction = (DisplayImageAction*)parentObject->GetAction(ACTION_DISPLAY_IMAGE);

		if (fixedDisplayImageAction != nullptr)
		{
			fixedDisplayImageAction->SetTexture(this->GetTexture()); 
		}
		else
		{
			if(imageTexture == nullptr)
				CreateImage("None");
			else
				CreateImage(imageTexture->GetPath());

			drawIfSequential = true;
		}

		holdingData = true;
	}

	SetActionCompleted(true);
}

void DisplayImageAction::CleanUp()
{
	Action::CleanUp();

	if (quadMesh != nullptr)
	{
		quadMesh->CleanUp();
		delete quadMesh; 
		quadMesh = nullptr; 
	}

	if (imageTexture != nullptr)
	{

	}

	parentObject = nullptr; 
}

void DisplayImageAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Image Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Display Image:");
		ImGui::PopFont();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("UI_ImagePreview", ImVec2(ImGui::GetContentRegionAvailWidth(), 195));

		ImVec2 imageMaxSize = ImVec2(ImGui::GetContentRegionAvailWidth(), 135);
		ImVec2 uiImageDimensions = ImVec2(150, 150);
		Texture* imageTexture = GetTexture();
		ImTextureID selectedTextureID = 0;

		if (imageTexture != nullptr)
		{
			float aspectRatio = imageTexture->GetAspectRatio();

			if (imageTexture->IsVertical())
			{
				uiImageDimensions.y = imageMaxSize.y;
				uiImageDimensions.x = uiImageDimensions.x * aspectRatio;
			}
			else
			{
				uiImageDimensions.y = imageMaxSize.y;
				uiImageDimensions.x = uiImageDimensions.y * aspectRatio;

				if (uiImageDimensions.x > imageMaxSize.x)
				{
					float diff = uiImageDimensions.x - imageMaxSize.x;
					uiImageDimensions.x -= diff;
					uiImageDimensions.y = uiImageDimensions.x * aspectRatio;
				}
			}
			selectedTextureID = (ImTextureID)imageTexture->GetTextureID();
		}

		ImGui::Spacing();

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (uiImageDimensions.x / 2), imageMaxSize.y / 2 - (uiImageDimensions.y / 2) + 10));
		ImGui::Image(selectedTextureID, uiImageDimensions);

		ImGui::Spacing();

		static char searchButtonImageBuffer[256];

		ImGui::Spacing();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 70);


		if (imageTexture != nullptr)
		{
			strcpy(searchButtonImageBuffer, imageTexture->GetName().c_str()); 
		}

		ImGui::InputTextWithHint("", "Search Image...", searchButtonImageBuffer, IM_ARRAYSIZE(searchButtonImageBuffer));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("drag_resource"))
			{
				int* selectedResourceUID = (int*)payload->Data;
				Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

				if (resourceDropped->GetType() == RESOURCE_TEXTURE)
				{
					Texture* textureDropped = (Texture*)resourceDropped;
					SetTexture(textureDropped);
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		if (ImGui::Button("Search##SearchUIIMage"))
		{
			ImGui::OpenPopup("print_image_selection_popup"); 
		}

		Texture* selectedImage = (Texture*)ResourceManager::getInstance()->PrintImagesSelectionPopup(); 

		if (selectedImage != nullptr)
		{
			SetTexture(selectedImage); 
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		//static char buf[256] = "";

		//// Object Occurrence -----------------------
		//float aspect_ratio = GetTexture()->GetAspectRatio();
		//float previewQuadWidth = 150;
		//float previewQuadHeight = previewQuadWidth / aspect_ratio;

		//PUSH_FONT(App->moduleImGui->rudaRegularMid);
		//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		//int childHeight = previewQuadHeight + 20;

		//PUSH_FONT(App->moduleImGui->rudaBoldBig);
		//ImGui::Text("Image Settings:");
		//POP_FONT;

		//ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		//ImGui::Columns(2);
		//ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		//ImGui::Spacing();
		//ImGui::Image((ImTextureID)GetTexture()->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		//ImGui::NextColumn();

		//ImGui::Spacing();
		//ImGui::Text("Name: "); ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", GetTexture()->GetName().c_str());

		//ImGui::Text("Width: "); ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", GetTexture()->GetWidth());

		//ImGui::Text("Height: "); ImGui::SameLine();
		//ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", GetTexture()->GetHeigth());

		//Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		//if (ImGui::Button("Change Image"))
		//{
		//	char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
		//	const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

		//	if (path != NULL)
		//	{
		//		if (!ResourceManager::getInstance()->ExistResourcePath(path))
		//		{
		//			SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
		//			ResourceManager::getInstance()->AddResource(GetTexture(), GetTexture()->GetName());
		//		}
		//		else		
		//			SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
		//	
		//		strcpy(buf, path);
		//		flog("Player Opened %s", path);
		//	}
		//}
		//ImGui::PopStyleColor();
		//ImGui::EndChild();

		//POP_FONT;
	}
}

void DisplayImageAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection); 

	if (imageTexture != nullptr)
	{
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("ImageWidth")).c_str(), imageWidth);
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("ImageHeigth")).c_str(), imageHeight);

		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem(imageTexture->GetPath(), true).c_str());
	}
	else
	{
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("ImageWidth")).c_str(), 0);
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("ImageHeigth")).c_str(), 0);
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
	}
}

void DisplayImageAction::DrawActionOccurenceCheckboxes()
{
	//ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	//ImGui::Text("Action Happens On:");
	//ImGui::PopFont();

	//ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	//ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));

	//ImGui::SetCursorPos(ImVec2(5, 8)); 
	//ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
	//ImGui::SetCursorPos(ImVec2(5, 38)); 
	//ImGui::Checkbox("Scene Leave", &occ_SceneLeave);
	//ImGui::SetCursorPos(ImVec2(5, 68)); 
	//ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);

	//ImGui::Spacing(); 
	//ImGui::EndChild(); 

	//ImGui::PopFont();
	//ImGui::PopStyleColor(); 
}

bool DisplayImageAction::CreateImage(const char* texturePath)
{
	SetImageTextureByPath(texturePath);

	if (imageTexture != nullptr)
	{
		imageWidth = imageTexture->GetWidth();
		imageHeight = imageTexture->GetHeigth();
	}

	quadMesh = new Quad(); 
	quadMesh->Create(imageWidth, imageHeight); 
	holdingData = true; 

	return true; 
}

void DisplayImageAction::SetImageTextureByPath(const char* texturePath)
{
	if (texturePath != "None")
	{
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(texturePath, false);
		imageTexture = (Texture*)ResourceManager::getInstance()->GetResource(resourceName.c_str());
	}
	else
		imageTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");
}

bool DisplayImageAction::IsVertical()
{
	if(imageTexture == nullptr)
		return false;

	return imageTexture->IsVertical();
}

void DisplayImageAction::SetOwnPosition(float2 newPosition)
{
	if (parentObject != nullptr)
	{
		FLY_ERROR("Own Position to Display ImageAction can't be aplied to an object with a parent"); 
		return;
	}

	isSlotIcon = true; 
	ownPosition = newPosition; 
}

Quad* DisplayImageAction::GetQuad() const
{
	return quadMesh; 
}

void DisplayImageAction::SetQuad(Quad* newQuad)
{
	quadMesh = newQuad;
}

void DisplayImageAction::SetTexture(Texture* newTexture)
{
	// Addapt Quad 
	if(!isHolderInfo)
	{
		quadMesh->CleanUp();
		delete quadMesh; 

		quadMesh = new Quad(); 

		if(newTexture != nullptr)
			quadMesh->Create(newTexture->GetWidth(), newTexture->GetHeigth());
		else
			quadMesh->Create(100, 100);
	}

	// Set Texture 
	imageTexture = newTexture;
}

Texture* DisplayImageAction::GetTexture() const
{
	return imageTexture; 
}

void DisplayImageAction::SetWidth(float newHeight)
{
	imageWidth = newHeight; 
}

float DisplayImageAction::GetWidth() const
{
	return imageWidth;
}

void DisplayImageAction::SetHeigth(float newHeight)
{
	imageHeight = newHeight; 
}

float DisplayImageAction::GetHeigth() const
{
	return imageHeight;
}

void DisplayImageAction::SetVisible(bool newVisible)
{
	Action::SetVisible(newVisible); 
}
