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

	void CleanUp(); 

	void SaveData(std::string fileName); 
	void LoadData(std::string fileName); 

	void ModifyIntegerVariable(ModifyVariableEffect* variableEffect);

	// Utility 
	FlyVariable* DrawVariableListPopup();
	FlyVariable* AddDefaultVariable();
	FlyVariable* GetVariable(std::string name); 

public: 
	std::list<FlyVariable*> blackboardVariablesList; 
};

#endif // !_BLACKBOARD_H_

