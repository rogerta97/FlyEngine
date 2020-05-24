#include "EmitSoundAction.h"
#include "AudioClip.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"

#include "mmgr.h"

#include "Application.h"
#include "ModuleImGui.h"

#include "SDL_Mixer/SDL_mixer.h"

EmitSoundAction::EmitSoundAction(FlyObject* _parentObject)
{
	actionType = ACTION_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;
	audioPlayMode = ONE_TIME; 

	SetActionName("Emit Sound");
	SetToolDescription("This should be the description of the emit sound");
}

EmitSoundAction::~EmitSoundAction()
{

}

void EmitSoundAction::Update(float dt)
{
	if (isPlaying && !Mix_Playing(audioChannel))
	{
		flog("Channel Stop Playing: %d", audioChannel);
		audioChannel = -1;
		isPlaying = false;
		SetActionCompleted(true);
	}
}

void EmitSoundAction::DrawActionOccurenceCheckboxes()
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
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 130));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
		ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Scene Leave", &occ_SceneLeave);
		ImGui::SetCursorPos(ImVec2(5, 65));
		ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);
		ImGui::SetCursorPos(ImVec2(5, 95));
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


void EmitSoundAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Emit Sound Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawActionOccurenceCheckboxes(); 

		IMGUI_SPACED_SEPARATOR; 

		static int playModeSelected = 0; 
		if (ImGui::Combo("Play Mode##AudioPlayMode", &playModeSelected, "One Time\0Loop\0"))
		{
			audioPlayMode = (AudioPlayMode)playModeSelected; 
		}

		static char soundNameBuffer[256] = "";

		if (audioClip != nullptr)
			strcpy(soundNameBuffer, audioClip->GetName().c_str());

		Texture* buttonIcon = nullptr; 
		
		if(!isPlaying)
			buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
		else
			buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");

		if (ImGui::ImageButton((ImTextureID)buttonIcon->GetTextureID(), ImVec2(20, 20)))
		{
			if (!isPlaying)
				Play();
			else
				Stop(); 
		}
			
		ImGui::SameLine();

		ImGui::InputTextWithHint("", "Select Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
			{
				int* selectedResourceUID = (int*)payload->Data;
				Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

				if (resourceDropped->GetType() == RESOURCE_SFX)
				{
					audioClip = (AudioClip*)resourceDropped;
					strcpy(soundNameBuffer, resourceDropped->GetName().c_str());
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		if (ImGui::Button("Search##SearchSound"))
		{
			ImGui::OpenPopup("print_sound_selection_popup");
			showSoundSelectionPopup = true;
		}

		if (showSoundSelectionPopup)
		{
			Resource* selectedSound = ResourceManager::getInstance()->PrintSoundsSelectionPopup();

			if (selectedSound != nullptr)
			{
				audioClip = (AudioClip*)selectedSound;
				showSoundSelectionPopup = false;
				strcpy(soundNameBuffer, selectedSound->GetName().c_str());
			}
		}
	}	
}

void EmitSoundAction::DrawUISettingsInButton()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Emit Sound Settings: ");
	ImGui::PopFont();

	static char soundNameBuffer[256] = "";

	if (audioClip != nullptr)
		strcpy(soundNameBuffer, audioClip->GetName().c_str());

	Texture* playSound = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
	if (ImGui::ImageButton((ImTextureID)playSound->GetTextureID(), ImVec2(20, 20)))
		Play();
	
	ImGui::SameLine();

	ImGui::InputTextWithHint("", "Select Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
		{
			int* selectedResourceUID = (int*)payload->Data;
			Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

			if (resourceDropped->GetType() == RESOURCE_SFX)
			{
				audioClip = (AudioClip*)resourceDropped;
				strcpy(soundNameBuffer, resourceDropped->GetName().c_str());
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();

	if (ImGui::Button("Search##SearchSound"))
	{
		ImGui::OpenPopup("print_sound_selection_popup");
		showSoundSelectionPopup = true;
	}

	if (showSoundSelectionPopup)
	{
		Resource* selectedSound = ResourceManager::getInstance()->PrintSoundsSelectionPopup();

		if (selectedSound != nullptr)
		{
			audioClip = (AudioClip*)selectedSound;
			showSoundSelectionPopup = false;
			strcpy(soundNameBuffer, selectedSound->GetName().c_str());
		}
	}
}

void EmitSoundAction::Play()
{
	if (audioClip && !isPlaying)
	{
		if (audioPlayMode == AUDIO_LOOP)
		{
			audioChannel = audioClip->Play(-1);
			SetActionCompleted(true);
		}
		else
			audioChannel = audioClip->Play();

		isPlaying = true; 
		flog("Channel Begin Playing: %d", audioChannel);
	}	
}

void EmitSoundAction::Stop()
{
	if (isPlaying)
	{
		Mix_HaltChannel(audioChannel);
		isPlaying = false; 
		audioChannel = -1;
		SetActionCompleted(true);
	}
}

void EmitSoundAction::SaveAction(JSON_Object* jsonObject, std::string serializeStr, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection; 
	
	if (!literalStr)
		toolsSerializeSection = serializeStr + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeStr;

	Action::SaveAction(jsonObject, toolsSerializeSection); 
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
