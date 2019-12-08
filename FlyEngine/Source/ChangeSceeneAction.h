#ifndef _CHANGE_SCENE_TOOL_H_
#define _CHANGE_SCENE_TOOL_H_

#include "Action.h"

using namespace std;

class Room;
class FlyObject; 
class ChangeSceneAction : public Action
{
public:
	ChangeSceneAction(FlyObject* _parentObject);
	~ChangeSceneAction();

	void Init();
	void Update();
	void CleanUp();

	void DrawActionOccurenceCheckboxes(); 

	void SetDestination(Room* dstRoom); 
	Room* GetDesination(); 

private:
	Room* destinationRoom; 
};

#endif 