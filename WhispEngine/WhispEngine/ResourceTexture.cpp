#include "ResourceTexture.h"
#include "Application.h"
#include "MaterialImporter.h"

// Devil ---------------------------------------------------------
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

ResourceTexture::ResourceTexture(const uint64& uid) : Resource(uid, Resource::Type::TEXTURE)
{
}


ResourceTexture::~ResourceTexture()
{
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
	/*if (App->dummy_file_system->Exists(path) == false) {
		// TODO?: Recreate library
	}*/

	ilGenImages(1, &buffer_id);
	ilBindImage(buffer_id);

	ilutRenderer(ILUT_OPENGL);  // Switch the renderer

	if (!ilLoadImage(file.c_str())) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", file.c_str(), ilGetString(error));
	}
	else {
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

	ilDeleteImages(1, &buffer_id);
	return true;
}

bool ResourceTexture::FreeMemory()
{
	glDeleteTextures(1, &buffer_id);
	buffer_id = 0u;

	return true;
}
