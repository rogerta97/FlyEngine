#include "EmitSoundAction.h"

EmitSoundAction::EmitSoundAction(FlyObject* _parentObject)
{
	actionType = AT_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;

	SetActionName("Emit Sound");
	SetToolDescription("This should be the description of the emit sound");
}

EmitSoundAction::~EmitSoundAction()
{

}

void EmitSoundAction::SaveAction(JSON_Object* jsonObject, std::string serializeStr)
{
}

void EmitSoundAction::DoAction()
{
}
