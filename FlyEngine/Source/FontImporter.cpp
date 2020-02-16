#include "FontImporter.h"
#include "Texture.h"
#include "Application.h"
#include "MyFileSystem.h"
#include "mmgr.h"

#pragma comment(lib, "3rdParty/FreeType/win32/freetype.lib")

FontImporter* FontImporter::instance = 0;

FontImporter::FontImporter()
{
	if (FT_Init_FreeType(&freeType))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
}

FontImporter* FontImporter::getInstance()
{
	if (instance == 0)
		instance = new FontImporter();

	return instance;
}

FontImporter::~FontImporter()
{
	FT_Done_FreeType(freeType);
}

void FontImporter::Delete()
{
	delete instance;
}

Font* FontImporter::LoadFont(std::string path, int fontSize)
{
	// Create Font Face 
	FT_Face face;
	if (FT_New_Face(instance->freeType, path.c_str(), 0, &face))
	{
		FLY_ERROR("ERROR::FREETYPE: Failed to load font");
		return nullptr; 
	}

	Font* newFont = new Font(); 
	newFont->SetFace(face, fontSize);
	return newFont;
}
