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
	Blackboard(bool _isGlobal = false);
	~Blackboard(); 

	void CleanUp(); 

	void SaveData(std::string fileName); 
	void LoadData(std::string fileName); 

	void ModifyIntegerVariable(ModifyVariableEffect* variableEffect);

	// Utility 
	FlyVariable* DrawVariableListPopup(std::string popupID);
	FlyVariable* AddDefaultVariable();
	FlyVariable* GetVariable(std::string name); 

public: 
	std::list<FlyVariable*> blackboardVariablesList; 

private:
	bool isGlobal; 
};

#endif // !_BLACKBOARD_H_

