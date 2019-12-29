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
	varIntegerValue = 0;
	varToogleValue = true;
	uniqueID = RandomNumberGenerator::getInstance()->GenerateUID();
}
