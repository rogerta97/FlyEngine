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
	mouseInteraction = INTERACTION_NONE;

	backgroundTexture = nullptr; 

	mainTexture = nullptr; 
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
	// TODO: BUTTON NEED CLEANUP :D
}

void UI_Button::Save(JSON_Object* jsonObject, string serializeStr)
{
	// Save General Data 
	uiObject->SaveTransform(serializeStr, jsonObject);
	json_object_dotset_number(jsonObject, string(serializeStr + string("UID")).c_str(), uid);

	UI_Element::Save(jsonObject, serializeStr);
	json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionType")).c_str(), mouseInteraction);

	// Save Image Background Info
	switch (mouseInteraction)
	{
	case COLOR_TINT:

		// Mouse Over Tint -------
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.r")).c_str(), mouseOverTint.x);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.g")).c_str(), mouseOverTint.y);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.b")).c_str(), mouseOverTint.z);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.a")).c_str(), mouseOverTint.w);

		// Mouse Clicked Tint -------
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.r")).c_str(), mouseClickedTint.x);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.g")).c_str(), mouseClickedTint.y);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.b")).c_str(), mouseClickedTint.z);
		json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.a")).c_str(), mouseClickedTint.w);

		break; 

	case TEXTURE_SWAP:

		// Mouse Over Tint -------
		if(mouseOverTexture != nullptr)
			json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTextureID")).c_str(), mouseOverTexture->GetTextureID());
		else
			json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTextureID")).c_str(), 0);

		// Mouse Clicked Tint -------
		if(mouseClickedTexture != nullptr)
			json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTextureID")).c_str(), mouseClickedTexture->GetTextureID());
		else
			json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTextureID")).c_str(), 0);

		break;

	case INTERACTION_NONE:

		if (backgroundTexture != nullptr)
		{
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageWidth")).c_str(), backgroundTexture->GetWidth());
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageHeigth")).c_str(), backgroundTexture->GetHeigth());
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.BackgroundTextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem(backgroundTexture->GetPath(), true).c_str());
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.ResourcePath")).c_str(), backgroundTexture->GetPath());
		}
		else
		{
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageWidth")).c_str(), 0);
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageHeigth")).c_str(), 0);
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.ResourcePath")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
		}
		break;
	}

	// Save Actions 
	json_object_dotset_number(jsonObject, string(serializeStr + string("OnClickActions.ActionsAmount")).c_str(), onClickActions.size());

	int count = 0; 
	for (auto& currentAction : onClickActions)
	{
		string actionStr = serializeStr + string("OnClickActions.Action_") + to_string(count) + "."; 
		currentAction->SaveAction(jsonObject, actionStr.c_str(), true);
		count++;
	}
}

void UI_Button::Load(JSON_Object* jsonObject, string serializeStr)
{
	uid = json_object_dotget_number(jsonObject, string(serializeStr + string("UID")).c_str());
	int bi = json_object_dotget_number(jsonObject, string(serializeStr + string("BehaviourInteraction")).c_str());
	mouseInteraction = (ButtonBehaviourMouseInteraction)bi; 

	switch (mouseInteraction)
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

std::list<Action*>& UI_Button::GetOnClickActionList()
{
	return onClickActions; 
}

void UI_Button::SetMouseOverTintColor(float4 newColor)
{
	mouseOverTint = newColor;
}

void UI_Button::SetMouseClickedTintColor(float4 newColor)
{
	mouseClickedTint = newColor; 
}

void UI_Button::SetMouseOverTexture(Texture* buttonTexture)
{
	mouseOverTexture = buttonTexture;
}

void UI_Button::SetMouseClickedTexture(Texture* buttonTexture)
{
	mouseClickedTexture = buttonTexture; 
}

DisplayImageAction* UI_Button::GetDisplayImageAction()
{
	return uiObjectDisplayImageBackground;
}

void UI_Button::AddOnClickAction(Action* newAction)
{
	onClickActions.push_back(newAction); 
}

Texture* UI_Button::GetMainTexture()
{
	return backgroundTexture;
}

Texture* UI_Button::GetMouseOverTexture()
{
	return mouseOverTexture;
}

Texture* UI_Button::GetMouseClickedTexture()
{
	return mouseClickedTexture;
}
