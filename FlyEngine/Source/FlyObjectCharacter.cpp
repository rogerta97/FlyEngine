#include "FlyObjectCharacter.h"
#include "Room.h"

FlyObjectCharacter::FlyObjectCharacter(std::string objectName, std::string description, FlyObjectType _flyObjectType, Room* parentRoom)
{
	flyObjectType = OBJECT_SEQUENTIAL; 
}

FlyObjectCharacter::FlyObjectCharacter()
{
}

FlyObjectCharacter::~FlyObjectCharacter()
{
}

