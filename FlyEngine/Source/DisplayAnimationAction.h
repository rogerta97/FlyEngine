#ifndef _DISPLAY_ANIMATION_ACTION_H_
#define _DISPLAY_ANIMATION_ACTION_H_

#define MAX_ROOMS 256

#include "Action.h"

using namespace std;

class Room;
class FlyObject;
class DisplayAnimationAction : public Action
{
public:
	DisplayAnimationAction(FlyObject* _parentObject);
	~DisplayAnimationAction();

	void Init();
	void Update();
	void CleanUp();

private:
	
};

#endif 