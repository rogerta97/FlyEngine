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

DisplayTextAction* UI_Text::GetDisplayTextAction()
{
	return displayTextAction;
}

void UI_Text::Init()
{
	if (uiObject == nullptr)
		uiObject = new FlyObject("ImageUIHolder", "", UI_HOLDER);

	displayTextAction = uiObject->AddDisplayTextAction();
	displayTextAction->SetDrawTextBox(true);
	displayTextAction->SetText("UI Text"); 
	
}
