#include "Animation.h"
#include "Animation.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "imgui.h"

Animation::Animation()
{
	name = "";
	animationSpeed = 1.0f;
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

void Animation::AddFrame(Texture* newFrame)
{
	if (newFrame != nullptr)
	{
		frameTexturesList.push_back(newFrame); 
		framesAmount++;
	}
}

Texture* Animation::GetFrameByPos(int pos)
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

int Animation::GetFramePos(Texture* frame)
{
	int count = 0; 
	for (auto& currentTexture : frameTexturesList)
	{
		if (currentTexture == frame)
			return count; 

		count++;
	}

	return -1; 
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

float Animation::GetFramesInterval()
{
	return 1 / animationSpeed; 
}

float& Animation::GetSpeed()
{
	return animationSpeed;
}

void Animation::SetSpeed(float newSpeed)
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
