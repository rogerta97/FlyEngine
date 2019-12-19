#include "Blackboard.h"
#include "RandomNumberGenerator.h"
#include "FlyVariable.h"

Blackboard::Blackboard()
{
}

Blackboard::~Blackboard()
{
}

FlyVariable* Blackboard::AddDefaultVariable()
{
	FlyVariable* defaultVar = new FlyVariable(); 
	defaultVar->SetDefault();
	blackboardVariablesList.push_back(defaultVar);
	return defaultVar; 
}
