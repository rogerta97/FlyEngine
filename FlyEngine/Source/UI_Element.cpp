#include "UI_Element.h"
#include "RandomNumberGenerator.h"
#include "FlyObject.h"
#include "Gizmos.h"
#include "Application.h"
#include "ModuleManager.h"

UI_Element::UI_Element()
{
	uiObject = nullptr;
	uid = RandomNumberGenerator::getInstance()->GenerateUID(); 
}

UI_Element::~UI_Element()
{
}

void UI_Element::Update()
{
	uiObject->gizmos->Update();
}

void UI_Element::Draw()
{
	if (App->moduleManager->GetSelectedUIElement() == this)
		uiObject->gizmos->Draw(); 
}

void UI_Element::CleanUp()
{
}

void UI_Element::Save(JSON_Object* jsonObject, string serializeStr)
{
	json_object_dotset_number(jsonObject, string(serializeStr + string("UIElementType")).c_str(), uiElementType);
}

void UI_Element::Load(JSON_Object* jsonObject, string serializeStr)
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
