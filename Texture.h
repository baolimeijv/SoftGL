# pragma once

#include <glad/glad.h>

class Texture
{
private:
	int Tw, Th,ch;
	unsigned int Tex_id;
public:
	Texture(const char* path);
	~Texture();
	GLuint get_tex_id();
	void bind(unsigned int unit);
};