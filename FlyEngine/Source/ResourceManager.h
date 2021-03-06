#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Resource.h"
#include "Globals.h"

using namespace std; 


class Font;
class AudioClip;
class MusicTrack;
class Texture; 
struct ResourceManager
{
private:
	static ResourceManager* instance;
	ResourceManager();

public:
	static ResourceManager* getInstance();
	~ResourceManager();
	
	static void CleanUp();

	// Utility --------------------------------
	static bool AddResource(Resource* newResource, std::string name);
	static ResourceType GetResourceTypeFromExtension(FileExtension ext); 

	static Resource* GetResource(std::string resourceName, ResourceType type = ResourceType::RESOURCE_null);
	static Resource* GetResource(UID resourceUID, ResourceType type = ResourceType::RESOURCE_null);
	static vector<Resource*> GetResources(ResourceType type);
	static list<Resource*>& GetResourceList();
	static Resource* GetResourceByPath(std::string resourcePath);

	// Get Specific Resource 
	static Texture* GetTexture(string resourceName); 
	static Font* GetFont(string resourceName);
	static AudioClip* GetAudioClip(string resourceName);
	static MusicTrack* GetMusicTrack(string resourceName);

	static bool ExistResource(Resource* checkResource);
	static bool ExistResourceUID(UID resourceUID);
	static bool ExistResourcePath(std::string resourcePath);

	// Selection Popups 
	static Resource* PrintImagesSelectionPopup(); 
	static Resource* PrintSoundsSelectionPopup(); 
	static Resource* PrintSoundsSelectionPopup2(); 
	static Resource* PrintSoundsSelectionPopup3(); 
	static Resource* PrintMusicSelectionPopup(); 
	static Resource* PrintFontSelectionPopup(); 
	static string PrintFolderSelectionPopup(string parentFolder);

	// User Resources Loads 
	static void LoadResource(string newResourcePath, ResourceType forceType = ResourceType::RESOURCE_null);
	static void LoadAllFilesFromFolder(string path, ResourceType forceType = ResourceType::RESOURCE_null);
	static void LoadAllGameResources(); 

	// Engine Resources Loads
	static void LoadEngineIconsResources(); 

private: 
	std::list<Resource*> resourceList; 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


