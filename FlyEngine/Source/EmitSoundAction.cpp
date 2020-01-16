#include "EmitSoundAction.h"
#include "AudioClip.h"
#include "imgui.h"

#include "Application.h"
#include "ModuleImGui.h"

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

void EmitSoundAction::DrawActionOccurenceCheckboxes()
{
	
}

void EmitSoundAction::Play()
{
	audioClip->Play(); 
}

void EmitSoundAction::SaveAction(JSON_Object* jsonObject, std::string serializeStr)
{
	string toolsSerializeSection = serializeStr + string("Actions.EmitSound.");
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	if (audioClip != nullptr)	
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Path")).c_str(), audioClip->GetPath());
	else
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Path")).c_str(), "None");
	 
}

void EmitSoundAction::DoAction()
{
	Play(); 
}
