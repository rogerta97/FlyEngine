#include "UI_Button.h"
#include "DisplayImageAction.h"
#include "ModuleInput.h"
#include "Texture.h"
#include "FlyObject.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "Gizmos.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

#include "mmgr.h"

UI_Button::UI_Button()
{
	UI_Element::UI_Element();
	uiObjectDisplayImage = nullptr;
	uiElementType = UI_BUTTON;
	mouseInteraction = COLOR_TINT;
	mouseState = MOUSE_OUT;

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

	if (!App->isEngineInPlayMode)
		return; 

	ButtonMouseState mouseStateChange = HandleMouseState(); 
	if (mouseStateChange != MOUSE_NULL)
	{
		switch (mouseInteraction)
		{
		case COLOR_TINT:

			break;

		case TEXTURE_SWAP:

			if (mouseStateChange == MOUSE_OUT)
			{
				GetDisplayImageAction()->SetTexture(mainTexture);
			}
			else if (mouseStateChange == MOUSE_OVER)
			{
				GetDisplayImageAction()->SetTexture(mouseOverTexture);
			}
			else if (mouseStateChange == MOUSE_CLICKED)
			{
				GetDisplayImageAction()->SetTexture(mouseClickedTexture);
			}

			break;
		}
	}
}

ButtonMouseState UI_Button::HandleMouseState()
{
	if (mouseState == MOUSE_OUT)
	{
		if (GetHolderObject()->gizmos->IsMouseOver())
		{
			mouseState = MOUSE_OVER;		
			return MOUSE_OVER;
		}
	}

	if (mouseState == MOUSE_OVER)
	{
		if (!GetHolderObject()->gizmos->IsMouseOver())
		{
			mouseState = MOUSE_OUT;
			return MOUSE_OUT;
		}
		
		if (GetHolderObject()->gizmos->IsMouseOverClicked())
		{
			mouseState = MOUSE_CLICKED;			
			return MOUSE_CLICKED; 
		}
	}

	if (mouseState == MOUSE_CLICKED)
	{
		if (App->moduleInput->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		{
			if (GetHolderObject()->gizmos->IsMouseOver())
			{
				mouseState = MOUSE_OVER;
				return MOUSE_OVER;
			}
			else
			{
				mouseState = MOUSE_OUT;		
				return MOUSE_OUT; 
			}
		}
	}

	return MOUSE_NULL;
}

void UI_Button::Draw()
{
	uiObjectDisplayImage->Draw();
	UI_Element::Draw();
}

void UI_Button::CleanUp()
{
	uiObject->DeleteAction(uiObjectDisplayImage->GetActionName());
	uiObjectDisplayImage = nullptr;

	UI_Element::CleanUp(); 

	uiObjectDisplayImage = nullptr;
	mainTexture = nullptr;
	mouseOverTexture = nullptr;
	mouseClickedTexture = nullptr;
}

void UI_Button::Save(JSON_Object* jsonObject, string serializeStr)
{
	// Save General Data 
	uiObject->SaveTransform(serializeStr, jsonObject);
	json_object_dotset_number(jsonObject, string(serializeStr + string("UID")).c_str(), uid);

	UI_Element::Save(jsonObject, serializeStr);
	json_object_dotset_number(jsonObject, string(serializeStr + string("MouseInteractionType")).c_str(), (int)mouseInteraction);

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
			json_object_dotset_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTextureName")).c_str(), mouseOverTexture->GetName().c_str());
		else
			json_object_dotset_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTextureName")).c_str(), "None");

		// Mouse Clicked Tint -------
		if(mouseClickedTexture != nullptr)
			json_object_dotset_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTextureName")).c_str(), mouseClickedTexture->GetName().c_str());
		else
			json_object_dotset_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTextureName")).c_str(), "None");

		break;

	case INTERACTION_NONE:

		if (mainTexture != nullptr)
		{
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageWidth")).c_str(), mainTexture->GetWidth());
			json_object_dotset_number(jsonObject, string(serializeStr + string("MainImage.ImageHeigth")).c_str(), mainTexture->GetHeigth());
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.BackgroundTextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem(mainTexture->GetPath(), true).c_str());
			json_object_dotset_string(jsonObject, string(serializeStr + string("MainImage.ResourcePath")).c_str(), mainTexture->GetPath());
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
	int bi = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionType")).c_str());
	mouseInteraction = (ButtonBehaviourMouseInteraction)bi; 

	switch (mouseInteraction)
	{
	case COLOR_TINT:

		// Mouse Over Tint -------
		mouseOverTint.x = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.r")).c_str());
		mouseOverTint.y = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.g")).c_str());
		mouseOverTint.z = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.b")).c_str());
		mouseOverTint.w = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTint.a")).c_str());

		// Mouse Clicked Tint -------
		mouseClickedTint.x = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.r")).c_str());
		mouseClickedTint.y = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.g")).c_str());
		mouseClickedTint.z = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.b")).c_str());
		mouseClickedTint.w = json_object_dotget_number(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTint.a")).c_str());

		break;

	case TEXTURE_SWAP:
	{
		// Mouse Over Tint -------
		string mouseOverName = json_object_dotget_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseOverTextureName")).c_str());
		MyFileSystem::getInstance()->DeleteFileExtension(mouseOverName);

		if(mouseOverName != "None")
			mouseOverTexture = (Texture*)ResourceManager::getInstance()->GetResource(mouseOverName.c_str());

		string mouseClickedName = json_object_dotget_string(jsonObject, string(serializeStr + string("MouseInteractionData.MouseClickedTextureName")).c_str());
		MyFileSystem::getInstance()->DeleteFileExtension(mouseClickedName);

		if (mouseClickedName != "None")
			mouseClickedTexture = (Texture*)ResourceManager::getInstance()->GetResource(mouseClickedName.c_str());
	}
		break;
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
	mainTexture = buttonBgnTexture;

	if (uiObjectDisplayImage == nullptr)
		uiObjectDisplayImage = uiObject->AddDisplayImageAction(buttonBgnTexture->GetPath());
	else
		uiObjectDisplayImage->SetTexture(buttonBgnTexture); 
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
	return uiObjectDisplayImage;
}

void UI_Button::AddOnClickAction(Action* newAction)
{
	onClickActions.push_back(newAction); 
}

Texture* UI_Button::GetMainTexture()
{
	return mainTexture;
}

Texture* UI_Button::GetMouseOverTexture()
{
	return mouseOverTexture;
}

Texture* UI_Button::GetMouseClickedTexture()
{
	return mouseClickedTexture;
}
