#ifndef _MODIFY_VARIABLE_ACTION_H_
#define _MODIFY_VARIABLE_ACTION_H_

#include "Action.h"
#include <list>

using namespace std;

class FlyObject;
class Quad;
class Texture;
class FlyVariable;

enum VariableOperatorType
{
	VarEffect_ADD,
	VarEffect_SUBSTRACT,
	VarEffect_SET_NUMBER,

	VarEffect_TOGGLE, 
	VarEffect_SET_TOGGLE, 

	VarEffect_None
};

class VariableEffect
{
public: 

	VariableEffect(); 
	~VariableEffect(); 

	VariableOperatorType variableOperatorType;

	int incIntegerValue; 
	bool nextToggleValue; 
};

class ModifyVariableEffect
{
public: 

	ModifyVariableEffect(); 
	~ModifyVariableEffect(); 

	void CleanUp(); 
	void SaveEffect(JSON_Object* jsonObject, string serializeObjectString, int pos);

	void ApplyEffect(); 
	void AttachToVariable(FlyVariable* _targetVariable); 

	FlyVariable* targetVariable; 
	VariableEffect* variableEffect; 
};

class ModifyVariableAction : public Action
{
public:
	ModifyVariableAction(FlyObject* _parentObject);
	~ModifyVariableAction();

	// Virtuals -------
	void CleanUp(); 
	void DoAction(); 
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString);

	// Draw -----------
	void DrawEffectVariablesUI();
	void DrawEffectItem(ModifyVariableEffect*& currentVariable, int pos);

	// Utility --------
	ModifyVariableEffect* AddEmptyEffect();
	int CountEffects();
	list<ModifyVariableEffect*>& GetEffectVariablesList();
	int GetOperatorTextureIDFromType(VariableOperatorType effectType); 

private: 
	list<ModifyVariableEffect*> variablesEffectList;
};

#endif 