#include "DialogAction.h"

DialogAction::DialogAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOG;
	isVisual = false;
	acceptSequencial = true;

	SetActionName("Dialog");
	SetToolDescription("This should be the description of the dialog");
}

DialogAction::~DialogAction()
{
}
