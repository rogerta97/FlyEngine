#include "RoomUIHandler.h"
#include "Room.h"
#include "MyFileSystem.h"
#include "FlyObject.h"
#include "Texture.h"
#include "DisplayImageAction.h"
#include "ResourceManager.h"
#include "ViewportManager.h"
#include "Application.h"
#include "ModuleInput.h"

#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "mmgr.h"

RoomUIHandler::RoomUIHandler(Room* _roomAttached)
{
	roomAttached = _roomAttached;
	selectedElement = nullptr; 
	selectedButtonUIAction = nullptr; 
}

RoomUIHandler::~RoomUIHandler()
{
}

void RoomUIHandler::SaveRoomUI(JSON_Object* jsonObject, string baseSerializeStr)
{
	json_object_dotset_number(jsonObject, string(baseSerializeStr + ".ElementsAmount").c_str(), uiElements.size());

	if (!uiElements.empty())
	{
		int count = 0; 
		for (auto& currentUIElement : uiElements)
		{
			string serializeStr = baseSerializeStr + ".Element_" + to_string(count) + "."; 
			currentUIElement->Save(jsonObject, serializeStr);
			count++; 
		}
	}
}

void RoomUIHandler::LoadRoomUI(JSON_Object* jsonObject, string baseSerializeStr)
{
	int elementsAmount = json_object_dotget_number(jsonObject, std::string(baseSerializeStr + ".ElementsAmount").c_str()); 

	int count = 0; 
	while (count < elementsAmount)
	{
		string elementStr = baseSerializeStr + ".Element_" + to_string(count) + "."; 
		int currentElementType = json_object_dotget_number(jsonObject, std::string(elementStr + "UIElementType").c_str());

		switch (currentElementType)
		{
		case UIElementType::UI_IMAGE:
		{
			UI_Image* newElementImageCast = CreateUIImage();
			newElementImageCast->Load(jsonObject, elementStr);
			break; 
		}
		case UIElementType::UI_BUTTON:
		{
			UI_Button* newElementButtonCast = CreateUIButton();
			newElementButtonCast->Load(jsonObject, elementStr);
			break;
		}

		case UIElementType::UI_TEXT:
		{
			UI_Text* newElementTextCast = CreateUIText();
			newElementTextCast->Load(jsonObject, elementStr);
			break;
		}

		}

		count++;
	}

}

void RoomUIHandler::Update()
{
	for (auto& currentUIElement : uiElements)
	{
		currentUIElement->Update();
	}

	// Input Handling
	if (App->moduleInput->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN && ViewportManager::getInstance()->editRoomMode == EDIT_ROOM_UI)
	{
		if (selectedElement != nullptr)
		{
			DeleteElement(selectedElement->GetUID());
			selectedElement = nullptr; 
		}
	}
}

void RoomUIHandler::CleanUp()
{
	for (auto currentElement = uiElements.begin(); currentElement != uiElements.end();)
	{
		(*currentElement)->CleanUp(); 
		delete (*currentElement);
		currentElement = uiElements.erase(currentElement);
	}

	uiElements.clear(); 
	selectedElement = nullptr; 
	selectedButtonUIAction = nullptr;
}

void RoomUIHandler::DrawUIElements()
{
	for (auto& currentUIElement : uiElements)
	{
		currentUIElement->Draw(); 
	}
}

UID RoomUIHandler::DrawUIElementsHierarchy()
{
	int count = 0;
	for (auto& currentElement : uiElements)
	{
		string tabname = currentElement->GetHolderObject()->GetName() + "##UIHierarchySelectable" + to_string(count);

		if (ImGui::Selectable(tabname.c_str(), currentElement->isSelected))
		{
			return currentElement->GetUID();
		}

		count++; 
	}

	return 0;
}

void RoomUIHandler::AddEmptyElement(UIElementType elementType)
{
	switch (elementType)
	{
	case UI_IMAGE:
		CreateUIImage();
		break;

	case UI_BUTTON:
		CreateUIButton();
		break;

	case UI_TEXT:
		CreateUIText(); 
		break;
	}
}

void RoomUIHandler::SetSelectedElement(UI_Element* newSelectedElement)
{
	if (selectedElement == newSelectedElement)
		return;

	for (auto& it : uiElements)
	{
		if ((it) == newSelectedElement)
		{
			(it)->isSelected = true;
		}
		else
		{
			it->isSelected = false;
		}
	}

	selectedElement = newSelectedElement;
}

void RoomUIHandler::SetSelectedElement(UID newSelectedElementUID)
{
	UI_Element* newSelectedElement = GetUIElement(newSelectedElementUID);
	SetSelectedElement(newSelectedElement); 
}

UI_Element* RoomUIHandler::GetUIElement(UID elementUID)
{
	for (auto& currentElement : uiElements)
	{
		if (currentElement->GetUID() == elementUID)
			return currentElement; 
	}

	return nullptr; 
}

UI_Element* RoomUIHandler::GetSelectedElement()
{
	return selectedElement;
}

void RoomUIHandler::FitUIElements()
{
	for (auto& currentElement : uiElements)
	{
		currentElement->GetHolderObject()->FitObjectUtils(); 
	}
}

void RoomUIHandler::DrawSelectedOnClickActionSettings()
{
	if (selectedButtonUIAction == nullptr)
		return; 

	selectedButtonUIAction->DrawUISettingsInButton();
}

UIElementType RoomUIHandler::DrawUIElementSelectorPopup()
{	
	UIElementType uiElementTypeClicked = UIElementType::UI_null; 
	if (ImGui::BeginPopup("SelectUIElement"))
	{
		Texture* checkVariableIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckVariableIcon");
		ImGui::Image((ImTextureID)checkVariableIcon->GetTextureID(), ImVec2(18, 18));
		ImGui::SameLine();

		if (ImGui::Selectable("Add UI Image"))
		{
			uiElementTypeClicked = UI_IMAGE;
		}
			
		checkVariableIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckVariableIcon");
		ImGui::Image((ImTextureID)checkVariableIcon->GetTextureID(), ImVec2(18, 18));
		ImGui::SameLine();

		if (ImGui::Selectable("Add UI Button"))
		{
			uiElementTypeClicked = UI_BUTTON;
		}

		checkVariableIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckVariableIcon");
		ImGui::Image((ImTextureID)checkVariableIcon->GetTextureID(), ImVec2(18, 18));
		ImGui::SameLine();

		if (ImGui::Selectable("Add UI Text"))
		{
			uiElementTypeClicked = UI_TEXT;
		}

		ImGui::EndPopup();
	}

	return uiElementTypeClicked; 	
}

UI_Image* RoomUIHandler::CreateUIImage(UID resourceUID)
{
	UI_Image* newImage = new UI_Image(); 
	
	Texture* textureResource = (Texture*)ResourceManager::getInstance()->GetResource(resourceUID, ResourceType::RESOURCE_TEXTURE);
	newImage->Init(textureResource->GetPath()); 

	uiElements.push_back(newImage); 
	return newImage; 
}

UI_Image* RoomUIHandler::CreateUIImage()
{
	Resource* phResource = ResourceManager::getInstance()->GetResource("EmptyObject"); 
	return CreateUIImage(phResource->GetUID());
}

UI_Button* RoomUIHandler::CreateUIButton()
{
	UI_Button* newButton = new UI_Button();
	newButton->Init(); 
	uiElements.push_back(newButton);
	return newButton;
}

UI_Text* RoomUIHandler::CreateUIText()
{
	UI_Text* newText = new UI_Text();
	newText->Init();
	uiElements.push_back(newText);
	return newText;
}

void RoomUIHandler::DeleteElement(UID elementUID)
{
	for (auto currentElement = uiElements.begin(); currentElement != uiElements.end();)
	{
		if ((*currentElement)->GetUID() == elementUID)
		{
			(*currentElement)->CleanUp();
			delete (*currentElement);
			(*currentElement) = nullptr;

			currentElement = uiElements.erase(currentElement);

			if (uiElements.empty())
				break;
		}
		else
			currentElement++;
	}
}


