#include "RoomUIHandler.h"
#include "Room.h"
#include "UI_Image.h"
#include "MyFileSystem.h"
#include "FlyObject.h"
#include "Texture.h"
#include "DisplayImageAction.h"
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

void RoomUIHandler::LoadRoomUI(JSON_Object* jsonObject, string baseSerializeStr)
{
	int elementsAmount = json_object_dotget_number(jsonObject, std::string(baseSerializeStr + ".ElementsAmount").c_str()); 

	int count = 0; 
	while (count < elementsAmount)
	{
		string elementStr = baseSerializeStr + ".Element_" + to_string(count) + "."; 
		int currentElementType = json_object_dotget_number(jsonObject, std::string(elementStr + ".UIElementType").c_str());
		UI_Element* newElement = nullptr; 

		switch (currentElementType)
		{
		case UIElementType::UI_IMAGE:
		{
			newElement = new UI_Image();
			
			// Get Resource Attached
			string texturePath = json_object_dotget_string(jsonObject, std::string(elementStr + "TextureName").c_str());
			MyFileSystem::getInstance()->DeleteFileExtension(texturePath);
			Texture* resourceTexture = (Texture*)ResourceManager::getInstance()->GetResource(texturePath);

			// Cast To UI Image 
			UI_Image* newElementImageCast = CreateUIImage(resourceTexture->GetUID());

			// Set Width and Heigth
			int imageWidth = json_object_dotget_number(jsonObject, std::string(elementStr + "ImageWidth").c_str());
			int imageHeigth = json_object_dotget_number(jsonObject, std::string(elementStr + "ImageHeigth").c_str());

			newElementImageCast->GetDisplayImage()->SetWidth(imageWidth);
			newElementImageCast->GetDisplayImage()->SetHeigth(imageHeigth);

			// Load Transform
			Transform* uiElementTransform = newElementImageCast->GetHolderObject()->transform; 
			newElementImageCast->LoadTransform(jsonObject, elementStr.c_str(), uiElementTransform);
		}
			break; 
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


