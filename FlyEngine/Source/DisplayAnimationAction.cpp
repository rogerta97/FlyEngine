#include "DisplayAnimationAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "MyFileSystem.h"
#include "ImageImporter.h"
#include "Animation.h"
#include "FlyObject.h"
#include "ResourceManager.h"

#include "mmgr.h"

DisplayAnimationAction::DisplayAnimationAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_ANIMATION;
	isVisual = false;
	parentObject = _parentObject; 
	isSequential = false; 

	SetActionName("Display Animation");
	SetToolDescription("This should be the description of the animation action");

	animation = new Animation(); 
	currentFrame = -1; 
}

DisplayAnimationAction::~DisplayAnimationAction()
{
}

void DisplayAnimationAction::CopyData(DisplayAnimationAction* otherAction)
{
	if (otherAction->GetAnimation() != nullptr)
	{
		SetAnimation(otherAction->GetAnimation());
	}

	animPlayMode = otherAction->animPlayMode; 
}

void DisplayAnimationAction::AttachToImage(DisplayImageAction* imageToAttach)
{
	if (imageToAttach != nullptr)
		screenImageAction = imageToAttach;
}

void DisplayAnimationAction::Init()
{
	
}

void DisplayAnimationAction::DrawActionOccurenceCheckboxes()
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

void DisplayAnimationAction::Update(float dt)
{
	if (animationState == ANIMATION_PLAY) 
	{
		animationTime += App->GetDeltaTime(); 
		if (animationTime > animation->GetFramesInterval())
		{
			animationTime = 0;

			// Next Frame 
			if (NextFrame() && animPlayMode == ANIMATION_ONE_TIME)
				Stop();
		
			// Updates Viewport Image
			if(screenImageAction != nullptr)
				screenImageAction->SetTexture(animation->GetFrameByPos(currentFrame));		
		}
	}
}

void DisplayAnimationAction::CleanUp()
{
	Action::CleanUp();

	animation->CleanUp(); 
	delete animation; 
	animation = nullptr; 
}

void DisplayAnimationAction::DoAction()
{
	if (!isHolderInfo)
	{
		Play(); 
	}
	else if(!isDataAttached && actionClass == ActionClass::ACTION_CLASS_SEQUENTIAL)
	{
		//// Get The Fixed Animation (The one displaying) 
		DisplayAnimationAction* fixedAnimAction = (DisplayAnimationAction*)parentObject->GetAction(ACTION_DISPLAY_ANIMATION);

		if (fixedAnimAction)
			fixedAnimAction->CopyData(this);
		else
		{
			isDedicatedImage = true; 
			DisplayImageAction* animCanvasAction = parentObject->AddDisplayImageAction("Null", true);
			animCanvasAction->CreateImage("None");
			animCanvasAction->SetDrawIfSequential(true);
			animCanvasAction->fromAnimation = true; 

			this->AttachToImage(animCanvasAction); 
		}

		isDataAttached = true; 

		Play();
	}
}

void DisplayAnimationAction::StopAction()
{
	/*if (App->isEngineInPlayMode && isDedicatedImage)
	{
		parentObject->DeleteAction(ACTION_DISPLAY_IMAGE); 
	}

	Stop(); */
}

void DisplayAnimationAction::Play()
{
	currentFrame = 0; 
	animationState = ANIMATION_PLAY; 

	if (animPlayMode == AnimationPlayMode::ANIMATION_LOOP)
		SetActionCompleted(true);
}

void DisplayAnimationAction::Stop()
{
	animationState = ANIMATION_STOP;
	currentFrame = 0; 
}

// Will send true when it's the last frame 
bool DisplayAnimationAction::NextFrame()
{
	if (currentFrame == -1)	
		return false;

	if (currentFrame < animation->GetFramesAmount() - 1)
	{
		currentFrame++;
	}
	else
	{
		if(animPlayMode == ANIMATION_ONE_TIME)
			SetActionCompleted(true);

		currentFrame = 0; 
		return true; 
	}

	//flog("Next Frame: %d", currentFrame);
	return false; 
}

void DisplayAnimationAction::AddFrame(Texture* newFrame)
{
	animation->AddFrame(newFrame);
	currentFrame = animation->GetFramesAmount() - 1;

	if(!isHolderInfo && canChangeCanvas)
		screenImageAction->SetTexture(newFrame);
}

void DisplayAnimationAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string actionSerializeSection;

	if (!literalStr)
		actionSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		actionSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, actionSerializeSection);
	Action::SaveOccurrence(jsonObject, actionSerializeSection);

	if(animation == nullptr)
		json_object_dotset_number(jsonObject, string(actionSerializeSection + "FramesAmount").c_str(), 0);
	else
	{
		json_object_dotset_number(jsonObject, string(actionSerializeSection + "FramesAmount").c_str(), animation->GetFramesAmount());
		json_object_dotset_number(jsonObject, string(actionSerializeSection + "PlayMode").c_str(), animPlayMode);
		json_object_dotset_number(jsonObject, string(actionSerializeSection + "Speed").c_str(), animation->GetSpeed());

		int count = 0; 
		string saveFrameStr = actionSerializeSection + "Frames."; 
		while (count < animation->GetFramesAmount())
		{
			Texture* currentFrame = animation->GetFrameByPos(count); 
			string saveStr = saveFrameStr + "Frame_" + to_string(count) + "."; 
			json_object_dotset_string(jsonObject, string(saveStr + "TextureName").c_str(), currentFrame->GetName().c_str());
			count++; 
		}	
	}
}

void DisplayAnimationAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Animation Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int framesAmount = 0;

		if (animation)
			framesAmount = animation->GetFramesAmount();

		DrawActionOccurenceCheckboxes();

		ImGui::Separator();

		float squareSize = 250;
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, squareSize + 5);

		DrawUISettingsLeftColumn(squareSize);

		ImGui::NextColumn();

		DrawSettingsRightColumn();
	}
	
}

void DisplayAnimationAction::DrawUICharacterSettings(string previewTitle)
{
	int framesAmount = 0;

	if (animation)
		framesAmount = animation->GetFramesAmount();

	ImGui::Spacing(); 
	float squareSize = 250;
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, squareSize + 5);

	DrawUISettingsLeftColumn(squareSize, previewTitle);

	ImGui::NextColumn();

	DrawSettingsRightColumn();
}

void DisplayAnimationAction::DrawAddFramePopup()
{
	if (ImGui::BeginPopup("add_frame_popup"))
	{
		static int currentSelection = 0;

		//static Texture* iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");

		//ImGui::Image((ImTextureID)iconTexture->GetTextureID(), ImVec2(23, 23), ImVec2(1,0), ImVec2(0, 1));
//		ImGui::SameLine(); 
		ImGui::Combo("##LoadFrameType", &currentSelection, "Load Image\0Load Folder");

		//if (currentSelection == 0)
		//	iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon"); 
		//else if (currentSelection == 1)
		//	iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("FolderIcon");

		string hint = "";
		if (ImGui::Button("Search##SearchAnimFrame"))
		{
			if (currentSelection == 0)
			{
				hint = "File...";
				ImGui::OpenPopup("print_image_selection_popup"); 
			}

			else if (currentSelection == 1)
			{
				hint = "Folder...";
				ImGui::OpenPopup("print_folder_selection_popup");
			}
			
		}

		static char searchFileBuffer[256];

		if (currentSelection == 0)
		{
			Resource* selectedResource = ResourceManager::getInstance()->PrintImagesSelectionPopup();

			if (selectedResource != nullptr)			
				strcpy(searchFileBuffer, selectedResource->GetName().c_str());			
		}
		else if (currentSelection == 1)
		{
			string folderPath = ResourceManager::getInstance()->PrintFolderSelectionPopup(std::string(MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Animations"));
		
			if (folderPath != "")			
				strcpy(searchFileBuffer, folderPath.c_str());			
		}

		ImGui::SameLine();
		ImGui::InputTextWithHint("", hint.c_str(), searchFileBuffer, IM_ARRAYSIZE(searchFileBuffer));

		ImGui::Separator(); 
		if (ImGui::SmallButton("Load Frame/s"))
		{
			if (currentSelection == 0)
			{
				Texture* newFrameTexture = (Texture*)ResourceManager::getInstance()->GetResource(searchFileBuffer);

				animation->AddFrame(newFrameTexture);
				currentFrame = animation->GetFramesAmount() - 1;

				if(screenImageAction != nullptr && !isSequential && canChangeCanvas)
					screenImageAction->SetTexture(newFrameTexture);

				ImGui::CloseCurrentPopup();
				ImGui::EndPopup(); 
				return;
			}
			else if (currentSelection == 1)
			{
				string folderPath = searchFileBuffer; 
				
				vector<string> frameFilesList; 
				MyFileSystem::getInstance()->GetFilesInDirectory(folderPath.c_str(), frameFilesList, false); 

				if (!frameFilesList.empty())
				{
					for (auto& currentFrameFile : frameFilesList)
					{
						string fileWithoutExtension = MyFileSystem::getInstance()->DeleteFileExtension(currentFrameFile);
						Texture* newAnimFrameTexture = (Texture*)ResourceManager::getInstance()->GetResource(fileWithoutExtension);

						if (newAnimFrameTexture == nullptr)
						{
							string imagePath = folderPath + "\\" + currentFrameFile;
							newAnimFrameTexture = ImageImporter::getInstance()->LoadTexture(imagePath.c_str(), false);
						}

						animation->AddFrame(newAnimFrameTexture);
						currentFrame = animation->GetFramesAmount() - 1;

						if (screenImageAction != nullptr && !isSequential && canChangeCanvas)
							screenImageAction->SetTexture(newAnimFrameTexture);
					}

					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return;
				}


			}
		}

		ImGui::EndPopup();
	}
}

void DisplayAnimationAction::DrawSettingsRightColumn()
{
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");

	ImGui::PushID(0);
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(25, 25)))
	{
		ImGui::OpenPopup("add_frame_popup"); 
	}

	DrawAddFramePopup(); 

	ImGui::PopID();

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(25, 25)))
	{

	}

	ImGui::SameLine();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Animation Frames:");
	ImGui::PopFont();
	
	//static char animationNameBuffer[256];

	//if (animation != nullptr)
	//{
	//	strcpy(animationNameBuffer, animation->GetName().c_str());
	//}

	//if (ImGui::InputTextWithHint("Name", "None...", animationNameBuffer, IM_ARRAYSIZE(animationNameBuffer)))
	//{
	//	animation->SetName(animationNameBuffer);
	//}

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationFrames", ImVec2(ImGui::GetContentRegionAvail().x, 283));

	if (animation && animation->GetFramesAmount() > 0)
	{
		int count = 0; 
		while (count < animation->GetFramesAmount())
		{
			string frameName = to_string(count + 1) + ". " +animation->GetFrameByPos(count)->GetName(); 

			if (count == 0)
			{
				INC_CURSOR_7;
			}
			else
				INC_CURSOR_X_7;

			if (ImGui::Selectable(frameName.c_str()))
			{
				currentFrame = count;
			}

			Texture* arrowUp = (Texture*)ResourceManager::getInstance()->GetResource("ArrowUpWhite");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 60);
			ImGui::ImageButton((ImTextureID)arrowUp->GetTextureID(), ImVec2(15, 15));

			Texture* arrowDown = (Texture*)ResourceManager::getInstance()->GetResource("ArrowDownWhite");
			ImGui::SameLine();
			ImGui::ImageButton((ImTextureID)arrowDown->GetTextureID(), ImVec2(15, 15));

			count++;
		}
	}
	else
	{
		INC_CURSOR_X_7;
		ImGui::PushFont(App->moduleImGui->rudaBlackGiant);
		ImGui::TextColored(ImVec4(1,1,1,0.04f), "Empty");
		ImGui::PopFont();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void DisplayAnimationAction::DrawUISettingsLeftColumn(float squareSize, string previewTitle)
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text(previewTitle.c_str());
	ImGui::PopFont();

	// Current Frame Preview
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationPreview", ImVec2(squareSize, 175));

	Texture* frameTexture = nullptr;

	if (animation->GetFramesAmount() <= 0)
		frameTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");

	if (currentFrame != -1)
	{
		if (App->isEngineInPlayMode)
			frameTexture = animation->GetFrameByPos(0);
		else
			frameTexture = animation->GetFrameByPos(currentFrame);
	}

	App->moduleManager->DrawImageFitInCenter(frameTexture);

	ImGui::EndChild();
	ImGui::PopStyleColor();


	// Frame Controls
	ImVec2 centerControlls = ImVec2(ImGui::GetContentRegionAvail().x / 2, 30);
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationControls", ImVec2(squareSize, 115));

	ImTextureID playPauseIconID = 0; 

	if (animationState == ANIMATION_PLAY)
	{
		Texture* placeholder = (Texture*)ResourceManager::getInstance()->GetResource("StopIcon"); 
		playPauseIconID = (ImTextureID)placeholder->GetTextureID(); 
	}
	else if (animationState == ANIMATION_STOP)
	{
		Texture* placeholder = (Texture*)ResourceManager::getInstance()->GetResource("PlayIcon");
		playPauseIconID = (ImTextureID)placeholder->GetTextureID();
	}


	Texture* previewFrame = (Texture*)ResourceManager::getInstance()->GetResource("PreviewFrameIcon");
	ImGui::SetCursorPos(ImVec2(centerControlls.x - 80, centerControlls.y - 25));
	if (ImGui::ImageButton((ImTextureID)previewFrame->GetTextureID(), ImVec2(25, 25)))
	{
		
	}

	INC_CURSOR_4;
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(centerControlls.x - 35, centerControlls.y - 25));
	if (ImGui::ImageButton(playPauseIconID, ImVec2(25, 25)))
	{
		if (animationState == ANIMATION_STOP)
			Play();
		else if (animationState == ANIMATION_PLAY)
		{
			Stop(); 

			if(!isHolderInfo)
				screenImageAction->SetTexture(animation->GetFrameByPos(0));	
		}
	}

	ImGui::SameLine();
	Texture* nextFrame = (Texture*)ResourceManager::getInstance()->GetResource("NextFrameIcon");
	ImGui::SetCursorPos(ImVec2(centerControlls.x + 10, centerControlls.y - 25));
	if (ImGui::ImageButton((ImTextureID)nextFrame->GetTextureID(), ImVec2(25, 25)))
	{

	}

	ImGui::SameLine();
	ImGui::PushFont(App->moduleImGui->rudaBlackHuge);

	int displayCurrentFrame = currentFrame + 1; 

	if (currentFrame == -1)
		displayCurrentFrame = 0; 

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 7, ImGui::GetCursorPosY() + 2));
	ImGui::TextColored(ImVec4(0.43137f, 0.56863f, 0.80392f, 1.0f), "%d/%d", displayCurrentFrame, animation->GetFramesAmount());
	ImGui::PopFont();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() +-7, ImGui::GetCursorPosY() + -2));
	ImGui::Separator();

	int speed = animation->GetSpeed();
	
	INC_CURSOR_4;
	if (ImGui::InputInt("Speed", &speed, 1, 3))	
		animation->SetSpeed(speed); 	

	INC_CURSOR_4;
	int playModeSelected = animPlayMode;
	if (ImGui::Combo("Play Mode", &playModeSelected, "Loop\0One Time\0"))
		animPlayMode = (AnimationPlayMode)playModeSelected; 

	ImGui::EndChild();
	ImGui::PopStyleColor();
}


Animation* DisplayAnimationAction::GetAnimation()
{
	return animation;
}

void DisplayAnimationAction::SetAnimation(Animation* newAnimation)
{
	if (newAnimation != nullptr)
	{
		animation = newAnimation; 
	}
}

int DisplayAnimationAction::GetCurrentFrame()
{
	return currentFrame;
}

void DisplayAnimationAction::SetCurrentFrame(int _currentFrame)
{
	currentFrame = _currentFrame;
}

void DisplayAnimationAction::SetVisible(bool newVisible)
{
	Action::SetVisible(newVisible); 

	if (screenImageAction != nullptr)
		screenImageAction->SetVisible(newVisible);	
}
