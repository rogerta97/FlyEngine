#include "DisplayTextAction.h"

DisplayTextAction::DisplayTextAction(FlyObject* _parentObject)
{
	actionType = ACTION_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;

	SetActionName("Emit Sound");
	SetToolDescription("This should be the description of the emit sound");
}

DisplayTextAction::~DisplayTextAction()
{

}