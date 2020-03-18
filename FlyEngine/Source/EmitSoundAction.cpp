#include "EmitSoundAction.h"
#include "AudioClip.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"

#include "mmgr.h"

#include "Application.h"
#include "ModuleImGui.h"

EmitSoundAction::EmitSoundAction(FlyObject* _parentObject)
{
	actionType = ACTION_EMIT_SOUND;
	parentObject = _parentObject;
	isVisual = false;
	acceptSequencial = true;

	SetActionName("Emit Sound");
	SetToolDescription("This should be the description of the emit sound");
}

EmitSoundAction::~EmitSoundAction()
{

}

void EmitSoundAction::DrawUISettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Emit Sound Settings: ");
	ImGui::PopFont();

	static char soundNameBuffer[256] = "";

	if(audioClip != nullptr)
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

void EmitSoundAction::DrawActionOccurenceCheckboxes()
{
	
}

void EmitSoundAction::Play()
{
	if(audioClip)
		audioClip->Play(); 
}

void EmitSoundAction::SaveAction(JSON_Object* jsonObject, std::string serializeStr, bool literalStr)
{
	string toolsSerializeSection; 
	
	if (!literalStr)
		toolsSerializeSection = serializeStr + string("Actions.EmitSound.");
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
	SetActionCompleted(true); 
}
