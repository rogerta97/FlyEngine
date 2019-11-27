#include "ObjectCreatorDockPanel.h"

#include "Application.h"
#include "TinyFileDialog.h"
#include "ModuleImGui.h"
#include "Tool.h"
#include "ModuleRoomManager.h"
#include "MyFileSystem.h"
#include "Room.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "ImageTool.h"
#include "ViewportManager.h"
#include "ResourceManager.h"
#include "GameViewportDockPanel.h"

#include "FlyObject.h"
#include "imgui.h"

#include "mmgr.h"

ObjectCreatorDockPanel::ObjectCreatorDockPanel(bool isVisible) : DockPanel("Entity Creator", isVisible)
{
	flyEngineSection = FLY_SECTION_null;
	dockPanelType = DOCK_OBJECT_CREATOR;

	previewClickableAreaTexture = nullptr; 

	clickableAreaPosPerc = float2(0, 0);
	clickableAreaSizePerc = float2(1.0f, 1.0f); 
	previewTextureMaxSize = 220; 
}

ObjectCreatorDockPanel::~ObjectCreatorDockPanel()
{

}

bool ObjectCreatorDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Tools"))
			{
				DrawObjectCreator(); 
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Clickable Area"))
			{
				DrawClickableAreaCreator(); 
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		DrawCreateButton();
	}

	ImGui::End(); 
}

void ObjectCreatorDockPanel::ResetObjectData()
{
	strcpy(newObjectName, ""); 
	strcpy(searchNewToolBuffer, ""); 
	selectedTool = nullptr; 
	creatingObject = new FlyObject("Prev");
}

void ObjectCreatorDockPanel::DrawObjectCreator()
{
	DrawObjectToolsList(); 
	DrawAddAndDeleteToolButtons(); 
	DrawSelectedToolSettings(); 
}

void ObjectCreatorDockPanel::DrawObjectToolsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Add Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int pos = 0; 
	for (auto& currentTool : creatingObject->GetToolsList())
	{
		ToolSelectableInfo selectableInfo = currentTool->GetToolSelectableInfo();
		DrawSelectable(selectableInfo, currentTool->IsSelected(), pos, 42, currentTool);
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawSelectable(ToolSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight = 42, Tool* currentTool = nullptr)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");
	ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, +(selectableHeight * posInList)));
	if (ImGui::Selectable(selectableInfo.toolName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, selectableHeight - 3))) {
		creatingObject->selectedTool = currentTool;
		selectedTool = currentTool; 
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY((selectableHeight * posInList) + 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.toolDescription.c_str());
	ImGui::PopFont();
}

void ObjectCreatorDockPanel::DrawSelectedToolSettings()
{
	if (selectedTool)
	{
		if (selectedTool->GetToolType() != AT_null) {
			IMGUI_SPACED_SEPARATOR
		}

		switch (selectedTool->GetToolType())
		{
		case AT_IMAGE:
			DrawToolImageSettings();
			break;

		case AT_CHANGE_SCENE:
			break;
		}

	}
}

void ObjectCreatorDockPanel::OnAddToolButtonClicked()
{
	if (showToolDictionary)
	{
		ImGui::Separator();
		ImGui::Spacing(); 

		// Search Bar ---------------
		ImGui::InputText("##SearchTool", searchNewToolBuffer, IM_ARRAYSIZE(searchNewToolBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		// Tools Dictonary ----------
		ToolSelectableInfo* newToolSelected = App->moduleManager->DrawToolDictionaryUI(); 
		if (newToolSelected != nullptr)
		{
			switch (newToolSelected->toolType)
			{
			case AT_IMAGE:
				creatingObject->AddImageTool(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "ImageNull.png").c_str()); 
				break;

			case AT_null:
				break;
			}

			showToolDictionary = false; 
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(); 
		ImGui::Separator();
	}
}

void ObjectCreatorDockPanel::Close() 
{
	selectedTool = nullptr;
	visible = false; 
}
	
void ObjectCreatorDockPanel::DrawClickableAreaCreator()
{
	PrintClickableAreaObjectVisuals();
	DrawClickableAreaSettings();
}

void ObjectCreatorDockPanel::DrawClickableAreaSettings()
{
	if (clickableAreaActive)
	{
		float2 posLimit = float2(1,1);
		float2 sizeLimit = float2(1, 1);

		if (!creatingObject->HasVisuals())
		{		
			ImGui::Separator();

			ImGui::PushFont(App->moduleImGui->rudaRegularSmall); 
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvailWidth() + 5);
			ImGui::TextColored(ImVec4(1, 1, 0.2f, 0.8f), "The object being created has no tools with visible content. Clickable Area position will fit the center of the object");
			ImGui::PopTextWrapPos(); 
			ImGui::PopFont(); 

			ImGui::Separator();		
			
			ImGui::DragFloat("Width", &clickableAreaSizePerc.x, 1.0f, 0.1f, App->moduleImGui->gameViewportDockPanel->GetViewportSize().x);
			ImGui::DragFloat("Height", &clickableAreaSizePerc.y, 1.0f, 0.1f, App->moduleImGui->gameViewportDockPanel->GetViewportSize().y);
		}
		else
		{
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("Position:");
			ImGui::PopFont();

			posLimit = float2(1, 1) - clickableAreaSizePerc;
			if (clickableAreaSizePerc.x > sizeLimit.x)
			{
				sizeLimit.x = 1 - clickableAreaPosPerc.x;
			}

			if (clickableAreaSizePerc.y > sizeLimit.y)
			{
				sizeLimit.y = 1 - clickableAreaPosPerc.x;
			}

			ImGui::DragFloat("Horizontal", &clickableAreaPosPerc.x, 0.005f, 0.05f, posLimit.x);
			ImGui::DragFloat("Vertical", &clickableAreaPosPerc.y, 0.005f, 0.05f, posLimit.y);
	
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("Size:");
			ImGui::PopFont();

			float2 viewportSize = App->moduleImGui->gameViewportDockPanel->GetViewportSize();
			sizeLimit = float2(1, 1) - clickableAreaPosPerc;
			if (clickableAreaPosPerc.x > posLimit.x)
			{
				posLimit.x = 1 - clickableAreaSizePerc.x;
			}

			if (clickableAreaPosPerc.y > posLimit.y)
			{
				posLimit.y = 1 - clickableAreaSizePerc.x;
			}

			ImGui::DragFloat("Width", &clickableAreaSizePerc.x, 0.005f, 0.05f, sizeLimit.x);
			ImGui::DragFloat("Height", &clickableAreaSizePerc.y, 0.005f, 0.05f, sizeLimit.y);

			IMGUI_SPACED_SEPARATOR;
		}
	}
}

void ObjectCreatorDockPanel::PrintClickableAreaObjectVisuals()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Preview:");
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() + 10); 
	if (ImGui::Checkbox("Active", &clickableAreaActive))
	{
		if (clickableAreaActive && previewClickableAreaTexture != nullptr)
		{
			float2 textureSize = float2(previewClickableAreaTexture->GetWidth(), previewClickableAreaTexture->GetHeigth()); 
		}
	}

	ImGui::BeginChild("ShowClickableArea", ImVec2(ImGui::GetContentRegionAvailWidth(), previewTextureMaxSize));

	if (!creatingObject->HasVisuals())
	{
		// Background
		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (previewTextureMaxSize / 2), 0));
		ImGui::Image(0, ImVec2(previewTextureMaxSize, previewTextureMaxSize));
		
		// Show No Visual Text
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 50, 100));
		ImGui::TextColored(ImVec4(1, 0.8, 0.8f, 0.8f), "NO VISUALS");
		ImGui::PopFont(); 
	}
	else
	{
		DrawPrevTextureCA();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawPrevTextureCA()
{
	// Prev Texture
	ImageTool* imageTool = (ImageTool*)creatingObject->GetTool(AT_IMAGE);
	if (imageTool != nullptr)
	{
		previewClickableAreaTexture = imageTool->GetTexture();

		if (previewClickableAreaTexture == nullptr)
			return; 

		ImVec2 prevTextureSize;
		GetTextureSizeFitted(prevTextureSize);

		float yOffset = ImGui::GetContentRegionAvail().y / 2 - (prevTextureSize.y / 2);
		float2 imageTopLeft = float2(ImGui::GetContentRegionAvailWidth() / 2 - (previewTextureMaxSize / 2), yOffset); 

		ImGui::SetCursorPos(ImVec2(imageTopLeft.x, imageTopLeft.y));
		ImGui::Image((ImTextureID)previewClickableAreaTexture->GetTextureID(), prevTextureSize);
		
		if (clickableAreaActive) {
			DrawPreviewClickableAreaOnTexture(imageTopLeft, float2(prevTextureSize.x, prevTextureSize.y));
		}
	}
}

void ObjectCreatorDockPanel::GetTextureSizeFitted(ImVec2& prevTextureSize)
{
	prevTextureSize.x = previewClickableAreaTexture->GetWidth();
	prevTextureSize.y = previewClickableAreaTexture->GetHeigth();

	if (prevTextureSize.x <= prevTextureSize.y)
	{
		prevTextureSize.y = previewTextureMaxSize;
		prevTextureSize.x = previewTextureMaxSize / previewClickableAreaTexture->GetAspectRatio();
	}
	else
	{
		prevTextureSize.x = previewTextureMaxSize;
		prevTextureSize.y = previewTextureMaxSize / previewClickableAreaTexture->GetAspectRatio();
	}
}

void ObjectCreatorDockPanel::DrawPreviewClickableAreaOnTexture(float2 textureTopLeft, float2 prevTextureSize)
{
	float2 clickableAreaPos = float2(prevTextureSize.x * clickableAreaPosPerc.x, prevTextureSize.y * clickableAreaPosPerc.y); 
	float2 clickableAreaSize = float2(prevTextureSize.x * clickableAreaSizePerc.x, prevTextureSize.y * clickableAreaSizePerc.y);

	float2 clickableAreaTopLeft = textureTopLeft + clickableAreaPos;
	ImGui::SetCursorPos(ImVec2(clickableAreaTopLeft.x, clickableAreaTopLeft.y));

	Texture* colorTexture = (Texture*)ResourceManager::getInstance()->GetResource("ClickableAreaPreviewColor"); 
	ImGui::Image((ImTextureID)colorTexture->GetTextureID(), ImVec2(clickableAreaSize.x, clickableAreaSize.y));
}

void ObjectCreatorDockPanel::DrawCreateButton()
{
	ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 35); 

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::InputTextWithHint("##ObjectNaming", "Object Name...", newObjectName, 256 * sizeof(char));
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::SameLine();
	if (ImGui::Button("Create", ImVec2(80, 25)))
	{
		std::string newObjectNameStr(newObjectName);
		if (newObjectNameStr.empty()) {
			FLY_ERROR("Object with no name can not be created");
			ImGui::PopFont();
			return;
		}
		else 
		{
			AddCreatingObject();
		}

		Close();
	}

	ImGui::PopFont();
}

void ObjectCreatorDockPanel::AddCreatingObject()
{
	// Clickable Area
	if (clickableAreaActive)
	{
		if (!creatingObject->HasVisuals())
		{
			creatingObject->SetCASizeFromOne(clickableAreaPosPerc, clickableAreaSizePerc, true);
		}
		else
		{
			creatingObject->SetCASizeFromOne(clickableAreaPosPerc, clickableAreaSizePerc); 
		}
	}

	creatingObject->SetClickableAreaPosOne(clickableAreaPosPerc);
	creatingObject->SetClickableAreaSizeOne(clickableAreaSizePerc);

	// Add Object
	creatingObject->SetName(newObjectName);
	App->moduleRoomManager->GetSelectedRoom()->AddFlyObject(creatingObject);
}

void ObjectCreatorDockPanel::DrawToolImageSettings()
{
	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";
		ImageTool* selectedImageTool = (ImageTool*)this->selectedTool; 

		if (selectedImageTool->GetTexture() == nullptr)
			selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResource("ImageNull"));
		else
			selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResource(selectedImageTool->GetTexture()->GetName()));

		float aspect_ratio = selectedImageTool->GetTexture()->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		int childHeight = previewQuadHeight + 20; 
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)selectedImageTool->GetTexture()->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", selectedImageTool->GetTexture()->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageTool->GetTexture()->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageTool->GetTexture()->GetHeigth());

		Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)searchTexture->GetTextureID(), ImVec2(24, 24)))
		{
			char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

			if (path != NULL)
			{
				if (!ResourceManager::getInstance()->ExistResourcePath(path))
				{
					selectedImageTool->SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
					ResourceManager::getInstance()->AddResource(selectedImageTool->GetTexture(), selectedImageTool->GetTexture()->GetName());
				}
				else
				{
					selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
				}

				strcpy(buf, path);
				FLY_LOG("Player Opened %s", path);
			}

		}

		ImGui::PopStyleColor(2);
		ImGui::EndChild();
		ImGui::PopFont();
	}
}

void ObjectCreatorDockPanel::DrawAddAndDeleteToolButtons()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");

	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
	{
		showToolDictionary = true; 
		//ImGui::OpenPopup("plusIconClicked");
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
	{
		if (selectedTool != nullptr) {
			creatingObject->DeleteTool(selectedTool->GetToolName()); 
		}
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	// Callbacks for buttons 
	OnAddToolButtonClicked(); 
}
