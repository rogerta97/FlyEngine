#include "FlyVariable.h"
#include "RandomNumberGenerator.h"

FlyVariable::FlyVariable()
{
}

FlyVariable::~FlyVariable()
{
}

void FlyVariable::SetDefault()
{
	name = "Default_Var";
	varType = Var_Integer;
	varInteger = 0;
	varToogle = true;
	uniqueID = RandomNumberGenerator::getInstance()->GenerateUID();
}
