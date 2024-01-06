#ifndef TEXTURE_H
#define TEXTURE_H

// #define STB_IMAGE_IMPLEMENTATION
#include "../other/stb/stb_image.h"
#include "string"
#include "../other/glad/glad.h"
#include <filesystem>
#include "iostream"

class Texture
{
public:
	Texture() {};

	void texture_from_file(const char *path, const std::string &directory);
	void load_cubemap_texture(std::filesystem::path path);
	unsigned int id;
private:
	int _width; 
	int _height; 
	int _n_channels;
};

#endif // TEXTURE_H
