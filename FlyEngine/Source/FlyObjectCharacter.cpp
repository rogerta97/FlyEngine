#include "FlyObjectCharacter.h"
#include "DisplayAnimationAction.h"
#include "MyFileSystem.h"
#include "DisplayImageAction.h"
#include "Room.h"

FlyObjectCharacter::FlyObjectCharacter(std::string objectName, std::string description, FlyObjectType _flyObjectType, Room* parentRoom) : FlyObject(objectName, description,  _flyObjectType, parentRoom)
{
	flyObjectType = OBJECT_CHARACTER;
}

FlyObjectCharacter::FlyObjectCharacter()
{
}

void FlyObjectCharacter::SaveObjectData(JSON_Object* jsonObject, int objectIndex)
{
	// Save Object Properties
	string serializeObjectName = "RoomData.Objects.FlyObject_" + to_string(objectIndex) + string(".");

	json_object_dotset_string(jsonObject, string(serializeObjectName + "Name").c_str(), GetName().c_str());
	json_object_dotset_number(jsonObject, string(serializeObjectName + "UID").c_str(), uid);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Interactable").c_str(), isInteractable);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "ObjectType").c_str(), this->flyObjectType);

	if (flyObjectType == OBJECT_CHARACTER)
	{
		string saveCharacterAnimStr = serializeObjectName + "CharacterAnimations.IdleAnimation.";
		idleAnimation->SaveAction(jsonObject, saveCharacterAnimStr.c_str(), true); 

		saveCharacterAnimStr = serializeObjectName + "CharacterAnimations.WalkAnimation.";
		walkAnimation->SaveAction(jsonObject, saveCharacterAnimStr.c_str(), true);

		saveCharacterAnimStr = serializeObjectName + "CharacterAnimations.TalkAnimation.";
		talkAnimation->SaveAction(jsonObject, saveCharacterAnimStr.c_str(), true);
	}

	if (!GetDescription().empty())
		json_object_dotset_string(jsonObject, string(serializeObjectName + "Description").c_str(), GetDescription().c_str());

	SaveTransform(serializeObjectName, jsonObject);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Actions.ActionsAmount").c_str(), actionsList.size());

	// Save Object Action Settings
	int counter = 0;
	for (auto& it : actionsList)
	{
		it->SaveAction(jsonObject, serializeObjectName, false, counter);
		counter++;
	}

	json_object_dotset_number(jsonObject, string(serializeObjectName + "SequentialActions.ActionsAmount").c_str(), sequentialActionsList.size());

	// Save Object Sequenntial Action Settings
	counter = 0;
	for (auto& it : sequentialActionsList)
	{
		it->SaveAction(jsonObject, std::string(serializeObjectName + "SequentialActions."), false, counter);
		counter++;
	}

	// Save Object Clickable Area
	SaveClickableArea(serializeObjectName, jsonObject);
}

void FlyObjectCharacter::InitCharacter()
{
	DisplayImageAction* fixedImageAction = AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str(), false);

	// Idle Animation -------------------------------------
	idleAnimation = new DisplayAnimationAction(this);
	idleAnimation->SetIsSequencial(false);

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		idleAnimation->AttachToImage(fixedImageAction);

	// Walk Animation -------------------------------------
	walkAnimation = new DisplayAnimationAction(this);
	walkAnimation->SetIsSequencial(false);
	walkAnimation->canChangeCanvas = false; 

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		walkAnimation->AttachToImage(fixedImageAction);

	// Talk Animation -------------------------------------
	talkAnimation = new DisplayAnimationAction(this);
	talkAnimation->SetIsSequencial(false);
	talkAnimation->canChangeCanvas = false;

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		talkAnimation->AttachToImage(fixedImageAction);
}

DisplayAnimationAction* FlyObjectCharacter::GetWalkAnimation()
{
	return walkAnimation;
}

DisplayAnimationAction* FlyObjectCharacter::GetIdleAnimation()
{
	return idleAnimation;
}

DisplayAnimationAction* FlyObjectCharacter::GetTalkAnimation()
{
	return talkAnimation;
}

FlyObjectCharacter::~FlyObjectCharacter()
{
}

