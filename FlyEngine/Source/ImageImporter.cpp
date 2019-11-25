#include "ImageImporter.h"
#include "Texture.h"
#include "Application.h"
#include "MyFileSystem.h"
#include "mmgr.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

ImageImporter* ImageImporter::instance = 0;

ImageImporter::ImageImporter()
{
	ilInit();
	iluInit(); 
	ilutInit(); 
}

ImageImporter* ImageImporter::getInstance()
{
	if (instance == 0)
		instance = new ImageImporter();

	return instance;
}

ImageImporter::~ImageImporter() 
{
	
}

void ImageImporter::Delete()
{
	delete instance; 
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

	success = ilLoadImage(path.c_str());

	FLY_ERROR("%s", iluErrorString(success));

	if (success)
	{
		std::string textureNameGenerated = MyFileSystem::getInstance()->GetLastPathItem(path, false);
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
			new_tex->SetHeigth(ilGetInteger(IL_IMAGE_HEIGHT));

			new_tex->SetPath(path.c_str());
			new_tex->SetName(textureNameGenerated.c_str());

			new_tex->Bind();
			new_tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_tex->GetWidth(), new_tex->GetHeigth(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			new_tex->UnBind();
		}
	}
	else
		new_tex = nullptr; 

	return new_tex;
}
