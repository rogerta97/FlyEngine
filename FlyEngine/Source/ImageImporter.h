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
	void Delete();

	Texture* LoadTexture(std::string path, bool flipImage); 
};

#endif // !_IMAGE_IMPORTER_H_


