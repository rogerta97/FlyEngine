#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWorldManager.h"
#include "ModuleManager.h"
#include "ModuleInput.h"
#include "ResourceManager.h"
#include "ObjectCreatorDockPanel.h"
#include "CharacterCreatorDockPanel.h"
#include "ModuleImGui.h"
#include "AudioClip.h"

#include "UI_Image.h"
#include "ModuleAudioManager.h"
#include "UI_Button.h"
#include "RoomDockPanel.h"
#include "RoomUIHandler.h"
#include "GameInventory.h"
#include "ImageImporter.h"
#include "EmitSoundAction.h"
#include "TinyFileDialog.h"
#include "SaveAndLoad.h"
#include "MyFileSystem.h"
#include "MusicTrack.h"

#include "Room.h"
#include "UI_Text.h"
#include "Texture.h"
#include "FlyVariable.h"
#include "ViewportManager.h"
#include "FlyObject.h"
#include "mmgr.h"

RoomDockPanel::RoomDockPanel(bool isVisible) : DockPanel("Room", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_SCENE;
	playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayIcon");
}

RoomDockPanel::~RoomDockPanel()
{

}

bool RoomDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw() || App->moduleWorldManager->GetSelectedRoom() == nullptr)
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		DrawTopButtons();

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Room Object List"))
			{
				DrawRoomHierarchy();

				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_OBJECTS;
				App->moduleWorldManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 1));
				SaveAndLoad::getInstance()->SaveInitFile();
			}

			if (ImGui::BeginTabItem("User Interface"))
			{
				DrawUserInterfaceTab();
				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_UI;
				App->moduleWorldManager->GetSelectedRoom()->SetSelectedObject(nullptr); 
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 0.5f));
				SaveAndLoad::getInstance()->SaveInitFile();
			}

			if (ImGui::BeginTabItem("Room Settings"))
			{
				ShowViewportSettingsTab();
				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_OBJECTS;
				App->moduleWorldManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 1));
			}

			if (ImGui::BeginTabItem("Blackboard"))
			{
				ShowBlackboardTab();
				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_OBJECTS;
				App->moduleWorldManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 1));
				SaveAndLoad::getInstance()->SaveInitFile();
			}


			ImGui::EndTabBar();
		}
	}

	ImGui::End();
	return true;
}

void RoomDockPanel::DrawUserInterfaceTab()
{
	if (ViewportManager::getInstance()->editRoomMode == EDIT_ROOM_UI)
	{
		if(ImGui::CollapsingHeader("Settings##UISettings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static float color[4];
			color[0] = App->moduleManager->GetSceneUITint().x;
			color[1] = App->moduleManager->GetSceneUITint().y;
			color[2] = App->moduleManager->GetSceneUITint().z;
			color[3] = App->moduleManager->GetSceneUITint().w;
			
			if (ImGui::ColorEdit4("Background Tint", color))
				App->moduleManager->SetSceneUITint(float4(color[0], color[1], color[2], color[3]));
			
			IMGUI_SPACED_SEPARATOR;
		}

		RoomUIHandler* roomUIHandler = App->moduleWorldManager->GetSelectedRoom()->roomUIHandler;

		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("User Interface Objects:");
		ImGui::PopFont();

		PUSH_CHILD_BG_COLOR;
		ImGui::BeginChild("UIHierarchy", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50));

		UID selectedObjectUID = roomUIHandler->DrawUIElementsHierarchy();

		if (selectedObjectUID != 0)
			roomUIHandler->SetSelectedElement(selectedObjectUID);
		
		ImGui::EndChild();
		ImGui::PopStyleColor();

		Texture* plusIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
		if (ImGui::ImageButton((ImTextureID)plusIcon->GetTextureID(), ImVec2(35, 35)))
		{
			ImGui::OpenPopup("SelectUIElement");
		}

		UIElementType selectedType = roomUIHandler->DrawUIElementSelectorPopup();

		if (selectedType != UI_null)
			roomUIHandler->AddEmptyElement(selectedType);

		ImGui::SameLine();
		Texture* minusIcon = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
		if (ImGui::ImageButton((ImTextureID)minusIcon->GetTextureID(), ImVec2(35, 35)))
		{
			if (roomUIHandler->GetSelectedElement() != nullptr)
			{
				roomUIHandler->DeleteElement(roomUIHandler->GetSelectedElement()->GetUID());
				roomUIHandler->SetSelectedElement(nullptr);
			}

		}


	}
}

void RoomDockPanel::ShowBlackboardTab()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Blackboard Variables:");
	POP_FONT;

	// Search Bar --------------------
	static char searchVariableBuffer[256];
	ImGui::InputTextWithHint("", "Search...", searchVariableBuffer, IM_ARRAYSIZE(searchVariableBuffer));

	ImGui::SameLine();
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(20, 20)))
	{
		App->moduleWorldManager->GetSelectedRoom()->GetBlackboard()->AddDefaultVariable();
	}

	// Variable List UI --------------
	DrawRoomVariablesUI();
}

void RoomDockPanel::DrawRoomVariablesUI()
{
	Blackboard* roomBB = App->moduleWorldManager->GetSelectedRoom()->GetBlackboard();

	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("BlackboardChild", ImVec2(ImGui::GetContentRegionMax().x - 5, ImGui::GetContentRegionAvail().y));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, (ImGui::GetCursorPos().y + 5)));

	int counter = 0;
	for (auto& currentVar : roomBB->blackboardVariablesList)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, (ImGui::GetCursorPos().y + 5)));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.28f, 0.43f, 0.56, 0.2f));

		string varChildID = "VariableUIGroup" + to_string(counter);
		ImGui::BeginChild(varChildID.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 5, 103));

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 70);

		int iconSize = 55;
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 4, (ImGui::GetContentRegionAvail().y / 2) - iconSize / 2));
		Texture* variableType = nullptr;

		if (currentVar->varType == Var_Integer)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
		else if (currentVar->varType == Var_Toggle)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

		ImGui::Image((ImTextureID*)variableType->GetTextureID(), ImVec2(iconSize, iconSize));

		ImGui::NextColumn();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 28);
		Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("DeleteIcon");

		if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(20, 20)))
		{

		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 27);

		char nameBuffer[256];
		strcpy(nameBuffer, currentVar->name.c_str());
		string nameStringID = "Name##VarName" + to_string(counter);

		if (ImGui::InputTextWithHint(nameStringID.c_str(), "Attach Variable...", nameBuffer, IM_ARRAYSIZE(nameBuffer)))
		{
			currentVar->name = nameBuffer;
		}

		string comboStringID = "Variable Type##VarType" + to_string(counter);
		int currentItemType = currentVar->varType;
		ImGui::Combo(comboStringID.c_str(), &currentItemType, "Integer\0Toggle\0");

		string valueStringID = "Value##ComboVar" + to_string(counter);
		switch (currentItemType)
		{

		case 0:
		{
			ImGui::InputInt(valueStringID.c_str(), &currentVar->varIntegerValue);
			currentVar->varType = Var_Integer;

			break;
		}

		case 1:
		{
			ImGui::Checkbox(valueStringID.c_str(), &currentVar->varToogleValue);
			currentVar->varType = Var_Toggle;
			break;
		}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		counter++;
	}

	ImGui::PopStyleColor();

	ImGui::EndChild();
}

void RoomDockPanel::DrawTopButtons()
{
	// Create Object Button ---
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.29f, 0.35f, 0.39f, 1.00f));
	static ImVec4 playButtonColor = ImVec4(0.35f, 0.39f, 0.50f, 1.00f);
	Texture* plusObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusObjectTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		App->moduleImGui->objectCreatorDockPanel->ResetObjectData();
		App->moduleImGui->objectCreatorDockPanel->ToggleVisibility();
	}

	// Create Character Button ---
	Texture* characterCreatorIcon = (Texture*)ResourceManager::getInstance()->GetResource("AddCharacterIcon");
	ImGui::SameLine(); 
	if (ImGui::ImageButton((ImTextureID)characterCreatorIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		App->moduleImGui->characterCreatorDockPanel->ResetObjectData();
		App->moduleImGui->characterCreatorDockPanel->ToggleVisibility();
	}

	// Save Button ------------
	ImGui::SameLine();
	Texture* exportTexture = (Texture*)ResourceManager::getInstance()->GetResource("SaveIcon");
	if (ImGui::ImageButton((ImTextureID)exportTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		SaveAndLoad::getInstance()->SaveCurrentRoomData();
	}

	// Load Button ------------
	ImGui::SameLine();
	Texture* importIcon = (Texture*)ResourceManager::getInstance()->GetResource("ExportIcon");
	if (ImGui::ImageButton((ImTextureID)importIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		char const* lFilterPatterns[1] = { "*.json" };
		string defaultPath = MyFileSystem::getInstance()->GetSolutionDirectory();
		defaultPath += "Source\\Game\\Resources\\EngineSavedData\\RoomsData\\";
		string path = tinyfd_openFileDialog("Load Image...", defaultPath.c_str(), 1, lFilterPatterns, NULL, 0);

		if (path != "")
		{
			// Clean Current Room Content
			Room* currentRoom = App->moduleWorldManager->GetSelectedRoom();
			currentRoom->CleanUp();

			// Load New Data
			SaveAndLoad::getInstance()->LoadDataToSelectedRoom(path);
		}
	}

	// Play Button ------------
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, playButtonColor);
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 37);
	if (ImGui::ImageButton((ImTextureID)playStopButtonTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		App->isEngineInPlayMode = !App->isEngineInPlayMode;

		if (App->isEngineInPlayMode)
		{
			App->BroadCastEvent(FlyEngineEvent::ENGINE_PLAY);
			playButtonColor = ImVec4(0.50f, 0.50f, 0.80f, 1.0f);
			playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");
			SaveAndLoad::getInstance()->SaveSelectedRoomToOnPlayData();
			App->BroadCastEvent(FlyEngineEvent::ENTER_ROOM);
		}
		else
		{
			App->BroadCastEvent(FlyEngineEvent::LEAVE_ROOM);

			App->BroadCastEvent(FlyEngineEvent::ENGINE_STOP);

			playButtonColor = ImVec4(0.35f, 0.39f, 0.50f, 1.00f);
			playStopButtonTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayIcon");

			SaveAndLoad::getInstance()->LoadOnPlayData();
		}
	}
	ImGui::PopStyleColor(2);
}

void RoomDockPanel::DrawRoomHierarchy()
{
	static char searchObjectBuffer[256];
	ImGui::InputTextWithHint("", "Search...", searchObjectBuffer, IM_ARRAYSIZE(searchObjectBuffer));

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.5f));

	ImGui::BeginChild("##ObjectsInRoom", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y));

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::Spacing();

	if (App->moduleWorldManager->GetSelectedRoom()->GetObjectsInRoomAmount() == 0)
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
		ImGui::TextColored(ImVec4(1, 1, 1, 0.25), "This Room Is Empty");
		ImGui::PopFont();
	}
	else
	{
		Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
		FlyObject* selectedObject = selectedRoom->GetSelectedObject();

		bool popupOpenThisTick = false;
		for (list<FlyObject*>::reverse_iterator it = selectedRoom->objectsInRoom.rbegin(); it != selectedRoom->objectsInRoom.rend(); it++) 
		{
			PUSH_FONT(App->moduleImGui->rudaBlackMid);

			bool objectSelected = false;

			if (selectedObject != nullptr)
				objectSelected = (selectedObject->GetName() == (*it)->GetName());

			if (ImGui::Selectable((*it)->GetName().c_str(), objectSelected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				(*it)->isSelected = true;
				App->moduleWorldManager->GetSelectedRoom()->SetSelectedObject(*it);
			}

			if (ImGui::IsWindowHovered() && App->moduleInput->GetMouseButton(RIGHT_CLICK) == KEY_DOWN)
			{
				if (!popupOpenThisTick)
				{
					if (ImGui::IsItemHovered())
					{
						(*it)->isSelected = true;
						App->moduleWorldManager->GetSelectedRoom()->SetSelectedObject(*it);
						ImGui::OpenPopup("right_click_item_hierarchy");
						popupOpenThisTick = true;
					}
					else
					{
						ImGui::OpenPopup("right_click_hierarchy");
					}
				}
			}

			POP_FONT;
		}

		bool openRenamePopup = false; 
		if (ImGui::BeginPopup("right_click_item_hierarchy"))
		{
			if (ImGui::Selectable("Rename"))
			{
				ImGui::CloseCurrentPopup();
				openRenamePopup = true; 
			}

			if (ImGui::Selectable("Duplicate"))
			{
				FlyObject* objectToClone = selectedRoom->GetSelectedObject(); 

				if (objectToClone != nullptr)
				{
					FlyObject* newClone = new FlyObject(string(objectToClone->GetName() + " (2)"), objectToClone->GetDescription(), objectToClone->flyObjectType, objectToClone->GetParentRoom());
					objectToClone->GetParentRoom()->AddFlyObject(newClone);
					
					newClone->CopyFlyObjectActions(objectToClone); 
					newClone->FitObjectUtils();
					
					App->moduleWorldManager->GetSelectedRoom()->SetSelectedObject(newClone);
				}

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Selectable("Layer Up"))
			{
				selectedRoom->MoveObjectInList(selectedObject, true);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Selectable("Layer Down"))
			{

				selectedRoom->MoveObjectInList(selectedObject, false);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("right_click_hierarchy"))
		{
			if (ImGui::Selectable("New FlyObject"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		static char newNameBuffer[256] = "";

		if (openRenamePopup)
		{
			ImGui::OpenPopup("rename_popup");
			strcpy(newNameBuffer, ""); 
		}

		if (ImGui::BeginPopup("rename_popup"))
		{
			bool assignName = false; 
			ImGui::InputTextWithHint("", "New Name...", newNameBuffer, IM_ARRAYSIZE(newNameBuffer)); 
			ImGui::SameLine();

			if (App->moduleInput->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->moduleInput->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
				assignName = true; 

			if (ImGui::Button("OK"))
				assignName = true; 

			if (assignName)
			{
				FlyObject* objectToRename = selectedRoom->GetSelectedObject();
				objectToRename->SetName(newNameBuffer); 
				strcpy(newNameBuffer, "");

				ImGui::CloseCurrentPopup(); 
			}

			ImGui::EndPopup(); 
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void RoomDockPanel::ShowViewportSettingsTab()
{

	if (ImGui::CollapsingHeader("Music"))
	{
		Room* currentRoom = App->moduleWorldManager->GetSelectedRoom();

		static char trackNameBuffer[256] = "";

		if (currentRoom->backgroundMusic != nullptr)
			strcpy(trackNameBuffer, currentRoom->backgroundMusic->GetName().c_str());
		else
			strcpy(trackNameBuffer, "");

		ImGui::InputTextWithHint("", "Select Music...", trackNameBuffer, IM_ARRAYSIZE(trackNameBuffer), ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("drag_resource"))
			{
				int* selectedResourceUID = (int*)payload->Data;
				Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

				if (resourceDropped->GetType() == RESOURCE_MUSIC)
				{
					MusicTrack* musicTrackDropped = (MusicTrack*)resourceDropped;
					currentRoom->backgroundMusic = musicTrackDropped;
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		if (ImGui::Button("Search##SearchMusic"))
		{
			ImGui::OpenPopup("print_music_selection_popup");
			showMusicSelectionPopup = true;
		}

		if (showMusicSelectionPopup)
		{
			Resource* selectedSound = ResourceManager::getInstance()->PrintMusicSelectionPopup();

			if (selectedSound != nullptr)
			{
				MusicTrack* audioTrackDropped = (MusicTrack*)selectedSound;
				currentRoom->backgroundMusic = audioTrackDropped;
				showMusicSelectionPopup = false;
			}
		}

		float musicV = App->moduleAudioManager->GetMusicVolume();
		if (ImGui::SliderFloat("Music Volume", &musicV, 0.0f, 100.0f, "%.1f"))
		{
			App->moduleAudioManager->SetMusicVolume(musicV);
		}

		float sfxV = App->moduleAudioManager->GetSFXVolume();
		if (ImGui::SliderFloat("SFX Volume", &sfxV, 0.0f, 100.0f, "%.1f"))
		{
			App->moduleAudioManager->SetSFXVolume(sfxV);
		}
	}

	if (ImGui::CollapsingHeader("Inventory"))
	{
		float4 backgroundInventoryColor = GameInventory::getInstance()->backgroundColor; 
		if(ImGui::ColorEdit4("Background Color", (float*)&backgroundInventoryColor))
			GameInventory::getInstance()->backgroundColor = backgroundInventoryColor;

		float4 slotInventoryColor = GameInventory::getInstance()->slotColor;
		if (ImGui::ColorEdit4("Slot Color", (float*)&slotInventoryColor))		
			GameInventory::getInstance()->slotColor = slotInventoryColor; 
		
		float4 arrowInventoryColor = GameInventory::getInstance()->arrowColor;
		if(ImGui::ColorEdit4("Arrow Color", (float*)&arrowInventoryColor))
			GameInventory::getInstance()->arrowColor = arrowInventoryColor;

		// Open Inventory Sound
		IMGUI_SPACED_SEPARATOR; 
		{
			ImGui::Text("Open Inventory SFX:");
			static char soundNameBuffer[256] = "";

			if (GameInventory::getInstance()->openInventorySFX->audioClip != nullptr)
				strcpy(soundNameBuffer, GameInventory::getInstance()->openInventorySFX->audioClip->GetName().c_str());

			Texture* buttonIcon = nullptr;

			if (!GameInventory::getInstance()->openInventorySFX->IsPlaying())
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
			else
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");

			if (ImGui::ImageButton((ImTextureID)buttonIcon->GetTextureID(), ImVec2(20, 20)))
			{
				if (!GameInventory::getInstance()->openInventorySFX->IsPlaying())
					GameInventory::getInstance()->openInventorySFX->Play();
				else
					GameInventory::getInstance()->openInventorySFX->Stop();
			}

			ImGui::SameLine();

			ImGui::InputTextWithHint("", "Open Inventory Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_SFX)
					{
						GameInventory::getInstance()->openInventorySFX->audioClip = (AudioClip*)resourceDropped;
						SaveAndLoad::getInstance()->SaveInitFile();
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
					GameInventory::getInstance()->openInventorySFX->audioClip = (AudioClip*)selectedSound;
					SaveAndLoad::getInstance()->SaveInitFile();
					showSoundSelectionPopup = false;
					strcpy(soundNameBuffer, GameInventory::getInstance()->openInventorySFX->audioClip->GetName().c_str());
				}
			}
		}
		{
			ImGui::Text("Close Inventory SFX:");
				static char soundNameBuffer[256] = "";

			if (GameInventory::getInstance()->closeInventorySFX->audioClip != nullptr)
				strcpy(soundNameBuffer, GameInventory::getInstance()->closeInventorySFX->audioClip->GetName().c_str());

			Texture* buttonIcon = nullptr;

			if (!GameInventory::getInstance()->closeInventorySFX->IsPlaying())
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
			else
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");

			ImGui::PushID(0);
			if (ImGui::ImageButton((ImTextureID)buttonIcon->GetTextureID(), ImVec2(20, 20)))
			{
				if (!GameInventory::getInstance()->closeInventorySFX->IsPlaying())
					GameInventory::getInstance()->closeInventorySFX->Play();
				else
					GameInventory::getInstance()->closeInventorySFX->Stop();
			}
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::InputTextWithHint("", "Open Inventory Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_SFX)
					{
						GameInventory::getInstance()->closeInventorySFX->audioClip = (AudioClip*)resourceDropped;
						SaveAndLoad::getInstance()->SaveInitFile();
						strcpy(soundNameBuffer, resourceDropped->GetName().c_str());
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();

			if (ImGui::Button("Search##SearchSoundClose"))
			{
				ImGui::OpenPopup("print_sound_selection_popup2");
				showSoundSelectionPopup = true;
			}

			if (showSoundSelectionPopup)
			{
				Resource* selectedSound = ResourceManager::getInstance()->PrintSoundsSelectionPopup2();

				if (selectedSound != nullptr)
				{
					GameInventory::getInstance()->closeInventorySFX->audioClip = (AudioClip*)selectedSound;
					SaveAndLoad::getInstance()->SaveInitFile();
					showSoundSelectionPopup = false;
					strcpy(soundNameBuffer, GameInventory::getInstance()->closeInventorySFX->audioClip->GetName().c_str());
				}
			}
		}
		{
			ImGui::Text("Drag From Inventory SFX:");
			static char soundNameBuffer[256] = "";

			if (GameInventory::getInstance()->pickFromInventorySFX->audioClip != nullptr)
				strcpy(soundNameBuffer, GameInventory::getInstance()->pickFromInventorySFX->audioClip->GetName().c_str());

			Texture* buttonIcon = nullptr;

			if (!GameInventory::getInstance()->pickFromInventorySFX->IsPlaying())
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
			else
				buttonIcon = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon");

			ImGui::PushID(1);
			if (ImGui::ImageButton((ImTextureID)buttonIcon->GetTextureID(), ImVec2(20, 20)))
			{
				if (!GameInventory::getInstance()->pickFromInventorySFX->IsPlaying())
					GameInventory::getInstance()->pickFromInventorySFX->Play();
				else
					GameInventory::getInstance()->pickFromInventorySFX->Stop();
			}
			ImGui::PopID();

			ImGui::SameLine();

			ImGui::InputTextWithHint("", "Pick From Inventory Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_SFX)
					{
						GameInventory::getInstance()->pickFromInventorySFX->audioClip = (AudioClip*)resourceDropped;
						SaveAndLoad::getInstance()->SaveInitFile();
						strcpy(soundNameBuffer, resourceDropped->GetName().c_str());
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();

			if (ImGui::Button("Search##SearchSoundDrag"))
			{
				ImGui::OpenPopup("print_sound_selection_popup3");
				showSoundSelectionPopup = true;
			}

			if (showSoundSelectionPopup)
			{
				Resource* selectedSound = ResourceManager::getInstance()->PrintSoundsSelectionPopup3();

				if (selectedSound != nullptr)
				{
					GameInventory::getInstance()->pickFromInventorySFX->audioClip = (AudioClip*)selectedSound;
					SaveAndLoad::getInstance()->SaveInitFile();
					showSoundSelectionPopup = false;
					strcpy(soundNameBuffer, GameInventory::getInstance()->pickFromInventorySFX->audioClip->GetName().c_str());
				}
			}
		}

		if (App->isEngineInPlayMode)
		{
			IMGUI_SPACED_SEPARATOR;
			if (ImGui::Checkbox("See Preview", &GameInventory::getInstance()->seePreview))
			{
				GameInventory::getInstance()->ToggleVisibility();
			}
		}
	}

	if (ImGui::CollapsingHeader("Visibility"))
	{
		//ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		//ImGui::Text("Clickable Area Draw Mode:");
		//ImGui::PopFont();

		static int selectedClickableAreaDrawMode = 0;
		if (ImGui::Combo("##drawMode", &selectedClickableAreaDrawMode, "Draw On Selected Object\0Draw Always"))
		{
			switch (selectedClickableAreaDrawMode)
			{
			case 0:
				ViewportManager::getInstance()->drawClickableAreaCondition = DRAW_ON_SELECTED;
				break;

			case 1:
				ViewportManager::getInstance()->drawClickableAreaCondition = DRAW_ALWAYS;
				break;
			}
		}
	}

	if (ImGui::CollapsingHeader("Room Thumbnail"))
	{
		static char buf[256] = "";

		Texture* roomThumbnailTexture = App->moduleWorldManager->GetSelectedRoom()->roomThumbnail; 

		if (roomThumbnailTexture == nullptr)
			roomThumbnailTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject"); 

		// Object Occurrence -----------------------
		float aspect_ratio = roomThumbnailTexture->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		PUSH_FONT(App->moduleImGui->rudaRegularMid);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		int childHeight = previewQuadHeight + 20;

		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)roomThumbnailTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", roomThumbnailTexture->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", roomThumbnailTexture->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", roomThumbnailTexture->GetHeigth());

		Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		if (ImGui::Button("Change Image"))
		{
			char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

			if (path != NULL)
			{
				if (!ResourceManager::getInstance()->ExistResourcePath(path))
				{
					App->moduleWorldManager->GetSelectedRoom()->roomThumbnail = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(App->moduleWorldManager->GetSelectedRoom()->roomThumbnail, App->moduleWorldManager->GetSelectedRoom()->roomThumbnail->GetName());
				}
				else
					App->moduleWorldManager->GetSelectedRoom()->roomThumbnail = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);

				strcpy(buf, path);
				flog("Player Opened %s", path);
			}
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();

		POP_FONT;
	}
}

