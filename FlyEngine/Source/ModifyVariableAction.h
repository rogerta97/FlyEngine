#ifndef _MODIFY_VARIABLE_ACTION_H_
#define _MODIFY_VARIABLE_ACTION_H_

#include "Action.h"
#include <list>

using namespace std;

class FlyObject;
class Quad;
class Texture;
class FlyVariable;

enum VariableEffect
{
	VarEffect_ADD,
	VarEffect_SUBSTRACT,
	VarEffect_SET_NUMBER,

	VarEffect_TOGGLE, 
	VarEffect_SET_TOGGLE, 

	VarEffect_None
};

struct ModifyVariableEffect
{
	FlyVariable* targetVariable; 
	VariableEffect variableEffect; 
};

class ModifyVariableAction : public Action
{
public:
	ModifyVariableAction(FlyObject* _parentObject);
	~ModifyVariableAction();

	void DrawEffectVariablesUI();

	void DrawEffectItem(ModifyVariableEffect*& currentVariable, int pos);

	ModifyVariableEffect* AddEmptyEffect();
	int CountEffects();
	list<ModifyVariableEffect*>& GetEffectVariablesList();
	int GetOperatorTextureIDFromType(VariableEffect effectType); 

private: 
	list<ModifyVariableEffect*> variablesEffectList;
};

#endif 