#include "FollowPathAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"

using namespace ImGui; 

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_FOLLOW_PATH;
	parentObject = _parentObject;
	isVisual = false;

	SetActionName("Follow Path");
	SetToolDescription("This should be the description of follow path action");
}

FollowPathAction::~FollowPathAction()
{

}

void FollowPathAction::DrawPath()
{
}

void FollowPathAction::DrawUISettings()
{
	if (CollapsingHeader("Follow Path Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}

void FollowPathAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.FollowPath.");
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);
}
