#include "DialogAction.h"

#include "mmgr.h"

DialogAction::DialogAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOG;
	isVisual = false;

	SetActionName("Dialog");
	SetToolDescription("This should be the description of the dialog");
}

DialogAction::~DialogAction()
{
}
