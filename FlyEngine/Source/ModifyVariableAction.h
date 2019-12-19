#ifndef _MODIFY_VARIABLE_ACTION_H_
#define _MODIFY_VARIABLE_ACTION_H_

#include "Action.h"

using namespace std;

class FlyObject;
class Quad;
class Texture;

class ModifyVariableAction : public Action
{
public:
	ModifyVariableAction(FlyObject* _parentObject);
	~ModifyVariableAction();

};

#endif 