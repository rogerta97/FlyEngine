#include "FlyObject.h"
#include "RandomNumberGenerator.h"
#include "AttributeImage.h"
#include "FileSystem.h"

#include <string>

FlyObject::FlyObject(std::string _name)
{
	name = _name; 
	id = RandomNumberGenerator::GenerateUID(); 
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

AttributeImage* FlyObject::AddAttributeImage(const char* imageTexturePath)
{
	AttributeImage* newAtrImage = new AttributeImage(); 

	std::string path = FileSystem::getInstance()->solutionDirectory; 
	path += "EngineResources / Images / PlaceHolder.png";
	newAtrImage->CreateImage(path.c_str());

	attributeList.push_back(newAtrImage); 

	return newAtrImage; 
}
