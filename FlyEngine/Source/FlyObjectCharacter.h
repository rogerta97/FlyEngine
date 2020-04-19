#ifndef _FLY_OBJECT_CHARACTER_H_
#define _FLY_OBJECT_CHARACTER_H_

#include "FlyObject.h"

#include <string>

class Room;
class FlyObjectCharacter : public FlyObject
{
public: 
	FlyObjectCharacter(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT, Room* parentRoom = nullptr);
	FlyObjectCharacter();

	~FlyObjectCharacter();

};

#endif