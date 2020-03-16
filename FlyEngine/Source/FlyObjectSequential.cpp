#include "FlyObjectSequential.h"
#include "Room.h"

FlyObjectSequential::FlyObjectSequential(std::string objectName, std::string description, FlyObjectType _flyObjectType, Room* parentRoom)
{
	flyObjectType = OBJECT_SEQUENTIAL; 
}

FlyObjectSequential::~FlyObjectSequential()
{
}

void FlyObjectSequential::DoObjectActions()
{
}
