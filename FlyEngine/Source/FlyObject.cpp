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
	for (auto& it : toolsList) {
		(it)->Draw(); 
	}
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

ImageTool* FlyObject::AddImageTool(const char* imageTexturePath)
{
	ImageTool* newAtrImage = new ImageTool(this); 
	newAtrImage->CreateImage(imageTexturePath);

	toolsList.push_back(newAtrImage); 
	return newAtrImage; 
}

std::list<Tool*> FlyObject::GetToolsList() const
{
	return toolsList;
}
