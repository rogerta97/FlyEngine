#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include "Globals.h"
#include "RandomNumberGenerator.h"

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
};


#endif // !_RESOURCES_H_
