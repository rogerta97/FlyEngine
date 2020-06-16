#include "ActionCondition.h"

#include "mmgr.h"

ActionCondition::ActionCondition()
{
	actionConditionType = AC_NONE;
}

ActionCondition::~ActionCondition()
{

}

void ActionCondition::CleanUp()
{

}

void ActionCondition::SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos)
{

}

bool ActionCondition::DrawUIItem(int itemPosition)
{
	return true;
}

bool ActionCondition::PassTestCondition()
{
	return false;
}
