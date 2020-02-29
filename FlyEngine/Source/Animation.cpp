#include "Animation.h"
#include "Animation.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "imgui.h"

Animation::Animation()
{
	name = "";
}

Animation::~Animation()
{
}

void Animation::BuildAnimation(string loadingPath)
{
	Clear();

	if (MyFileSystem::getInstance()->IsFolder(loadingPath))
	{
		vector<string> animationFramesFiles; 
		MyFileSystem::getInstance()->GetFilesInDirectory(loadingPath.c_str(), animationFramesFiles);

		for (auto& currentFrameFile : animationFramesFiles)
		{
			string textureResourceName = MyFileSystem::getInstance()->DeleteFileExtension(currentFrameFile);
			Texture* frameTexture = (Texture*)ResourceManager::getInstance()->GetResource(textureResourceName, RESOURCE_TEXTURE);

			if (frameTexture != nullptr)
			{
				framesAmount++;
				frameTexturesList.push_back(frameTexture);
				flog("Load Animation File: %s", currentFrameFile.c_str());
			}			
		}
	}
}

void Animation::AddFrame(int framePos)
{
	
}

Texture* Animation::GetFrame(int pos)
{
	int count = 0;
	for (auto& currentFrame : frameTexturesList)
	{
		if (count == pos)
			return currentFrame; 

		count++;
	}

	return nullptr; 
}

void Animation::Clear()
{
	frameTexturesList.clear();
	framesAmount = 0;
}

void Animation::DrawAddFramePopup()
{
	
}

int Animation::GetFramesAmount()
{
	return framesAmount; 
}

float Animation::GetAnimationSpeed()
{
	return animationSpeed;
}

void Animation::SetAnimationSpeed(float newSpeed)
{
	animationSpeed = newSpeed;
}

string Animation::GetName()
{
	return name;
}

void Animation::SetName(string newName)
{
	name = newName; 
}
