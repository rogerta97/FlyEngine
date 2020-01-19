#include "UI_Button.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "FlyObject.h"
#include "ResourceManager.h"

UI_Button::UI_Button()
{
	UI_Element::UI_Element();
	uiObjectDisplayImageBackground = nullptr;
	uiElementType = UI_BUTTON;
}

UI_Button::~UI_Button()
{
}

void UI_Button::Init()
{
	// For Now Set Object Default data on init
	uiObject = new FlyObject("TestButton", "", UI_HOLDER);
	
	// Background Image
	Texture* testTexture = (Texture*)ResourceManager::getInstance()->GetResource("BackButton"); 
	SetBackgroundTexture(testTexture);

	// Button Click 
	uiObject->CreateClickableArea(float2(1.0f, 1.0f), float2(1.0f, 1.0f), false); 
}

void UI_Button::Update()
{
	UI_Element::Update();
}

void UI_Button::Draw()
{
	uiObjectDisplayImageBackground->Draw();
	UI_Element::Draw();
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

void UI_Button::SetBackgroundTexture(Texture* buttonBgnTexture)
{
	idleTexture = buttonBgnTexture; 
	uiObjectDisplayImageBackground = uiObject->AddDisplayImageAction(buttonBgnTexture->GetPath());
}
