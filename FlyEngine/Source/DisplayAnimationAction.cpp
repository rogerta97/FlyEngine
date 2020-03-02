#include "DisplayAnimationAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "Texture.h"
#include "MyFileSystem.h"
#include "Animation.h"
#include "ResourceManager.h"

DisplayAnimationAction::DisplayAnimationAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_ANIMATION;
	isVisual = false;
	parentObject = _parentObject; 

	SetActionName("Display Animation");
	SetToolDescription("This should be the description of the animation action");

	animation = new Animation(); 
	currentFrame = -1; 
	animation->BuildAnimation(string(MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Animations\\TestAnim_001").c_str());

}

DisplayAnimationAction::~DisplayAnimationAction()
{
}

void DisplayAnimationAction::Init()
{
	
}

void DisplayAnimationAction::Update(float dt)
{
	static bool kidding = false; 
	if (animationState == ANIMATION_PLAY) 
	{
		animationTime += App->GetDeltaTime(); 
		if (animationTime > animation->GetFramesInterval())
		{
			animationTime = 0;
			NextFrame(); 
		}
	}
}

void DisplayAnimationAction::CleanUp()
{
	delete animation; 
	animation = nullptr; 
}

void DisplayAnimationAction::Play()
{
	animationState = ANIMATION_PLAY; 
}

void DisplayAnimationAction::Stop()
{
	animationState = ANIMATION_STOP;
	currentFrame = 0; 
}

void DisplayAnimationAction::NextFrame()
{
	if (currentFrame == -1)	
		return;

	if (currentFrame < animation->GetFramesAmount())
	{
		currentFrame++;
	}
	else
		currentFrame = 0; 

	flog("Next Frame: %d", currentFrame);
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
	int framesAmount = 0;

	if (animation)
		framesAmount = animation->GetFramesAmount();

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::Text("Frames Amount:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.43137f, 0.56863f, 0.80392f, 1.0f), "%d", framesAmount);
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::Text("Current Frame:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.43137f, 0.56863f, 0.80392f, 1.0f), "%d", currentFrame);
	ImGui::PopFont();

	ImGui::Spacing();
	ImGui::Separator();

	float squareSize = 200;
	ImGui::Columns(2); 
	ImGui::SetColumnWidth(0, squareSize + 5);

	DrawUISettingsLeftColumn(squareSize);

	ImGui::NextColumn(); 

	DrawSettingsRightColumn();
}

void DisplayAnimationAction::DrawAddFramePopup()
{
	if (ImGui::BeginPopup("add_frame_popup"))
	{
		static int currentSelection = 0;

		Texture* iconTexture = 0; 
		if (currentSelection == 0)
			iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon"); 
		else if (currentSelection == 1)
			iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("FolderIcon");

		ImGui::Image(0, ImVec2(23, 23));
		ImGui::SameLine(); 
		ImGui::Combo("##LoadFrameType", &currentSelection, "Load Image\0Load Folder");

		string hint = "";
		if (ImGui::Button("Search##SearchAnimFrame"))
		{
			if (currentSelection == 0)
			{
				hint = "File...";
				ImGui::OpenPopup("print_image_selection_popup"); 
			}

			else if (currentSelection == 0)
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
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup(); 
				return;
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
	ImGui::BeginChild("AnimationFrames", ImVec2(ImGui::GetContentRegionAvail().x, 200));

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

void DisplayAnimationAction::DrawUISettingsLeftColumn(float squareSize)
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Preview:");
	ImGui::PopFont();

	// Current Frame Preview
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationPreview", ImVec2(squareSize, 175));

	if (currentFrame != -1)
	{
		Texture* frameTexture = animation->GetFrameByPos(currentFrame); 
		App->moduleManager->DrawImageFitInCenter(frameTexture);
	}
	
	ImGui::EndChild();
	ImGui::PopStyleColor();

	//flog("%f", App->GetDeltaTime());

	// Frame Controls
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("AnimationControls", ImVec2(squareSize, 70));

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

	INC_CURSOR_4;
	if (ImGui::ImageButton(playPauseIconID, ImVec2(25, 25)))
	{
		if (animationState == ANIMATION_STOP)
			Play();
		else if (animationState == ANIMATION_PLAY)
			Stop(); 
	}

	ImGui::SameLine();
	if (ImGui::ImageButton(0, ImVec2(25, 25)))
	{
		
	}

	ImGui::SameLine();
	if (ImGui::ImageButton(0, ImVec2(25, 25)))
	{

	}

	int speed = animation->GetSpeed();

	if (ImGui::InputInt("Speed", &speed, 1, 3))	
		animation->SetSpeed(speed); 	

	ImGui::EndChild();
	ImGui::PopStyleColor();
}


Animation* DisplayAnimationAction::GetAnimation()
{
	return animation;
}

int DisplayAnimationAction::GetCurrentFrame()
{
	return currentFrame;
}

void DisplayAnimationAction::SetCurrentFrame(int _currentFrame)
{
	currentFrame = _currentFrame;
}
