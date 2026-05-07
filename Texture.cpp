#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* path):Tex_id(0)
{
	unsigned char* data = stbi_load(path, &Tw, &Th, &ch, 0);
	if (data)
	{
		int format = (ch == 4) ? GL_RGBA : GL_RGB;
		glGenTextures(1, &Tex_id);
		glBindTexture(GL_TEXTURE_2D, Tex_id);

		glTexImage2D(GL_TEXTURE_2D, 0, format, Tw, Th, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		std::cout << "loading successed" << std::endl;
	}
	else
	{
		std::cout << "loading false" << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &Tex_id);
}

GLuint Texture::get_tex_id()
{
	return Tex_id;
}

void Texture::bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, Tex_id);
}
