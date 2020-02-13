#ifndef _FONT_H_
#define _FONT_H_

#include "Resource.h"
#include "Math/float2.h"
#include "OpenGL.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H  

struct Character 
{
	GLint     TextureID;  // ID handle of the glyph texture
	float2	  Size;       // Size of glyph
	float2    Bearing;    // Offset from baseline to left/top of glyph
	GLint     Advance;    // Offset to advance to next glyph
};

class Font : public Resource
{
public:
	Font();
	~Font();

	void SetFace(FT_Face newFace, int fontSize = 15);

private: 
	void CreateCharactersFromFace(); 

public: 
	FT_Face fontFace; 

private: 
	std::map<GLchar, Character> fontCharacters;

};

#endif
