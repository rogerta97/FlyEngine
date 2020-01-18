#include "UI_Element.h"
#include "RandomNumberGenerator.h"

UI_Element::UI_Element()
{
	uiObject = nullptr;
	uid = RandomNumberGenerator::getInstance()->GenerateUID(); 
}

UI_Element::~UI_Element()
{
}

void UI_Element::Draw()
{
}
