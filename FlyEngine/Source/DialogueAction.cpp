#include "DialogueAction.h"
#include "imgui.h"
#include "Dialogue.h"
#include "DialogueStep.h"
#include "DisplayTextAction.h"
#include "DialogueText.h"
#include "StepAnswer.h"

#include "DialogueViewportHandler.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "mmgr.h"


DialogueAction::DialogueAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOGUE;
	isVisual = false;
	showVariableConditions = false; 

	sentencePlacementCombo = 0;
	fontSize = 0; 
	fontColor = float4::zero; 
	dialoguePlaying = false; 

	SetActionName("Dialog");
	SetToolDescription("This should be the description of the dialog");

	dialogue = new Dialogue(); 
}

DialogueAction::~DialogueAction()
{
}

void DialogueAction::Update(float dt)
{
	if (dialoguePlaying)
	{
		DialogueStep* currentStep = dialogue->dialogueViewportHandler->GetCurrentStep();
		StepAnswer* clickedAnswer = currentStep->ListenAnswerClick();

		if (clickedAnswer != nullptr)
		{
			flog("Congrats, you have clicked the answer %s", clickedAnswer->GetName().c_str()); 
		}
	}
}

void DialogueAction::Draw()
{
	if(dialogue != nullptr)
		dialogue->Draw(); 
}

void DialogueAction::DoAction()
{
	if (dialogue != nullptr)
	{
		DialogueStep* dialogueFirstStep = dialogue->GetFirstStep();

		if (dialogueFirstStep != nullptr)
		{
			dialogue->dialogueViewportHandler->SetCurrentStep(dialogueFirstStep);
			dialoguePlaying = true; 
		}
	}
}

void DialogueAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string actionSerializeSection;

	if (!literalStr)
		actionSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		actionSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, actionSerializeSection);
	Action::SaveOccurrence(jsonObject, actionSerializeSection);

	json_object_dotset_number(jsonObject, string(actionSerializeSection + "NodeGraphID").c_str(), dialogue->GetUID());

	// Save Steps Data 
	string serialiseStepStr = actionSerializeSection + "Steps.";
	json_object_dotset_number(jsonObject, string(serialiseStepStr + "StepsAmount").c_str(), dialogue->GetDialogueSteps().size()); 

	int counter = 0;
	for (auto& currentStep : dialogue->GetDialogueSteps())
	{
		string stepStr = serialiseStepStr + "Step_" + to_string(counter) + ".";
		currentStep->SaveStep(jsonObject, stepStr);
		counter++;
	}

}

void DialogueAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Dialogue Settings:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawActionOccurenceCheckboxes();
		DialogueStep* selectedStep = dialogue->GetSelectedStep();

		ImGui::Separator();
		if (selectedStep)
		{
			DrawStepVisualSettings(selectedStep);
			
			ImGui::Spacing(); 
			DrawAnswersVisualSettings(selectedStep);

			// Preview Button 
			string buttonPreviewText = "Show Preview"; 

			if (showPreview == true)
				buttonPreviewText = "Hide Preview"; 

			if (ImGui::Button(buttonPreviewText.c_str(), ImVec2(100, 35)))
			{
				if(showPreview)
					dialogue->dialogueViewportHandler->SetCurrentStep(nullptr);
				else
					dialogue->dialogueViewportHandler->SetCurrentStep(selectedStep);

				showPreview = !showPreview; 
			}
		
			IMGUI_SPACED_SEPARATOR;
		}

		if (drawFontPopup)
		{
			Font* selectedFont = (Font*)ResourceManager::getInstance()->PrintFontSelectionPopup();
			if (selectedFont != nullptr && fontToSentence)
			{
				selectedStep->fontNameHold = selectedFont->GetName(); 
			}
			else if (selectedFont != nullptr && fontToAnswer)
			{
				selectedStep->answerFontNameHold = selectedFont->GetName();
			}
		}


		// Draw Step Settings -------------------------------------------------------------------------------------
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Dialogue Steps");
		ImGui::PopFont();

		PUSH_CHILD_BG_COLOR_DARK;
		ImGui::BeginChild("Steps Dialogue Holder", ImVec2(ImGui::GetContentRegionAvail().x, 150)); 

		INC_CURSOR_7;
		ImGui::BeginChild("Steps Dialogue Holder Inside", ImVec2(ImGui::GetContentRegionAvail().x - 8, 140));
		for (auto& currentStep : dialogue->GetDialogueSteps())
		{
			string selectableName = currentStep->GetName() + "##" + to_string(currentStep->GetUID());
			if (ImGui::Selectable(string(" " + selectableName).c_str(), currentStep->isSelected))
			{
				dialogue->SetSelectedStep(currentStep->GetUID());
			}
		}

		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::PopStyleColor(); 

		Texture* plusTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite2"); 
		if (ImGui::ImageButton((ImTextureID)plusTexture->GetTextureID(), ImVec2(35, 35)))
		{
			DialogueStep* newStep = AddDialogueStep("This Is a Sentence"); 
		}

		ImGui::Separator(); 

		if (selectedStep != nullptr)
		{
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Step Settings");
			ImGui::PopFont();

			char stepNameBuffer[256] = "";
			if (!selectedStep->GetName().empty())
				strcpy(stepNameBuffer, selectedStep->GetName().c_str());

			if (ImGui::InputText("Name", stepNameBuffer, IM_ARRAYSIZE(stepNameBuffer)))
			{
				selectedStep->SetName(stepNameBuffer);
			}

			static char stepTextBuffer[256] = "";
			if (!selectedStep->GetTextStr().empty())
				strcpy(stepTextBuffer, selectedStep->GetTextStr().c_str());

			if (ImGui::InputTextMultiline("Description##ObjectDescription", stepTextBuffer, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100)))
			{
				selectedStep->SetText(stepTextBuffer);
			}

			ImGui::Separator();
			
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Answers List:");
			ImGui::PopFont();

			PUSH_CHILD_BG_COLOR;
			ImGui::BeginChild("Answers Dialogue Holder", ImVec2(ImGui::GetContentRegionAvail().x, 200));

			INC_CURSOR_7;
			ImGui::BeginChild("Answers Dialogue Holder Holderdd", ImVec2(ImGui::GetContentRegionAvail().x - 8, 190));
			for (auto& currentAnswer : selectedStep->GetAnswersList())
			{
				string headerName = currentAnswer->GetName();
				string headerNameWithUID = currentAnswer->GetName() +"##" + to_string(currentAnswer->GetUID());
				if (ImGui::CollapsingHeader(headerNameWithUID.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					static char answerNameBuffer[256] = "";

					if(!headerName.empty())
						strcpy(answerNameBuffer, headerName.c_str());

					ImGui::Indent(15);
					if (ImGui::InputText(string("Name##" + to_string(currentAnswer->GetUID())).c_str(), answerNameBuffer, IM_ARRAYSIZE(answerNameBuffer)))
					{
						currentAnswer->SetName(answerNameBuffer); 
					}
				
					string answerText = currentAnswer->GetAnswerDialogueText()->GetTextAction()->GetText();
					string answerTextWithUID = currentAnswer->GetAnswerDialogueText()->GetTextAction()->GetText() + "##" + to_string(currentAnswer->GetUID());

					static char answerTextBuffer[256] = "";

					if (!answerText.empty())
						strcpy(answerTextBuffer, answerText.c_str());

					if (ImGui::InputTextMultiline(string("Text##" + to_string(currentAnswer->GetUID())).c_str(), answerTextBuffer, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100)))
					{
						currentAnswer->SetAnswerText(answerTextBuffer);
					}
					ImGui::Indent(-15);
					ImGui::Spacing(); 
				}
			}

			ImGui::EndChild();
			ImGui::EndChild();
			ImGui::PopStyleColor();
		
			if(ImGui::Button("New Answer"))
			{
				if (selectedStep != nullptr)
				{
					StepAnswer* sa = selectedStep->AddStepAnswer("Test");
					dialogue->answersMap.insert(std::make_pair(sa->GetUID(), sa)); 		
					dialogue->dialogueViewportHandler->AddaptAnswersPosition();
					
				}
			}
		}

	}
}

void DialogueAction::DrawAnswersVisualSettings(DialogueStep* selectedStep)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Answers Visual Settings:");
	ImGui::PopFont();

	string answerButtonString = "Find##FindFontAnswer";
	if (ImGui::Button(answerButtonString.c_str()))
	{
		drawFontPopup = true; 
		fontToSentence = false; 
		fontToAnswer = true; 

		if (selectedStep != nullptr)
			ImGui::OpenPopup("print_font_selection_popup");
	}

	char actionFontNameBuffer[256] = "None";
	if (selectedStep != nullptr && selectedStep->answerFontNameHold != "")
	{
		strcpy(actionFontNameBuffer, selectedStep->answerFontNameHold.c_str());
	}

	ImGui::SameLine();
	if (ImGui::InputText("Font", actionFontNameBuffer, IM_ARRAYSIZE(actionFontNameBuffer), ImGuiInputTextFlags_ReadOnly))
	{

	}

	// Text Color --------------------------------
	float4 textColor = float4::zero;
	if (selectedStep != nullptr && selectedStep->GetDialogueText()->GetTextAction() != nullptr)
		textColor = selectedStep->answerFontColorHold;

	if (ImGui::ColorEdit4("Text Color Answer", (float*)& textColor))
	{
		if (selectedStep != nullptr && selectedStep->GetDialogueText()->GetTextAction() != nullptr)
		{
			selectedStep->answerFontColorHold = textColor;
		}
	}

	// Background Color -------------------------
	float4 backgroundColor = float4::zero;
	if (selectedStep != nullptr && dialogue->dialogueViewportHandler != nullptr)
		backgroundColor = selectedStep->answerBackgroundColorHold;

	if (ImGui::ColorEdit4("Background Color Answer", (float*)& backgroundColor))
	{
		if (selectedStep != nullptr && dialogue->dialogueViewportHandler != nullptr)
		{
			selectedStep->answerBackgroundColorHold = backgroundColor;
			//dialogue->dialogueViewportHandler->GetSentenceBackgroundBB()->SetSquareColor(backgroundColor); 
		}
	}

}

void DialogueAction::DrawStepVisualSettings(DialogueStep* selectedStep)
{
	string stepSettingsStr = "Step Visual Settings [";

	ImGui::PushFont(App->moduleImGui->rudaBoldBig);

	if (selectedStep == nullptr)
	{
		ImGui::Text(stepSettingsStr.c_str()); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "None Selected"); ImGui::SameLine();
		ImGui::Text("]");
	}
	else
	{
		stepSettingsStr += string(selectedStep->GetName() + ']');
		ImGui::Text(stepSettingsStr.c_str());
	}

	ImGui::PopFont();

	// Display Mode ------------------------------------------------
	ImGui::Spacing(); 
	
	if (selectedStep != nullptr)
	{
		int displayMode = 0;

		// Draw Display Mode -------------------------------------------------------
		if (selectedStep != nullptr)
			displayMode = selectedStep->sentenceDisplayMode;

		if (ImGui::Combo("Sentence Placement", &displayMode, "Display On Top Screen\0Display Over Answers"))
		{
			if (selectedStep != nullptr)
				selectedStep->sentenceDisplayMode = (Sentence_Display_Mode)displayMode;
		}

		// Draw Font Button -------------------------------------------------------
		string buttonString = "Find##FindFont";
		if (ImGui::Button(buttonString.c_str()))
		{
			drawFontPopup = true;

			fontToSentence = true;
			fontToAnswer = false;

			if (selectedStep != nullptr)
				ImGui::OpenPopup("print_font_selection_popup");
		}

		// Draw Font Name -------------------------------------------------------
		char actionFontNameBuffer[256] = "None";
		if (!selectedStep->fontNameHold.empty())
		{
			strcpy(actionFontNameBuffer, selectedStep->fontNameHold.c_str());
		}

		ImGui::SameLine();
		if (ImGui::InputText("Font", actionFontNameBuffer, IM_ARRAYSIZE(actionFontNameBuffer), ImGuiInputTextFlags_ReadOnly))
		{

		}

		// Text Color ------------------------------------------------
		float4 textColor = float4::zero;
		if (selectedStep != nullptr && selectedStep->GetDialogueText()->GetTextAction() != nullptr)
			textColor = selectedStep->fontColorHold;

		if (ImGui::ColorEdit4("Text Color", (float*)&textColor))
		{
			if (selectedStep != nullptr && selectedStep->GetDialogueText()->GetTextAction() != nullptr)
			{
				selectedStep->GetDialogueText()->GetTextAction()->SetTextColor(textColor);
				selectedStep->fontColorHold = textColor;
			}
		}

		// Background Color ------------------------------------------------
		float4 backgroundColor = float4::zero;
		if (selectedStep != nullptr && dialogue->dialogueViewportHandler != nullptr)
			backgroundColor = selectedStep->backgroundColorHold;

		if (ImGui::ColorEdit4("Background Color", (float*)&backgroundColor))
		{
			if (selectedStep != nullptr && dialogue->dialogueViewportHandler != nullptr)
			{
				selectedStep->backgroundColorHold = backgroundColor;
				//dialogue->dialogueViewportHandler->GetSentenceBackgroundBB()->SetSquareColor(backgroundColor); 
			}
		}
	}
}

void DialogueAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	if (GetActionClass() == ACTION_CLASS_SEQUENTIAL)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}
	else if (GetActionClass() == ACTION_CLASS_DIRECT)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
		ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);
		ImGui::SetCursorPos(ImVec2(5, 65));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}

	ImGui::SameLine();
	static std::string showValueConditionButtonText = "Show Conditions";
	if (ImGui::Button(showValueConditionButtonText.c_str()))
	{
		if (showVariableConditions)
		{
			showVariableConditions = false;
			showValueConditionButtonText = "Show Conditions";
		}
		else
		{
			showVariableConditions = true;
			showValueConditionButtonText = "Hide Conditions";
		}
	}

	ImGui::EndChild();
	ImGui::Spacing();

	if (showVariableConditions)
		DrawActionConditionsList();

	ImGui::PopStyleColor();

}

DialogueStep* DialogueAction::AddDialogueStep(string _dialogueSlotText, string _dialogueStepName)
{
	return dialogue->AddDialogueStep(_dialogueSlotText, _dialogueStepName);
}

Dialogue* DialogueAction::GetDialogueData()
{
	return dialogue;
}
