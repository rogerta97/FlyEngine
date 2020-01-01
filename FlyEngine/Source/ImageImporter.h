#ifndef _IMAGE_IMPORTER_H_
#define _IMAGE_IMPORTER_H_

#include <string>

class Texture; 
struct ImageImporter {

private:
	static ImageImporter* instance;
	ImageImporter();

public:
	static ImageImporter* getInstance();
	~ImageImporter();

	static void Delete();
	static Texture* LoadTexture(std::string path, bool flipImage);
	static void LoadResourcesRecursive(std::string path);
};

#endif // !_IMAGE_IMPORTER_H_


