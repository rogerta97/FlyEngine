#ifndef _ACTION_CONDITION_HAS_ITEM_H_
#define _ACTION_CONDITION_HAS_ITEM_H_

#include <string>
#include "ActionCondition.h"

class FlyVariable;
class ActionConditionHasItem : public ActionCondition
{
public:
	ActionConditionHasItem();
	~ActionConditionHasItem();

	void CleanUp();
	void SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos);
	bool DrawUIItem(int itemPosition);
	bool PassTestCondition(); 

	std::string itemToCheckName;
	UID itemToCheckUID;
};

#endif // !_ACTION_CONDITION_HAS_ITEM_H_



