#ifndef _FOLLOW_PATH_ACTION_H_
#define _FOLLOW_PATH_ACTION_H_

#include "Action.h"
#include "Math/float3.h"
#include <list>

struct PathStep
{
	float3 targetPosition; 
	float targetTime; 
};

class FollowPathAction : public Action
{
public:
	FollowPathAction(FlyObject* _parentObject);
	~FollowPathAction();

	void DrawPath(); 
	void DrawUISettings();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

private: 
	std::list<PathStep*>* pathSteps; 
};

#endif 