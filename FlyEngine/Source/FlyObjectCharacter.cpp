#include "FlyObjectCharacter.h"
#include "DisplayAnimationAction.h"
#include "MyFileSystem.h"
#include "DisplayImageAction.h"
#include "Room.h"

#include "Application.h"
#include "ModuleInput.h"

FlyObjectCharacter::FlyObjectCharacter(std::string objectName, std::string description, FlyObjectType _flyObjectType, Room* parentRoom) : FlyObject(objectName, description,  _flyObjectType, parentRoom)
{
	flyObjectType = OBJECT_CHARACTER;
	characterMode = CHARACTER_IDLE; 
	playingAnimation = nullptr; 
}

FlyObjectCharacter::FlyObjectCharacter()
{

}

bool FlyObjectCharacter::Update(float dt)
{
	FlyObject::Update(dt); 

	if (flyObjectType == OBJECT_CHARACTER && sequentialSwapedTick)
	{
		if(currentSequentialAction != nullptr && currentSequentialAction->GetType() == ACTION_FOLLOW_PATH || App->moduleInput->GetKey(SDL_SCANCODE_2))
			PlayCharacterAnimation(CharacterMode::CHARACTER_MOVE); 
		else if (currentSequentialAction != nullptr  && currentSequentialAction->GetType() == ACTION_DIALOGUE || App->moduleInput->GetKey(SDL_SCANCODE_3))
			PlayCharacterAnimation(CharacterMode::CHARACTER_TALK);
		else
			PlayCharacterAnimation(CharacterMode::CHARACTER_IDLE);
	}

	if (playingAnimation != nullptr)
	{
		idleAnimation->Update(dt);
		talkAnimation->Update(dt);
		moveAnimation->Update(dt);
	}
	
	return true;
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
		moveAnimation->SaveAction(jsonObject, saveCharacterAnimStr.c_str(), true);

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

	// Save Sequential Occurrence 
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.SceneEnter").c_str(), occ_SceneEnter);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.ObjectClicked").c_str(), occ_ObjectClicked);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.BlackboardCondition").c_str(), occ_blackboardValue);

	SaveSequentialConditions(string(serializeObjectName + "Sequential_BlackboardConditions."), jsonObject);

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
	idleAnimation->fromCharacter = true; 

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		idleAnimation->AttachToImage(fixedImageAction);

	// Walk Animation -------------------------------------
	moveAnimation = new DisplayAnimationAction(this);
	moveAnimation->SetIsSequencial(false);
	moveAnimation->canChangeCanvas = false; 
	moveAnimation->fromCharacter = true;

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		moveAnimation->AttachToImage(fixedImageAction);

	// Talk Animation -------------------------------------
	talkAnimation = new DisplayAnimationAction(this);
	talkAnimation->SetIsSequencial(false);
	talkAnimation->canChangeCanvas = false;
	talkAnimation->fromCharacter = true;

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		talkAnimation->AttachToImage(fixedImageAction);

	// Assign idle playing animation by deafault
	playingAnimation = idleAnimation; 
}

void FlyObjectCharacter::PlayCharacterAnimation(CharacterMode characterMode)
{
	if (playingAnimation != nullptr)
		playingAnimation->Stop();

	switch (characterMode)
	{
	case CHARACTER_IDLE:
		playingAnimation = idleAnimation;
		break;
	case CHARACTER_MOVE:
		playingAnimation = moveAnimation;
		break;
	case CHARACTER_TALK:
		playingAnimation = talkAnimation;
		break;
	}

	if (playingAnimation != nullptr)
		playingAnimation->Play(); 
}

void FlyObjectCharacter::OnSceneEnter()
{
	// Play Idle Anim by Def
	PlayCharacterAnimation(CHARACTER_IDLE); 

	// Check Start Sequential 
	if (occ_SceneEnter)
	{
		if (currentSequentialAction == nullptr && !sequentialActionsList.empty())
		{
			currentSequentialAction = sequentialActionsList.front();
		}
	}
}

DisplayAnimationAction* FlyObjectCharacter::GetWalkAnimation()
{
	return moveAnimation;
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

