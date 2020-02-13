#ifndef _FONT_IMPORTER_H_
#define _FONT_IMPORTER_H_

#include "Font.h"

struct FontImporter {

private:
	static FontImporter* instance;
	FontImporter();

public:
	static FontImporter* getInstance();
	~FontImporter();

	static void Delete();
	static Font* LoadFont(std::string path);

private: 
	FT_Library freeType;
};

#endif // !_IMAGE_IMPORTER_H_


