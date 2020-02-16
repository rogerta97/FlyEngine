#include "Font.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

Font::Font() : Resource(RESOURCE_FONT)
{

}

Font::~Font()
{
	FT_Done_Face(fontFace);
}

void Font::CreateCharactersFromFace()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
		{
			FLY_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		// Generate texture
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			fontFace->glyph->bitmap.width,
			fontFace->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			fontFace->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			textureID,
			float2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			float2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			fontFace->glyph->advance.x / 64
		};

		fontCharacters.insert(std::pair<GLchar, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Character Font::GetCharacter(char getChar)
{
	return fontCharacters[getChar]; 
}

void Font::SetFace(FT_Face newFace, int fontSize)
{
	fontFace = newFace; 
	FT_Set_Pixel_Sizes(fontFace, 0, fontSize);
	CreateCharactersFromFace();
}
