#include "FlyObject.h"
#include "RandomNumberGenerator.h"

FlyObject::FlyObject(std::string _name)
{
	name = _name; 
	id = RandomNumberGenerator::GenerateUID(); 
}

FlyObject::~FlyObject()
{

}

std::string FlyObject::GetName() const
{
	return name;
}

void FlyObject::AddAttribute(ObjectAttribute* newAttribute)
{
	
}
