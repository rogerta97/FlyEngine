#include "RandomNumberGenerator.h"
#include "..//3rdParty/PCGRandom/pcg_basic.h"

RandomNumberGenerator* RandomNumberGenerator::instance = 0; 

RandomNumberGenerator* RandomNumberGenerator::getInstance() {
	if (instance == 0) instance = new RandomNumberGenerator();
	return instance;
}

void RandomNumberGenerator::Delete()
{
	delete instance; 
}

int RandomNumberGenerator::GetPCGRandomNumberInt(int minValue, int maxValue)
{
	int num_generated = 0;
	int margin = maxValue - minValue;

	num_generated = pcg32_boundedrand(margin);
	num_generated += minValue;

	return num_generated;
}

float RandomNumberGenerator::GetPCGRandomNumberFloat(float maxValue, float minValue)
{
	float num_generated = 0.0f;
	float margin_flt = maxValue - minValue;

	margin_flt *= 100;
	int margin = (int)margin_flt;

	num_generated = pcg32_boundedrand(margin);

	float inc = (float)num_generated / 100;
	num_generated = minValue + inc;

	return num_generated;
}

UID RandomNumberGenerator::GenerateUID()
{
	return GetPCGRandomNumberInt(0, 9999999);
}
