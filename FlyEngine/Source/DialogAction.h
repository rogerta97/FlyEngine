#ifndef _DIALOG_ACTION_H_
#define _DIALOG_ACTION_H_

#include "Action.h"

using namespace std;

class DialogAction : public Action
{
public:
	DialogAction(FlyObject* _parentObject);
	~DialogAction();

	void DrawUISettings();
};

#endif 
