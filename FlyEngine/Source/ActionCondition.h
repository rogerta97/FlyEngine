#ifndef _ACTION_CONDITION_H_
#define _ACTION_CONDITION_H_

#include <string>
#include "SaveAndLoad.h"

enum ActionConditionType
{
	AC_CHECK_VARIABLE, 
	AC_CHECK_OBJECT_INVENTORY, 
	AC_NONE, 
};

class FlyVariable;
class ActionCondition
{
public:
	ActionCondition();
	~ActionCondition();

	virtual void CleanUp();
	virtual void SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos);
	virtual void DrawUIItem(int itemPosition);

	ActionConditionType actionConditionType; 
};

#endif // !_ACTION_CONDITION_H_


