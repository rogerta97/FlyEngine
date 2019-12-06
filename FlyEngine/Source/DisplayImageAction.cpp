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

	SetActionName("Image"); 
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
		//float4x4 view_mat; 
		//GLfloat matrix[16];
		//glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		//view_mat.Set((float*)matrix);

		float2 appliedArPos = parentObject->transform->GetPosition(true);
		parentObject->transform->SetPosition(appliedArPos);

		glMatrixMode(GL_MODELVIEW);	
		glLoadMatrixf((GLfloat*)((parentObject->transform->CalculateViewMatrix()).Transposed() /** view_mat*/).v);

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
