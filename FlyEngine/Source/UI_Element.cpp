#include "UI_Element.h"
#include "RandomNumberGenerator.h"
#include "FlyObject.h"
#include "Gizmos.h"

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
	uiObject->gizmos->DrawSelectGizmo(); 
}

void UI_Element::CleanUp()
{
}

FlyObject* UI_Element::GetHolderObject()
{
	return uiObject;
}

FlyObject* UI_Element::CreateBaseUI()
{
	// Will Create Transparent Plane For UI Element Selection 

	return nullptr;
}
