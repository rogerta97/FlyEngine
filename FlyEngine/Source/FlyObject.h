#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <string>
#include <list>

#include "Globals.h"

class ObjectAttribute; 
class FlyObject
{
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	std::string GetName() const;

	void AddAttribute(ObjectAttribute* newAttribute); 

private: 
	std::list<ObjectAttribute*> attributeList; 
	std::string name; 
	UID id; 
};

#endif

