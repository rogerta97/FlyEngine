#ifndef _IMAGE_IMPORTER_H_
#define _IMAGE_IMPORTER_H_

#include <string>

#pragma comment(lib, "DevILWindowsSDK/libx86/DevILç")
#pragma comment(lib, "DevILWindowsSDK/libx86/ILU.lib")
#pragma comment(lib, "DevILWindowsSDK/libx86/ILUT.lib")

class Texture; 
class ImageImporter
{
public: 
	ImageImporter(); 
	~ImageImporter(); 

	Texture* LoadTexture(std::string path, bool flipImage); 
};

#endif // !_IMAGE_IMPORTER_H_


