#include "FollowPathAction.h"

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;

	SetActionName("Follow Path");
	SetToolDescription("This should be the description of follow path action");
}

FollowPathAction::~FollowPathAction()
{

}
