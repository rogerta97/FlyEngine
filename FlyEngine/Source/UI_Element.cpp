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

void UI_Element::Init()
{
}

void UI_Element::Update()
{
	if(App->moduleManager->GetSelectedUIElement() == this && !App->isEngineInPlayMode)
		uiObject->gizmos->Update();
}

void UI_Element::Draw()
{
	if (App->moduleManager->GetSelectedUIElement() == this)
		uiObject->gizmos->Draw(); 
}

void UI_Element::CleanUp()
{
	uiObject->CleanUp();
}

void UI_Element::Save(JSON_Object* jsonObject, string serializeStr)
{
	json_object_dotset_number(jsonObject, string(serializeStr + string("UIElementType")).c_str(), uiElementType);
}

void UI_Element::Load(JSON_Object* jsonObject, string serializeStr)
{
}

void UI_Element::LoadTransform(JSON_Object* jsonObject, std::string serializeStr, Transform* objectTransform)
{
	float positionX = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Position.x")).c_str());
	float positionY = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Position.y")).c_str());
	float2 position(positionX, positionY);
	objectTransform->SetPosition(position);

	float rotationX = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Rotation.x")).c_str());
	float rotationY = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Rotation.y")).c_str());
	float2 rotation(rotationX, rotationY);
	objectTransform->SetRotationEuler(rotation);

	float scaleX = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Scale.x")).c_str());
	float scaleY = json_object_dotget_number(jsonObject, string(serializeStr + string("Transform.Scale.y")).c_str());
	float2 scale(scaleX, scaleY);
	objectTransform->SetScale(scale);
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

UID UI_Element::GetUID()
{
	return uid;
}
