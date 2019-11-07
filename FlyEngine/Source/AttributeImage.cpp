#include "AttributeImage.h"
#include "Quad.h"
#include "Globals.h"
#include "OpenGL.h"
#include "MyFileSystem.h"
#include "Texture.h"
#include "FlyObject.h"
#include "ImageImporter.h"

AttributeImage::AttributeImage(FlyObject* _parentObject = nullptr)
{
	attributeType = AT_null; 
	quadMesh = nullptr; 
	imageTexture = nullptr; 
	parentObject = _parentObject; 
}

AttributeImage::~AttributeImage()
{
}

void AttributeImage::Init()
{

}

void AttributeImage::Update()
{

}

void AttributeImage::Draw()
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

		glMatrixMode(GL_MODELVIEW);	
		glLoadMatrixf((GLfloat*)((parentObject->transform->CalculateViewMatrix()).Transposed() /** view_mat*/).v);
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

void AttributeImage::CleanUp()
{

}

bool AttributeImage::CreateImage(const char* texturePath)
{
	imageTexture = ImageImporter::getInstance()->LoadTexture(texturePath, true);

	imageWidth = imageTexture->GetWidth();
	imageHeight = imageTexture->GetHeight();

	quadMesh = new Quad(); 
	quadMesh->Create(imageWidth, imageHeight); 

	return true; 
}

Quad* AttributeImage::GetQuad() const
{
	return quadMesh; 
}

Texture* AttributeImage::GetTexture() const
{
	return imageTexture; 
}
