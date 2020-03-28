#include "DialogAction.h"
#include "imgui.h"

#include "mmgr.h"

DialogAction::DialogAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOGUE;
	isVisual = false;

	SetActionName("Dialog");
	SetToolDescription("This should be the description of the dialog");
}

DialogAction::~DialogAction()
{
}

void DialogAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Dialogue Settings:"))
	{
		if (ImGui::Button("Open Dialogue Editor"))
		{

		}
	}
}
