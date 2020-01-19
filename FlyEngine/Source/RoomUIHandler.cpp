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

RoomUIHandler::RoomUIHandler(Room* _roomAttached)
{
	roomAttached = _roomAttached;
	selectedElement = nullptr; 
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

		}

		count++;
	}
}

void RoomUIHandler::Update()
{
	for (auto& currentUIElement : uiElements)
	{
		if(selectedElement == currentUIElement)
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
	for (auto& currentUIElement : uiElements)
	{
		currentUIElement->CleanUp();
	}
}

void RoomUIHandler::DrawUIElements()
{
	for (auto& currentUIElement : uiElements)
	{
		currentUIElement->Draw(); 
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

UI_Element* RoomUIHandler::GetSelectedElement()
{
	return selectedElement;
}

UI_Image* RoomUIHandler::CreateUIImage(UID resourceUID)
{
	UI_Image* newImage = new UI_Image(); 
	
	Texture* textureResource = (Texture*)ResourceManager::getInstance()->GetResource(resourceUID, ResourceType::RESOURCE_TEXTURE);
	newImage->Create(textureResource->GetPath()); 

	uiElements.push_back(newImage); 
	return newImage; 
}

UI_Image* RoomUIHandler::CreateUIImage()
{
	UI_Image* newImage = new UI_Image();

	uiElements.push_back(newImage);
	return newImage;
}

UI_Button* RoomUIHandler::CreateUIButton()
{
	UI_Button* newButton = new UI_Button();

	newButton->Init(); 

	uiElements.push_back(newButton);
	return newButton;
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


