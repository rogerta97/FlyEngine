#include "RoomUIHandler.h"
#include "Room.h"
#include "UI_Image.h"
#include "Texture.h"
#include "ResourceManager.h"

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

void RoomUIHandler::Update()
{
	for (auto& currentUIElement : uiElements)
	{
		if(selectedElement == currentUIElement)
			currentUIElement->Update();
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


