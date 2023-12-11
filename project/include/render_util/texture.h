#ifndef TEXTURE_H
#define TEXTURE_H

// #define STB_IMAGE_IMPLEMENTATION
#include "../other/stb/stb_image.h"

class Texture
{
public:
	Texture() {};

	unsigned int  load_texture(const char* path, bool flip = true);
	void apply_texture();

private:
	int width; 
	int height; 
	int nChannels;
};

#endif // TEXTURE_H