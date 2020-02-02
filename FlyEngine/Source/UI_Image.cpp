#include "UI_Image.h"
#include "FlyObject.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "MyFileSystem.h"
#include "Gizmos.h"
#include "GameViewportDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ResourceManager.h"

#include "mmgr.h"

UI_Image::UI_Image() : UI_Element()
{
	UI_Element::UI_Element(); 
	uiObjectDisplayImage = nullptr; 
	uiElementType = UI_IMAGE; 
}

UI_Image::~UI_Image()
{
}

void UI_Image::Update()
{
	UI_Element::Update(); 
}

void UI_Image::Draw()
{
	uiObjectDisplayImage->Draw(); 
	UI_Element::Draw(); 
}

void UI_Image::CleanUp()
{
	uiObject->DeleteAction(uiObjectDisplayImage->GetActionName()); 
	uiObjectDisplayImage = nullptr; 
	UI_Element::CleanUp(); 
}

void UI_Image::Save(JSON_Object* jsonObject, string serializeStr)
{
	UI_Element::Save(jsonObject, serializeStr);
	uiObject->SaveTransform(serializeStr, jsonObject); 

	json_object_dotset_number(jsonObject, string(serializeStr + string("UID")).c_str(), uid);

	if (uiObjectDisplayImage->GetTexture() != nullptr)
	{
		json_object_dotset_number(jsonObject, string(serializeStr + string("ImageWidth")).c_str(), uiObjectDisplayImage->GetWidth());
		json_object_dotset_number(jsonObject, string(serializeStr + string("ImageHeigth")).c_str(), uiObjectDisplayImage->GetHeigth());

		json_object_dotset_string(jsonObject, string(serializeStr + string("TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem(uiObjectDisplayImage->GetTexture()->GetPath(), true).c_str());
		json_object_dotset_string(jsonObject, string(serializeStr + string("ResourcePath")).c_str(), uiObjectDisplayImage->GetTexture()->GetPath());
	}
	else
	{
		json_object_dotset_number(jsonObject, string(serializeStr + string("ImageWidth")).c_str(), 0);
		json_object_dotset_number(jsonObject, string(serializeStr + string("ImageHeigth")).c_str(), 0);
		json_object_dotset_string(jsonObject, string(serializeStr + string("TextureName")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
		json_object_dotset_string(jsonObject, string(serializeStr + string("ResourcePath")).c_str(), MyFileSystem::getInstance()->GetLastPathItem("None", true).c_str());
	}
}

void UI_Image::Load(JSON_Object* jsonObject, string serializeStr)
{
	// Get Resource Attached
	string texturePath = json_object_dotget_string(jsonObject, std::string(serializeStr + "TextureName").c_str());
	texturePath = MyFileSystem::getInstance()->DeleteFileExtension(texturePath);
	Texture* resourceTexture = (Texture*)ResourceManager::getInstance()->GetResource(texturePath);

	// Set Width and Heigth
	int imageWidth = json_object_dotget_number(jsonObject, std::string(serializeStr + "ImageWidth").c_str());
	int imageHeigth = json_object_dotget_number(jsonObject, std::string(serializeStr + "ImageHeigth").c_str());
		
	Create(resourceTexture->GetPath());

	uiObjectDisplayImage->SetWidth(imageWidth);
	uiObjectDisplayImage->SetHeigth(imageHeigth);

	// Load Transform
	Transform* uiElementTransform = GetHolderObject()->transform;
	LoadTransform(jsonObject, serializeStr.c_str(), uiElementTransform);
	
	// Addapt Gizmos 
	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	uiObject->FitObjectUtils(); 
}

void UI_Image::Create(string imagePath)
{
	string resourceName = MyFileSystem::getInstance()->GetLastPathItem(imagePath.c_str(), false); 
	Texture* textureResource = (Texture*)ResourceManager::getInstance()->GetResource(resourceName);

	if (uiObject == nullptr)
		uiObject = new FlyObject("ImageUIHolder", "", UI_HOLDER);

	uiObjectDisplayImage = uiObject->AddDisplayImageAction(imagePath.c_str());
	uiObjectDisplayImage->SetTexture(textureResource);
}

DisplayImageAction* UI_Image::GetDisplayImage()
{
	return uiObjectDisplayImage;
}

