#ifndef _CHANGE_SCENE_TOOL_H_
#define _CHANGE_SCENE_TOOL_H_

#include "Tool.h"

using namespace std;

class Room;
class FlyObject; 
class ChangeSceneTool : public Tool
{
public:
	ChangeSceneTool(FlyObject* _parentObject);
	~ChangeSceneTool();

	void Init();
	void Update();
	void CleanUp();

	void SetDestination(Room* dstRoom); 
	Room* GetDesination(); 

private:
	Room* destinationRoom; 
};

#endif 