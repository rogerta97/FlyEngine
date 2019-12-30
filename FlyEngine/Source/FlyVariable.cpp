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

void FlyVariable::Serialize(JSON_Object* jsonObject, std::string _baseObjectStr)
{
	std::string saveString = _baseObjectStr + "VariableType";
	json_object_dotset_number(jsonObject, saveString.c_str(), varType);

	saveString = _baseObjectStr + "VariableName";
	json_object_dotset_string(jsonObject, saveString.c_str(), name.c_str());

	saveString = _baseObjectStr + "IntegerValue";
	json_object_dotset_number(jsonObject, saveString.c_str(), varIntegerValue);

	saveString = _baseObjectStr + "ToggleValue";
	json_object_dotset_boolean(jsonObject, saveString.c_str(), varToogleValue);

	saveString = _baseObjectStr + "UID";
	json_object_dotset_number(jsonObject, saveString.c_str(), uniqueID);
}
