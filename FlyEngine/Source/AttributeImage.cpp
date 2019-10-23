#include "AttributeImage.h"
#include "Quad.h"

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
}

void AttributeImage::CleanUp()
{

}

bool AttributeImage::SetImage(const char* texturePath)
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
