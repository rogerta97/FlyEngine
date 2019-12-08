#ifndef _CHANGE_ROOM_TOOL_H_
#define _CHANGE_ROOM_TOOL_H_

#include "Action.h"

using namespace std;

class Room;
class FlyObject; 
class ChangeRoomAction : public Action
{
public:
	ChangeRoomAction(FlyObject* _parentObject);
	~ChangeRoomAction();

	void Init();
	void Update();
	void CleanUp();

	void DoAction(); 

	void DrawActionOccurenceCheckboxes(); 

	void SetDestination(Room* dstRoom); 
	Room* GetDesination(); 

private:
	Room* destinationRoom; 
};

#endif 