#include "ImageImporter.h"
#include "Texture.h"
#include "Application.h"
#include "FileSystem.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

ImageImporter::ImageImporter()
{
}

ImageImporter::~ImageImporter()
{
}

Texture* ImageImporter::LoadTexture(std::string path, bool flipImage = false)
{
	Texture* new_tex = new Texture();

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	//Check if the texture existed before based on the name on its path
	std::string new_name = FileSystem::getInstance()->GetLastPathItem(path, true);

	success = ilLoadImage((const wchar_t*)path.c_str());

	if (success)
	{
		//Get data of the image
		ILinfo image_info;
		iluGetImageInfo(&image_info);

		if (flipImage == true)
		{
			if (image_info.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();
		}


		int a = ilGetInteger(IL_PNG_ALPHA_INDEX);

		if (success)
		{
			new_tex->SetTextureID(new_tex->CreateBuffer());
			new_tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			new_tex->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));

			new_tex->SetPath(path.c_str());
			new_tex->name = new_name;

			new_tex->Bind();
			new_tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_tex->GetWidth(), new_tex->GetHeight(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			new_tex->UnBind();
		}
	}

	return new_tex;
}
