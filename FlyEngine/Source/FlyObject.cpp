#include "FlyObject.h"
#include "RandomNumberGenerator.h"
#include "ImageTool.h"
#include "MyFileSystem.h"

#include <string>

FlyObject::FlyObject(std::string _name)
{
	name = _name; 
	id = RandomNumberGenerator::GenerateUID(); 
	isSelected = false; 

	transform = new Transform(); 
}

FlyObject::~FlyObject()
{

}

void FlyObject::Update()
{
}

void FlyObject::Draw()
{
	for (auto& it : attributeList) {
		(it)->Draw(); 
	}
}

std::string FlyObject::GetName() const
{
	return name;
}

ImageTool* FlyObject::AddAttributeImage(const char* imageTexturePath)
{
	ImageTool* newAtrImage = new ImageTool(this); 
	newAtrImage->CreateImage(imageTexturePath);

	attributeList.push_back(newAtrImage); 
	return newAtrImage; 
}
