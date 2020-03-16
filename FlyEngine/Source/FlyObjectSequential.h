#ifndef _FLY_OBJECT_SEQUENTIAL_H_
#define _FLY_OBJECT_SEQUENTIAL_H_

#include "FlyObject.h"

#include <string>

class Room;
class FlyObjectSequential : public FlyObject
{
public: 
	FlyObjectSequential(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT, Room* parentRoom = nullptr);
	FlyObjectSequential();
	~FlyObjectSequential();

	void DoObjectActions();
};

#endif