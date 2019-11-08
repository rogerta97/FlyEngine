#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include "Globals.h"
#include "RandomNumberGenerator.h"
#include <string>

enum ResourceType
{
	RESOURCE_TEXTURE,
	RESOURCE_null,
};

class Resource 
{
public:
	Resource(ResourceType resourceType)
	{
		resourceID = RandomNumberGenerator::getInstance()->GenerateUID(); 
		this->resourceType = resourceType; 
	}
	~Resource()
	{

	}

	void SetName(std::string newName)
	{
		resourceName = newName;
	}
	std::string GetName() const
	{
		return resourceName;
	}

	void SetType(ResourceType newType)
	{
		resourceType = newType;
	}
	ResourceType GetType() const 
	{
		return resourceType;
	}

	void SetUID(UID newUID)
	{
		resourceID = newUID;
	}
	UID GetUID() const
	{
		return resourceID;
	}

private: 
	ResourceType resourceType = RESOURCE_null; 
	UID resourceID = -1;
	std::string resourceName = ""; 
};


#endif // !_RESOURCES_H_
