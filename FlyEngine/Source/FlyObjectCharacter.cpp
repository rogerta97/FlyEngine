#include "FlyObjectCharacter.h"
#include "DisplayAnimationAction.h"
#include "DisplayImageAction.h"
#include "Room.h"

FlyObjectCharacter::FlyObjectCharacter(std::string objectName, std::string description, FlyObjectType _flyObjectType, Room* parentRoom) : FlyObject(objectName, description,  _flyObjectType, parentRoom)
{
	flyObjectType = OBJECT_SEQUENTIAL; 
}

FlyObjectCharacter::FlyObjectCharacter()
{
}

void FlyObjectCharacter::InitCharacter()
{
	DisplayImageAction* fixedImageAction = AddDisplayImageAction("Null", false);

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

	if (fixedImageAction == nullptr)
		fixedImageAction = AddDisplayImageAction("Null", false);

	fixedImageAction->fromAnimation = true;

	if (fixedImageAction != nullptr)
		walkAnimation->AttachToImage(fixedImageAction);

	// Talk Animation -------------------------------------
	talkAnimation = new DisplayAnimationAction(this);
	talkAnimation->SetIsSequencial(false);

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

