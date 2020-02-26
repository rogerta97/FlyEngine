#include "DisplayAnimationAction.h"

DisplayAnimationAction::DisplayAnimationAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_ANIMATION;
	isVisual = false;
	parentObject = _parentObject; 

	SetActionName("Display Animation");
	SetToolDescription("This should be the description of the animation action");
}

DisplayAnimationAction::~DisplayAnimationAction()
{
}

void DisplayAnimationAction::Init()
{
}

void DisplayAnimationAction::Update()
{
}

void DisplayAnimationAction::CleanUp()
{
}
