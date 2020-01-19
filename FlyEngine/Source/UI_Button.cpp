#include "UI_Button.h"

UI_Button::UI_Button()
{
	UI_Element::UI_Element();
	uiObjectDisplayImage = nullptr;
	uiElementType = UI_BUTTON;
}

UI_Button::~UI_Button()
{
}

void UI_Button::Update()
{
}

void UI_Button::Draw()
{
}

void UI_Button::CleanUp()
{
}

void UI_Button::Save(JSON_Object* jsonObject, string serializeStr)
{
}

void UI_Button::Load(JSON_Object* jsonObject, string serializeStr)
{
}
