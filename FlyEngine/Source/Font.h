#ifndef _FONT_H_
#define _FONT_H_

#include "Resource.h"
#include "OpenGL.h"
#include "Math/float2.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H  

struct Character 
{
	GLint     textureID;  // ID handle of the glyph texture
	float2	  size;       // Size of glyph
	float2    bearing;    // Offset from baseline to left/top of glyph
	GLint     Advance;    // Offset to advance to next glyph
};

class Font : public Resource
{
public:
	Font();
	~Font();

	Character GetCharacter(char getChar);
	void SetFace(FT_Face newFace, int fontSize = 15);

private: 
	void CreateCharactersFromFace(); 

public: 
	FT_Face fontFace; 
	int fontSize = 0; 

private: 
	std::map<GLchar, Character> fontCharacters;

};

#endif
