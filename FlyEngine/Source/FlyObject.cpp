#include "FlyObject.h"
#include "RandomNumberGenerator.h"
#include "ModuleImGui.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "TinyFileDialog.h"
#include "Quad.h"
#include "ImageImporter.h"
#include "ModuleManager.h"
#include "GameViewportDockPanel.h"
#include "ChangeRoomAction.h"
#include "ModuleInput.h"
#include "EmitSoundAction.h"
#include "ModuleRoomManager.h"
#include "GameInventory.h"
#include "ModifyVariableAction.h"
#include "ViewportManager.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "MyFileSystem.h"
#include "Room.h"
#include "Gizmos.h"
#include <string>

#include "mmgr.h"

FlyObject::FlyObject(std::string _name, std::string _description, FlyObjectType _flyObjectType, Room* _parentRoom)
{
	name = _name; 
	description = _description;
	uid = RandomNumberGenerator::GenerateUID(); 
	isSelected = false; 
	hasVisuals = false; 
	clickableAreaActive = false;
	drawClickableArea = false;
	flyObjectType = _flyObjectType; 
	parentRoom = _parentRoom; 

	clickableArea = new BoundingBox(); 
	clickableAreaPosPerc = float2(0, 0); 
	clickableAreaSizePerc = float2(0, 0); 
	clickableAreaColor = float4(1.0f, 0.8f, 0.8f, 0.4f);

	transform = new Transform();
	gizmos = new Gizmos(this);
}

FlyObject::~FlyObject()
{

}

//Returns true if this objects is deleted in the middle of the update
bool FlyObject::Update()
{
	bool ret = false; 

	if (isSelected) 
	{
		gizmos->Update();

		if(clickableArea != nullptr)
		{
			//clickableArea->Update();	
		}		
	}

	if (App->isEngineInPlayMode && App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && GameInventory::getInstance()->droppingObject == nullptr)
	{
		if (clickableArea->IsBoxClicked())
		{
			flog("Object Clicked"); 

			switch (flyObjectType)
			{
			case ACTION_OBJECT:
				DoOnClickActions();
				break;

			case INVENTORY_ITEM:
				DoOnClickActions(); 
				App->moduleRoomManager->GetSelectedRoom()->AddItemToInventory(this);
				App->moduleManager->AddDeleteFromListObject(this); 
				ret = true; 
				break;
			}
		}
	}

	return ret; 
}

void FlyObject::Draw()
{
	for (auto& it : actionsList) {
		(it)->Draw(); 
	}

	if (isSelected)
	{
		if (clickableArea != nullptr && drawClickableArea && clickableAreaActive)		
			DrawClickableArea();
		
		gizmos->Draw();
	}
}

void FlyObject::DrawClickableArea()
{
	if (drawClickableArea && clickableAreaActive)
	{
		clickableArea->Draw(true, clickableAreaColor);
	}
}

void FlyObject::CleanUp()
{
	for (auto& it : actionsList)
	{
		(it)->CleanUp(); 
		delete it; 
	}

	actionsList.clear(); 
	delete transform; 

	gizmos->CleanUp();
	delete gizmos; 

	if (clickableArea != nullptr)
	{
		clickableArea->CleanUp();
		delete clickableArea;
	}

}

std::string FlyObject::GetName() const
{
	return name;
}

void FlyObject::SetName(std::string newName)
{
	name = newName; 
}

UID FlyObject::GetUID() const
{
	return uid;
}

void FlyObject::SetUID(UID newUID)
{
	uid = newUID;
}

std::string FlyObject::GetDescription() const
{
	return description;
}

void FlyObject::SetDescription(std::string newDescription)
{
	description = newDescription;
}

float4 FlyObject::GetClickableAreaColor() const
{
	return clickableAreaColor;
}

void FlyObject::SetClickableAreaColor(float4 newColor)
{
	clickableAreaColor = newColor; 
}

Room* FlyObject::GetParentRoom() const
{
	return parentRoom; 
}

bool& FlyObject::IsInteractable()
{
	return isInteractable; 
}

void FlyObject::SetInteractable(bool _isInteractable)
{
	isInteractable = _isInteractable; 
}

DisplayImageAction* FlyObject::GetDisplayImageAction()
{
	return (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);
}

float2 FlyObject::GetObjectVisualDimensions()
{
	float2 objectVisualSize; 

	// For now we will just take into account ImageTool, if you are deleting this you advanced really. 
	DisplayImageAction* imageTool = (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE); 
	if (imageTool != nullptr)
	{
		objectVisualSize = float2(imageTool->GetQuad()->quadWidth, imageTool->GetQuad()->quadHeight); 
	}

	return objectVisualSize; 
}

void FlyObject::CalculateAllGizmos()
{
	gizmos->CalculateSelectGizmo(this);
	gizmos->CalculateMoveGizmo(this);
	gizmos->CalculateScaleGizmo(this); 
}

void FlyObject::FitObjectUtils()
{
	CalculateAllGizmos();
	if (clickableArea)
	{
		float2 offset = SetCASizeFromOne(clickableAreaPosPerc, clickableAreaSizePerc, !hasVisuals); 
		clickableArea->SetPosition(float2(transform->GetPosition(true).x + offset.x,transform->GetPosition(true).y + offset.y)); 
	}
}

bool FlyObject::IsMouseOver()
{
	return gizmos->IsMouseOver(); 
}

bool FlyObject::HasVisuals()
{
	for (auto& currentTool : actionsList)
	{
		if (currentTool->HasVisual())
			return true; 
	}

	return false; 
}

bool FlyObject::IsInventoryItem()
{
	if (flyObjectType == INVENTORY_ITEM)
		return true; 

	return false; 
}

void FlyObject::SaveObjectData(JSON_Object* jsonObject, int objectIndex)
{
	// Save Object Properties
	string serializeObjectName(App->moduleRoomManager->GetSelectedRoom()->GetName().c_str() + string(".")); 
	serializeObjectName += "FlyObject_" + to_string(objectIndex) + string(".");

	json_object_dotset_string(jsonObject, string(serializeObjectName + "Name").c_str(), GetName().c_str());
	json_object_dotset_number(jsonObject, string(serializeObjectName + "UID").c_str(), uid);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Interactable").c_str(), isInteractable);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "ObjectType").c_str(), this->flyObjectType);

	if(!GetDescription().empty())
		json_object_dotset_string(jsonObject, string(serializeObjectName + "Description").c_str(), GetDescription().c_str());

	SaveTransform(serializeObjectName, jsonObject);

	// Save Object Action Settings
	for (auto& it : actionsList)
	{
		it->SaveAction(jsonObject, serializeObjectName);
	}

	// Save Object Clickable Area
	SaveClickableArea(serializeObjectName, jsonObject);
}

void FlyObject::SaveTransform(std::string serializeObjectName, JSON_Object* jsonObject)
{
	serializeObjectName += "Transform.";
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Position.x").c_str(), transform->GetPosition().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Position.y").c_str(), transform->GetPosition().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Rotation.x").c_str(), transform->GetRotation().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Rotation.y").c_str(), transform->GetRotation().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Scale.x").c_str(), transform->GetScale().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Scale.y").c_str(), transform->GetScale().y);
}

void FlyObject::SaveClickableArea(std::string serializeObjectName, JSON_Object* jsonObject)
{
	serializeObjectName += "ClickableArea.";

	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Active").c_str(), clickableAreaActive);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "DirectPosition").c_str(),!hasVisuals);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.r").c_str(), clickableAreaColor.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.g").c_str(), clickableAreaColor.y);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.b").c_str(), clickableAreaColor.z);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.a").c_str(), clickableAreaColor.w);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "MinPoint.x").c_str(), clickableArea->GetMinPoint().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "MinPoint.y").c_str(), clickableArea->GetMinPoint().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "MaxPoint.x").c_str(), clickableArea->GetMaxPoint().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "MaxPoint.y").c_str(), clickableArea->GetMaxPoint().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "PosPerc.x").c_str(), clickableAreaPosPerc.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "PosPerc.y").c_str(), clickableAreaPosPerc.y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "SizePerc.x").c_str(), clickableAreaSizePerc.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "SizePerc.y").c_str(), clickableAreaSizePerc.y);

}

void FlyObject::DoOnClickActions()
{
	for (auto& it : actionsList)
	{
		if (it->IsOccObjectClicked())
			it->DoAction(); 
	}
}

void FlyObject::CalculateCurrentGizmo()
{
	switch (App->moduleImGui->gameViewportDockPanel->GetGizmoMode())
	{
	case GIZMO_SELECT:
		gizmos->CalculateSelectGizmo(this); 
		break; 

	case GIZMO_MOVE:
		gizmos->CalculateMoveGizmo(this);
		break;

	case GIZMO_SCALE:
		gizmos->CalculateScaleGizmo(this);
		break;
	}
}

DisplayImageAction* FlyObject::AddDisplayImageAction(const char* imageTexturePath)
{
	if (GetAction(ACTION_DISPLAY_IMAGE) == nullptr)
	{
		DisplayImageAction* newAtrImage = new DisplayImageAction(this);
		newAtrImage->CreateImage(imageTexturePath);

		actionsList.push_back(newAtrImage);

		// Addapt Gizmo Rect to new Image
		gizmos->FitSelectBoxSize();
		hasVisuals = true; 

		return newAtrImage;
	}

	return (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE); 	
}

ChangeRoomAction* FlyObject::AddChangeRoomAction()
{
	if (GetAction(ACTION_CHANGE_ROOM) == nullptr)
	{
		ChangeRoomAction* changeSceneTool = new ChangeRoomAction(this);

		actionsList.push_back(changeSceneTool);

		// Addapt Gizmo Rect to new Image
		gizmos->FitSelectBoxSize();

		return changeSceneTool;
	}

	return (ChangeRoomAction*)GetAction(ACTION_CHANGE_ROOM);
}

ModifyVariableAction* FlyObject::AddModifyVariableAction()
{
	if (GetAction(ACTION_MOD_VARIABLE) == nullptr)
	{
		ModifyVariableAction* mofidyVarAction = new ModifyVariableAction(this);
		//ModifyVariableAction* mofidyVarAction = nullptr; 
		actionsList.push_back(mofidyVarAction);
		return mofidyVarAction;
	}

	return (ModifyVariableAction*)GetAction(ACTION_MOD_VARIABLE);
}

EmitSoundAction* FlyObject::AddEmitSoundAction()
{
	if (GetAction(ACTION_EMIT_SOUND) == nullptr)
	{
		EmitSoundAction* emitSoundAction = new EmitSoundAction(this);
		actionsList.push_back(emitSoundAction);
		return emitSoundAction;
	}

	return (EmitSoundAction*)GetAction(ACTION_EMIT_SOUND);
}

void FlyObject::SetSelectedAction(ActionType toolTypeSelected)
{
	for (auto& it : actionsList)
	{
		if (it->GetActionType() == toolTypeSelected)
		{
			it->SetSelected(true);
			selectedAction = it; 
		}
		else
			it->SetSelected(false); 
	}
}

ActionType FlyObject::GetSelectedActionType()
{
	if (selectedAction == nullptr)
		return AT_null; 

	return selectedAction->GetActionType();
}

Action* FlyObject::GetAction(std::string toolName)
{
	for (auto& currentTool : actionsList)
	{
		if (currentTool->GetActionName() == toolName)
			return currentTool;
	}

	return nullptr; 
}

Action* FlyObject::GetAction(ActionType toolType)
{
	for (auto& currentTool : actionsList)
	{
		if (currentTool->GetActionType() == toolType)
			return currentTool;
	}

	return nullptr;
}

std::list<Action*> FlyObject::GetActionsList() const
{
	return actionsList;
}

void FlyObject::DeleteAction(std::string toolNameToDelete)
{
	for (auto currentTool = actionsList.begin(); currentTool != actionsList.end(); currentTool++)
	{
		if ((*currentTool)->GetActionName() == toolNameToDelete)
		{
			(*currentTool)->CleanUp();
			delete (*currentTool);
			(*currentTool) = nullptr; 
			currentTool = actionsList.erase(currentTool); 
			return; 
		}
	}
}

BoundingBox* FlyObject::GetClickableArea()
{
	return clickableArea;
}

void FlyObject::CreateClickableArea(float2 percentagePos, float2 percentageSize, bool directPosition)
{
	if (clickableArea == nullptr)
		clickableArea = new BoundingBox();

	SetCASizeFromOne(percentagePos, percentageSize, directPosition); 

	clickableAreaPosPerc = percentagePos; 
	clickableAreaSizePerc = percentageSize;
}


float2& FlyObject::GetClickableAreaPosOne()
{
	return clickableAreaPosPerc; 
}

float2& FlyObject::GetClickableAreaSizeOne()
{
	return clickableAreaSizePerc; 
}

void FlyObject::SetClickableAreaPosOne(float2 newAreaPosOne)
{
	clickableAreaPosPerc = newAreaPosOne; 
}

void FlyObject::SetClickableAreaSizeOne(float2 newAreaSizeOne)
{
	clickableAreaSizePerc = newAreaSizeOne; 
}

float2 FlyObject::SetCASizeFromOne(float2 percentagePos, float2 percentageSize, bool directiPosition)
{
	if (clickableArea == nullptr)
		return float2::zero; 

	float2 offsetFromCenter; 

	if (directiPosition)
	{
		clickableArea->SetMinPoint(float2(-percentageSize.x, percentageSize.y));
		clickableArea->SetMaxPoint(float2(percentageSize.x, -percentageSize.y));
		offsetFromCenter = float2::zero;
	}
	else
	{
		float2 objectSize = GetObjectVisualDimensions(); 
		
		float2 objectTopLeft = float2(-objectSize.x / 2, -objectSize.y / 2);
		float2 clickable_area_size = float2(objectSize.x * percentageSize.x, objectSize.y * percentageSize.y);
		float2 clickable_area_pos = float2(objectSize.x * percentagePos.x, objectSize.y * percentagePos.y);

		clickableArea->SetMinPoint(float2(objectTopLeft.x + clickable_area_pos.x, objectTopLeft.y + clickable_area_pos.y + clickable_area_size.y));
		clickableArea->SetMaxPoint(float2(objectTopLeft.x + clickable_area_pos.x + clickable_area_size.x, objectTopLeft.y + clickable_area_pos.y));

		clickableArea->SetMinPoint(float2(clickableArea->GetMinPoint().x * transform->GetScale().x, clickableArea->GetMinPoint().y * transform->GetScale().y));
		clickableArea->SetMaxPoint(float2(clickableArea->GetMaxPoint().x * transform->GetScale().x, clickableArea->GetMaxPoint().y * transform->GetScale().y));

		this->clickableAreaPosPerc = percentagePos;
		this->clickableAreaSizePerc = percentageSize;

		float2 objectCenter = objectTopLeft + GetObjectVisualDimensions() / 2;
		offsetFromCenter = clickableArea->GetCenter() - objectCenter; 
	}	

	return offsetFromCenter; 
}

void FlyObject::DrawDisplayImageSettings()
{
	DisplayImageAction* imageTool = (DisplayImageAction*)GetAction("Image"); 
	Texture* imageTexture = imageTool->GetTexture(); 

	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";
		if (imageTexture == nullptr)
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");
		else
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource(imageTool->GetTexture()->GetName());

		float aspect_ratio = imageTexture->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)imageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", imageTexture->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetHeigth());

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
					imageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(imageTexture, imageTexture->GetName());
				}
				else
				{
					imageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
				}

				strcpy(buf, path);
				flog("Player Opened %s", path);
			}

		}

		ImGui::PopStyleColor(2);
		ImGui::EndChild();
		ImGui::PopFont();
	}
}

//void FlyObject::/*DrawToolsList*/()
//{
//	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
//	ImGui::Text("Add Object Actions: ");
//	ImGui::PopFont();
//
//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
//
//	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));
//
//	if (showImageTool)
//	{
//		ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription("Image");
//		if (DrawToolSelectable(toolInfo)) selectedInList = AT_IMAGE;
//	}
//
//	ImGui::EndChild();
//
//	ImGui::PopStyleVar();
//	ImGui::PopStyleColor();
//}
