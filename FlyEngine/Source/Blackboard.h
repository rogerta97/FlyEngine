#ifndef _BLACKBOARD_H_
#define _BLACKBOARD_H_

#include <list>
#include <string>
#include "Globals.h"

class FlyVariable; 
class ModifyVariableEffect; 
class Blackboard
{

public: 
	Blackboard(); 
	~Blackboard(); 

	void ModifyIntegerVariable(ModifyVariableEffect* variableEffect);

	FlyVariable* DrawVariableListPopup();
	FlyVariable* AddDefaultVariable();
	FlyVariable* GetVariable(std::string name); 

public: 
	std::list<FlyVariable*> blackboardVariablesList; 
};

#endif // !_BLACKBOARD_H_

