#include "RoomUIHandler.h"
#include "Room.h"
#include "UI_Image.h"
#include "Texture.h"
#include "ResourceManager.h"

RoomUIHandler::RoomUIHandler(Room* _roomAttached)
{
	roomAttached = _roomAttached;
}

RoomUIHandler::~RoomUIHandler()
{
}

void RoomUIHandler::DrawUIElements()
{
	for (auto& currentUIElement : uiElements)
	{
		currentUIElement->Draw(); 
	}
}

UI_Image* RoomUIHandler::CreateUIImage(UID resourceUID)
{
	UI_Image* newImage = new UI_Image(); 
	
	Texture* textureResource = (Texture*)ResourceManager::getInstance()->GetResource(resourceUID, ResourceType::RESOURCE_TEXTURE);
	newImage->Create(textureResource->GetPath()); 

	uiElements.push_back(newImage); 
	return newImage; 
}


