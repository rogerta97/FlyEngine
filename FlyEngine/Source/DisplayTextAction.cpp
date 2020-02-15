#include "DisplayTextAction.h"

DisplayTextAction::DisplayTextAction(FlyObject* _parentObject)
{
	actionType = ACTION_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;

	currentFont = nullptr;

	SetActionName("Display Text");
	SetToolDescription("This should be the description of display text");
}

DisplayTextAction::~DisplayTextAction()
{

}