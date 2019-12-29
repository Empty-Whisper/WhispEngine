#include "ResourceTexture.h"
#include "Application.h"
#include "MaterialImporter.h"

#include "glew-2.1.0/include/GL/glew.h"

// Devil ---------------------------------------------------------
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#include "mmgr/mmgr.h"

ResourceTexture::ResourceTexture(const uint64& uid) : Resource(uid, Resource::Type::TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
	if (references > 0u)
		FreeMemory();
	references = 0u;
}

bool ResourceTexture::Set(const uint & width, const uint & height, const uint & id)
{
	this->width = width;
	this->height = height;
	this->buffer_id = id;
	
	return true;
}

bool ResourceTexture::LoadInMemory()
{
	/*if (App->file_system->Exists(path) == false) {
		// TODO?: Recreate library
	}*/
	ILuint image_id = 0;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	ilutRenderer(ILUT_OPENGL);  // Switch the renderer

	if (!ilLoadImage(resource_path.c_str())) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", file.c_str(), ilGetString(error));
	}
	else {

		Set(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilutGLBindTexImage());
		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, buffer_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0); // Deselect buffer
	}

	ilDeleteImages(1, &image_id);
	return true;
}

bool ResourceTexture::FreeMemory()
{
	glDeleteTextures(1, &buffer_id);
	buffer_id = 0u;

	return true;
}
