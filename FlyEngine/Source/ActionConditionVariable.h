#ifndef _ACTION_CONDITION_VARIABLE_H_
#define _ACTION_CONDITION_VARIABLE_H_

#include <string>
#include "ActionCondition.h"

enum ActionConditionOperator
{
	AC_EQUALS_TO,
	AC_BIGGER_THAN,
	AC_LESS_THAN,
	AC_DIFFERENT_THAN,
	AC_None
};

class FlyVariable;
class ActionConditionVariable : public ActionCondition
{
public:
	ActionConditionVariable();
	~ActionConditionVariable();

	void CleanUp();
	void SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos);
	void DrawUIItem(int itemPosition);

	FlyVariable* targetVariable;
	ActionConditionOperator actionConditionOperator;

	int targetValueInteger;
	bool targetValueBoolean;
};

#endif // !_ACTION_CONDITION_H_



