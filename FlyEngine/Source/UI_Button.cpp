#include "UI_Button.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "FlyObject.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

UI_Button::UI_Button()
{
	UI_Element::UI_Element();
	uiObjectDisplayImageBackground = nullptr;
	uiElementType = UI_BUTTON;
	behaviourInteraction = INTERACTION_NONE;

	backgroundTexture = nullptr; 

	idleTexture = nullptr; 
	mouseOverTexture = nullptr;
	mouseClickedTexture = nullptr;
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
	SetMainTexture(testTexture);

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
	// Save General Data 
	UI_Element::Save(jsonObject, serializeStr);
	uiObject->SaveTransform(serializeStr, jsonObject);

	json_object_dotset_number(jsonObject, string(serializeStr + string("UID")).c_str(), uid);
	json_object_dotset_number(jsonObject, string(serializeStr + string("BehaviourInteraction")).c_str(), behaviourInteraction);

	// Save Image Background Info
	switch (behaviourInteraction)
	{
	case COLOR_TINT:
		break; 

	case TEXTURE_SWAP:
		break;

	case INTERACTION_NONE:

		if (backgroundTexture != nullptr)
		{
			json_object_dotset_number(jsonObject, string(serializeStr + string("BackImage.ImageWidth")).c_str(), backgroundTexture->GetWidth());
			json_object_dotset_number(jsonObject, string(serializeStr + string("BackImage.ImageHeigth")).c_str(), backgroundTexture->GetHeigth());
			json_object_dotset_string(jsonObject, string(serializeStr + string("BackImage.BackgroundTextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem(backgroundTexture->GetPath(), true).c_str());
			json_object_dotset_string(jsonObject, string(serializeStr + string("BackImage.ResourcePath")).c_str(), backgroundTexture->GetPath());
		}
		else
		{
			json_object_dotset_number(jsonObject, string(serializeStr + string("BackImage.ImageWidth")).c_str(), 0);
			json_object_dotset_number(jsonObject, string(serializeStr + string("BackImage.ImageHeigth")).c_str(), 0);
			json_object_dotset_string(jsonObject, string(serializeStr + string("BackImage.TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
			json_object_dotset_string(jsonObject, string(serializeStr + string("BackImage.ResourcePath")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
		}
		break;
	}

	// Save Actions 

}

void UI_Button::Load(JSON_Object* jsonObject, string serializeStr)
{
	uid = json_object_dotget_number(jsonObject, string(serializeStr + string("UID")).c_str());
	int bi = json_object_dotget_number(jsonObject, string(serializeStr + string("BehaviourInteraction")).c_str());
	behaviourInteraction = (ButtonBehaviourMouseInteraction)bi; 

	switch (behaviourInteraction)
	{
	case COLOR_TINT:
		break;

	case TEXTURE_SWAP:
		break;

	case INTERACTION_NONE:
	{
		if (uiObject == nullptr)
			uiObject = new FlyObject("UIButtonHolder", "", UI_HOLDER); 

		// Load Background Image 
		if (json_object_dothas_value(jsonObject, std::string(serializeStr + string("BackImage.")).c_str()))
		{
			string backgroundTextureName = json_object_dotget_string(jsonObject, std::string(serializeStr + string("BackImage.BackgroundTextureName")).c_str());
			MyFileSystem::getInstance()->DeleteFileExtension(backgroundTextureName); 
			backgroundTexture = (Texture*)ResourceManager::getInstance()->GetResource(backgroundTextureName);
			string texturePath = json_object_dotget_string(jsonObject, std::string(serializeStr + string("BackImage.ResourcePath")).c_str());

			backgroundTexture->SetWidth(json_object_dotget_number(jsonObject, string(serializeStr + string("BackImage.ImageWidth")).c_str())); 
			backgroundTexture->SetHeigth(json_object_dotget_number(jsonObject, string(serializeStr + string("BackImage.ImageHeigth")).c_str())); 

			uiObjectDisplayImageBackground = uiObject->AddDisplayImageAction(texturePath.c_str());
		}
		
		break;
	}
	}

	// Load Transform
	Transform* uiElementTransform = GetHolderObject()->transform;
	LoadTransform(jsonObject, serializeStr.c_str(), uiElementTransform);

	// Addapt Gizmos 
	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	uiObject->FitObjectUtils();
}

void UI_Button::SetMainTexture(Texture* buttonBgnTexture)
{
	backgroundTexture = buttonBgnTexture; 

	if (uiObjectDisplayImageBackground == nullptr)
		uiObjectDisplayImageBackground = uiObject->AddDisplayImageAction(buttonBgnTexture->GetPath());
	else
		uiObjectDisplayImageBackground->SetTexture(buttonBgnTexture); 
}

DisplayImageAction* UI_Button::GetDisplayImageAction()
{
	return uiObjectDisplayImageBackground;
}

Texture* UI_Button::GetMainTexture()
{
	return backgroundTexture;
}

Texture* UI_Button::GetIdleTexture()
{
	return idleTexture;
}

Texture* UI_Button::GetMouseOverTexture()
{
	return mouseOverTexture;
}

Texture* UI_Button::GetMouseClickedTexture()
{
	return mouseClickedTexture;
}
