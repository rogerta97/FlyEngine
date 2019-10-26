#include "AttributeImage.h"
#include "Quad.h"
#include "Globals.h"
#include "OpenGL.h"

AttributeImage::AttributeImage()
{
	attributeType = AT_null; 
	quadMesh = nullptr; 
	imageTexture = nullptr; 
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh->indicesID);
	glDrawElements(GL_TRIANGLES, quadMesh->numIndices, GL_UNSIGNED_INT, NULL); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void AttributeImage::CleanUp()
{

}

bool AttributeImage::CreateImage(const char* texturePath)
{
	//imageTexture = new Texture();
	//get width and height 

	// -----

	imageWidth = imageHeight = 1; // SUPOSE 

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
