#include "DisplayImageAction.h"
#include "Quad.h"
#include "Globals.h"
#include "OpenGL.h"
#include "MyFileSystem.h"
#include "Texture.h"
#include "FlyObject.h"
#include "ImageImporter.h"
#include "Gizmos.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

#include "mmgr.h"

DisplayImageAction::DisplayImageAction(FlyObject* _parentObject = nullptr)
{
	toolType = AT_IMAGE; 
	quadMesh = nullptr; 
	imageTexture = nullptr; 
	parentObject = _parentObject; 
	isVisual = true; 

	SetActionName("Display Image"); 
	SetToolDescription("This should be the description of the image"); 
}

DisplayImageAction::~DisplayImageAction()
{
}

void DisplayImageAction::Init()
{

}

void DisplayImageAction::Update()
{

}

void DisplayImageAction::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY); 
	glBindBuffer(GL_ARRAY_BUFFER, quadMesh->verticesID); 
	glVertexPointer(3, GL_FLOAT, 0, NULL); 

	if (parentObject->transform != nullptr)
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh->indicesID);
	glDrawElements(GL_TRIANGLES, quadMesh->numIndices, GL_UNSIGNED_INT, NULL); 

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
}

void DisplayImageAction::CleanUp()
{
	quadMesh->CleanUp();
	delete quadMesh; 

	if (imageTexture != nullptr)
	{
		imageTexture->CleanUp();
		delete imageTexture; 
	}

	parentObject = nullptr; 
}

void DisplayImageAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString)
{
	string toolsSerializeSection = serializeObjectString + string("Tools.DisplayImage.");

	if (imageTexture != nullptr)
	{
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Image_Width")).c_str(), imageWidth);
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Image_Heigth")).c_str(), imageHeight);

		string relativePath = imageTexture->GetPath();
		MyFileSystem::getInstance()->GetRelativeDirectory(relativePath);

		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Image_Path")).c_str(), relativePath.c_str());
	}
}

void DisplayImageAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));

	ImGui::SetCursorPos(ImVec2(5, 8)); 
	ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
	ImGui::SetCursorPos(ImVec2(5, 38)); 
	ImGui::Checkbox("Scene Leave", &occ_SceneLeave);
	ImGui::SetCursorPos(ImVec2(5, 68)); 
	ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);

	ImGui::Spacing(); 
	ImGui::EndChild(); 

	ImGui::PopFont();
	ImGui::PopStyleColor(); 
}

bool DisplayImageAction::CreateImage(const char* texturePath)
{
	imageTexture = ImageImporter::getInstance()->LoadTexture(texturePath, false);

	if (imageTexture != nullptr)
	{
		imageWidth = imageTexture->GetWidth();
		imageHeight = imageTexture->GetHeigth();
	}

	quadMesh = new Quad(); 
	quadMesh->Create(imageWidth, imageHeight); 

	return true; 
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
	quadMesh->UnloadFromMemory();
	delete quadMesh; 

	quadMesh = new Quad(); 
	quadMesh->Create(newTexture->GetWidth(), newTexture->GetHeigth());

	// Set Texture 
	imageTexture = newTexture;
}

Texture* DisplayImageAction::GetTexture() const
{
	return imageTexture; 
}
