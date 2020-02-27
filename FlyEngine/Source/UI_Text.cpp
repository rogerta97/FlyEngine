#include "UI_Text.h"
#include "DisplayTextAction.h"
#include "FlyObject.h"

UI_Text::UI_Text()
{
	UI_Element::UI_Element();
	displayTextAction = nullptr;
	uiElementType = UI_TEXT;
}

UI_Text::~UI_Text()
{
}

void UI_Text::Update()
{
}

void UI_Text::Draw()
{
	if (displayTextAction)
		displayTextAction->RenderText(); 	

	if (isSelected)
		displayTextAction->DrawTextBox(); 
}

void UI_Text::CleanUp()
{
}

void UI_Text::Save(JSON_Object* jsonObject, string serializeStr)
{
	UI_Element::Save(jsonObject, serializeStr);
	uiObject->SaveTransform(serializeStr, jsonObject);

	json_object_dotset_number(jsonObject, string(serializeStr + string("UID")).c_str(), uid);

	displayTextAction->SaveAction(jsonObject, serializeStr); 
}

void UI_Text::Load(JSON_Object* jsonObject, string serializeStr)
{	
}

DisplayTextAction* UI_Text::GetDisplayTextAction()
{
	return displayTextAction;
}

void UI_Text::Init()
{
	if (uiObject == nullptr)
		uiObject = new FlyObject("UI Text", "", UI_HOLDER);

	displayTextAction = uiObject->AddDisplayTextAction();
	displayTextAction->SetDrawTextBox(true);
	displayTextAction->SetText("UI Text"); 
	
}
