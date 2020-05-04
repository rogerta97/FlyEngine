#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "ModuleManager.h"
#include "ModuleInput.h"
#include "ResourceManager.h"
#include "ObjectCreatorDockPanel.h"
#include "CharacterCreatorDockPanel.h"
#include "ModuleImGui.h"

#include "UI_Image.h"
#include "UI_Button.h"
#include "RoomDockPanel.h"
#include "RoomUIHandler.h"
#include "ImageImporter.h"
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
	if (!DockPanel::Draw() || App->moduleRoomManager->GetSelectedRoom() == nullptr)
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
				App->moduleRoomManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 1));
			}

			if (ImGui::BeginTabItem("User Interface"))
			{
				DrawUserInterfaceTab();
				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_UI;
				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr); 
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 0.5f));
			}

			if (ImGui::BeginTabItem("Room Settings"))
			{
				ShowViewportSettingsTab();

				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked())
			{
				ViewportManager::getInstance()->editRoomMode = EDIT_ROOM_OBJECTS;
				App->moduleRoomManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
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
				App->moduleRoomManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(nullptr);
				App->moduleManager->SetSceneUITint(float4(1, 1, 1, 1));
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

		RoomUIHandler* roomUIHandler = App->moduleRoomManager->GetSelectedRoom()->roomUIHandler;

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
	// Add and delete variable Buttons -----------
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);

	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->AddDefaultVariable();
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
	{

	}

	ImGui::PopStyleVar();
	IMGUI_SPACED_SEPARATOR;

	// Search Bar --------------------
	static char searchVariableBuffer[256];
	ImGui::InputTextWithHint("", "Search...", searchVariableBuffer, IM_ARRAYSIZE(searchVariableBuffer));

	// Child List Space Title --------
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Blackboard Variables:");
	POP_FONT;

	// Variable List UI --------------
	DrawRoomVariablesUI();
}

void RoomDockPanel::DrawRoomVariablesUI()
{
	Blackboard* roomBB = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard();

	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("BlackboardChild", ImVec2(ImGui::GetContentRegionMax().x - 5, ImGui::GetContentRegionAvail().y));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, (ImGui::GetCursorPos().y + 5)));

	int counter = 0;
	for (auto& currentVar : roomBB->blackboardVariablesList)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.28f, 0.43f, 0.56, 0.2f));

		string varChildID = "VariableUIGroup" + to_string(counter);
		ImGui::BeginChild(varChildID.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 5, 95));

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 70);

		int iconSize = 47;
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, (ImGui::GetContentRegionAvail().y / 2) - iconSize / 2));
		Texture* variableType = nullptr;

		if (currentVar->varType == Var_Integer)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
		else if (currentVar->varType == Var_Toggle)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

		ImGui::Image((ImTextureID*)variableType->GetTextureID(), ImVec2(iconSize, iconSize));

		ImGui::NextColumn();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

		char nameBuffer[256];
		strcpy(nameBuffer, currentVar->name.c_str());
		string nameStringID = "Name##VarName" + to_string(counter);

		if (ImGui::InputText(nameStringID.c_str(), nameBuffer, IM_ARRAYSIZE(nameBuffer)))
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
	Texture* exportTexture = (Texture*)ResourceManager::getInstance()->GetResource("ExportIcon");
	if (ImGui::ImageButton((ImTextureID)exportTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		SaveAndLoad::getInstance()->SaveCurrentRoomData();
	}

	// Save Button ------------
	ImGui::SameLine();
	Texture* importIcon = (Texture*)ResourceManager::getInstance()->GetResource("ImportIcon");
	if (ImGui::ImageButton((ImTextureID)importIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1)))
	{
		char const* lFilterPatterns[1] = { "*.json" };
		string defaultPath = MyFileSystem::getInstance()->GetSolutionDirectory();
		defaultPath += "Source\\Game\\Resources\\EngineSavedData\\RoomsData\\";
		const char* path = tinyfd_openFileDialog("Load Image...", defaultPath.c_str(), 1, lFilterPatterns, NULL, 0);

		if (path != "")
		{
			// Clean Current Room Content
			Room* currentRoom = App->moduleRoomManager->GetSelectedRoom();
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

	if (App->moduleRoomManager->GetSelectedRoom()->GetObjectsInRoomAmount() == 0)
	{
		ImGui::TextColored(ImVec4(0, 1, 1, 1), " There are no objects in this room");
	}
	else
	{
		Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		FlyObject* selectedObject = selectedRoom->GetSelectedObject();

		for (list<FlyObject*>::reverse_iterator it = selectedRoom->objectsInRoom.rbegin(); it != selectedRoom->objectsInRoom.rend(); it++) 
		{
			PUSH_FONT(App->moduleImGui->rudaBlackMid);

			bool objectSelected = false;

			if (selectedObject != nullptr)
				objectSelected = (selectedObject->GetName() == (*it)->GetName());

			if (ImGui::Selectable((*it)->GetName().c_str(), objectSelected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				(*it)->isSelected = true;
				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(*it);
			}

			if (ImGui::IsWindowHovered() && App->moduleInput->GetMouseButton(RIGHT_CLICK) == KEY_DOWN)
			{
				if (ImGui::IsItemHovered())
				{
					(*it)->isSelected = true;
					App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(*it);
					ImGui::OpenPopup("right_click_item_hierarchy");
				}
				else
					ImGui::OpenPopup("right_click_hierarchy");
			}

			// Move Layer Arrows
			DrawMoveLayerSelectableButtons();

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
					
					App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(newClone);
				}

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

void RoomDockPanel::DrawMoveLayerSelectableButtons()
{
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 48);
	Texture* arrowUpObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowUpWhite");
	if (ImGui::ImageButton((ImTextureID)arrowUpObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr);
	}

	ImGui::SameLine();
	Texture* arrowDownObjectTexture = (Texture*)ResourceManager::getInstance()->GetResource("ArrowDownWhite");
	if (ImGui::ImageButton((ImTextureID)arrowDownObjectTexture->GetTextureID(), ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0))
	{
		App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr);
	}

	ImGui::PopStyleColor();
}

void RoomDockPanel::ShowViewportSettingsTab()
{

	if (ImGui::CollapsingHeader("Music", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Music:");
		ImGui::PopFont();

		Room* currentRoom = App->moduleRoomManager->GetSelectedRoom();

		static char trackNameBuffer[256] = "";

		if (currentRoom->backgroundMusic != nullptr)
			strcpy(trackNameBuffer, currentRoom->backgroundMusic->GetName().c_str());

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
	}

	if (ImGui::CollapsingHeader("Visibility", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Clickable Area Draw Mode:");
		ImGui::PopFont();

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

	/*	static int resolutionSelected = 0;
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Resolution:");
		ImGui::PopFont();

		if (ImGui::Combo("ResolutionCombo##", &resolutionSelected, "4:3\0 1:1\0")) {

			switch (resolutionSelected)
			{

			case 0:
				ViewportManager::getInstance()->SetAspectRatioType(AR_4_3);
				break;

			case 1:
			{
				ViewportManager::getInstance()->SetAspectRatioType(AR_1_1);
				break;
			}

			default:
				break;
			}
		}*/
	}

	if (ImGui::CollapsingHeader("Room Thumbnail", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static char buf[256] = "";

		Texture* roomThumbnailTexture = App->moduleRoomManager->GetSelectedRoom()->roomThumbnail; 

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
					App->moduleRoomManager->GetSelectedRoom()->roomThumbnail = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(App->moduleRoomManager->GetSelectedRoom()->roomThumbnail, App->moduleRoomManager->GetSelectedRoom()->roomThumbnail->GetName());
				}
				else
					App->moduleRoomManager->GetSelectedRoom()->roomThumbnail = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);

				strcpy(buf, path);
				flog("Player Opened %s", path);
			}
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();

		POP_FONT;
	}
}

