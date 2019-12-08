#include "JSONParser.h"

JSONParser* JSONParser::instance = 0;

JSONParser* JSONParser::getInstance()
{
	if (instance == 0)
		instance = new JSONParser();

	return instance;
}

JSONParser::~JSONParser()
{
	instance->Delete(); 
}

JSONParser::JSONParser()
{
	
}

void JSONParser::Delete()
{
	delete instance; 
}
