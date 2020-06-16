#ifndef _FLY_VARIABLE_H_
#define _FLY_VARIABLE_H_

#include "Globals.h"
#include "SaveAndLoad.h"
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

	void CleanUp(); 

	void SetDefault(); 
	void Serialize(JSON_Object* jsonObject, std::string _baseObjectStr);
	
public: 
	std::string name;
	VariableType varType;
	bool isGlobal = false; 

	int varIntegerValue;
	bool varToogleValue;

	UID uniqueID;
};

#endif 

