#include "UI_Text.h"
#include "DisplayTextAction.h"
#include "FlyObject.h"
#include "ResourceManager.h"

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
	uiObject->DeleteAction(displayTextAction->GetType());
	displayTextAction = nullptr;
	UI_Element::CleanUp();
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
	// Get Resource Attached
	Init();

	serializeStr += "Actions.DisplayText."; 
	string fontName = json_object_dotget_string(jsonObject, std::string(serializeStr + "FontName").c_str());
	Font* currentFont = ResourceManager::getInstance()->GetFont(fontName);

	std::string text = json_object_dotget_string(jsonObject, std::string(serializeStr + "Text").c_str());
	displayTextAction->SetText(text);
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
	displayTextAction->SetText("This is a test string"); 
	
}
