#ifndef _BLACKBOARD_H_
#define _BLACKBOARD_H_

#include <list>
#include <string>
#include "Globals.h"

class FlyVariable; 
class Blackboard
{

public: 
	Blackboard(); 
	~Blackboard(); 

	FlyVariable* AddDefaultVariable(); 

public: 
	std::list<FlyVariable*> blackboardVariablesList; 
};

#endif // !_BLACKBOARD_H_

