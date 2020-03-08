#ifndef _FOLLOW_PATH_ACTION_H_
#define _FOLLOW_PATH_ACTION_H_

#include "Action.h"
#include "Math/float2.h"
#include <list>

struct PathStep
{
	float2 targetPosition; 
	float targetTime; 

	void Save(JSON_Object* jsonObject, string serializeObjectString);
	void DrawStepGUI(int stepPos, float selectableHeigth = 65);
};

enum PathMode
{
	PATH_ONE_TIME, 
	PATH_AMOUNT_TIMES,
	PATH_LOOP_TELEPORT,
	PATH_LOOP_CYCLE,
};

class FollowPathAction : public Action
{
public:
	FollowPathAction(FlyObject* _parentObject);
	~FollowPathAction();

	void DrawPath(); 
	void DrawUISettings();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

	PathMode GetPathMode();
	void SetPathMode(PathMode newPathMode); 

private: 
	std::list<PathStep*>* pathSteps; 
	PathMode pathMode; 
};

#endif 