#include "DisplayAnimationAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "MyFileSystem.h"
#include "Animation.h"

DisplayAnimationAction::DisplayAnimationAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_ANIMATION;
	isVisual = false;
	parentObject = _parentObject; 

	SetActionName("Display Animation");
	SetToolDescription("This should be the description of the animation action");

	animation = new Animation(); 
	//animation->BuildAnimation(string(MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Animations\\TestAnim_001").c_str());

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
	delete animation; 
	animation = nullptr; 
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

	static char animationNameBuffer[256]; 

	if (animation != nullptr)
	{
		strcpy(animationNameBuffer, animation->GetName().c_str());
	}

	if (ImGui::InputTextWithHint("Name", "Name...", animationNameBuffer, IM_ARRAYSIZE(animationNameBuffer)))
	{
		animation->SetName(animationNameBuffer); 
	}

	int a = 0; 

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::Text("Frames Amount:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "0");
	ImGui::PopFont();
}

Animation* DisplayAnimationAction::GetAnimation()
{
	return animation;
}
