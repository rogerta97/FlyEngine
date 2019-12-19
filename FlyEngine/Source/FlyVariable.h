#ifndef _FLY_VARIABLE_H_
#define _FLY_VARIABLE_H_

#include "Globals.h"
#include <string>

enum VariableType
{
	Var_Integer,
	Var_Toggle,
	Var_None
};

class FlyVariable
{

public: 
	FlyVariable(); 
	~FlyVariable();

	void SetDefault(); 
	
public: 
	std::string name;
	VariableType varType;

	int varInteger;
	bool varToogle;

	UID uniqueID;
};

#endif 

