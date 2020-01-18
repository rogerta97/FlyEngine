#include "UI_Image.h"
#include "FlyObject.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"

UI_Image::UI_Image() : UI_Element()
{
	UI_Element::UI_Element(); 
	uiObjectDisplayImage = nullptr; 
}

UI_Image::~UI_Image()
{
}

void UI_Image::Draw()
{
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
