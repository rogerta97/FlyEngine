#ifndef _FOLLOW_PATH_ACTION_H_
#define _FOLLOW_PATH_ACTION_H_

#include "Action.h"

class FollowPathAction : public Action
{
public:
	FollowPathAction(FlyObject* _parentObject);
	~FollowPathAction();
};

#endif 