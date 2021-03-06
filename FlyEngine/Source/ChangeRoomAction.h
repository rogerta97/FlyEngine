#ifndef _CHANGE_ROOM_TOOL_H_
#define _CHANGE_ROOM_TOOL_H_

#define MAX_ROOMS 256

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
	void Update(float dt);
	void CleanUp();

	void DrawSelectDestinationCombo(); 
	void DrawUISettings(); 
	void DrawUISettingsInButton(); 
	void DrawActionOccurenceCheckboxes(); 
	bool PassAllOccurrenceConditions();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);
	void DoAction(); 

	void SetDestination(Room* dstRoom); 
	Room* GetDesination(); 

private:
	Room* destinationRoom; 
	bool showClickWithItems = false; 
	bool showVariableConditions = false; 
};

#endif 