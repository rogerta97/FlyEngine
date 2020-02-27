#include "DisplayAnimationAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"

DisplayAnimationAction::DisplayAnimationAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_ANIMATION;
	isVisual = false;
	parentObject = _parentObject; 

	SetActionName("Display Animation");
	SetToolDescription("This should be the description of the animation action");
}

DisplayAnimationAction::~DisplayAnimationAction()
{
}

void DisplayAnimationAction::Init()
{
}

void DisplayAnimationAction::Update()
{
}

void DisplayAnimationAction::CleanUp()
{
}

void DisplayAnimationAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr)
{
	string actionSerializeSection;

	if (!literalStr)
		actionSerializeSection = serializeObjectString + string("Actions.DisplayAnimation.");
	else
		actionSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, actionSerializeSection);
	Action::SaveOccurrence(jsonObject, actionSerializeSection);
}

void DisplayAnimationAction::DrawUISettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Preview:");
	ImGui::PopFont();

	ImGui::Separator();
	ImGui::Spacing();

	float squareSize = 200;
	ImGui::Columns(2); 
	ImGui::SetColumnWidth(0, squareSize + 5);
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationPreview", ImVec2(squareSize, squareSize));

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::NextColumn(); 

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Image Info:");
	ImGui::PopFont();

}
