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
	VariableOperatorType variableOperatorType;

	int incIntegerValue;
	bool nextToggleValue;
};

class ModifyVariableAction : public Action
{
public:
	ModifyVariableAction(FlyObject* _parentObject);
	~ModifyVariableAction();

	// Virtuals -------
	void CleanUp(); 
	void DoAction(); 
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);

	void DrawUISettings();
	void DrawActionOccurenceCheckboxes();

	void DrawUISettingsInButton();

	// Draw -----------
	void DrawEffectVariablesUI();
	void DrawEffectItem(ModifyVariableEffect*& currentVariable, int pos);

	// Utility --------
	ModifyVariableEffect* AddEmptyEffect();
	void AddEffect(ModifyVariableEffect* newEffect); 
	int CountEffects();
	int GetOperatorTextureIDFromType(VariableOperatorType effectType); 

private: 
	list<ModifyVariableEffect*> variablesEffectList;
	bool showVariableConditions = false; 
};

#endif 