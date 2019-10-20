#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <string>

#include "Globals.h"

class FlyObject
{
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	std::string GetName() const; 

private: 
	std::string name; 
	UID id; 
};

#endif

