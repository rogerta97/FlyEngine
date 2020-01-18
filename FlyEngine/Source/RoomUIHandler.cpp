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


