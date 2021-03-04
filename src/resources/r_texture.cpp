#include "r_texture.hpp"

RTexture::RTexture()
: Resource{}
{
}

RTexture::RTexture(std::string const& path)
: Resource{}
{
	this->loadFromFile(path);
}

RTexture::~RTexture()
{
}

void 
RTexture::loadTexture( std::string const& path ) 
{
    // int width = 0, height = 0, channels = 0;
	// Image imagedata = SOIL_load_image("assets/plantilla.png", &width, &height, 0, SOIL_LOAD_RGB);

	// if(imagedata) {
	// 	LOG("SUCCESSFULLY LOADED IMAGE!!!!");
	// 	LOG("width: " << width);
	// 	LOG("height: " << height);
	// 	LOG("channels: " << channels);

	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
    //     glGenerateMipmap(GL_TEXTURE_2D);
	// } else {
	// 	LOG("[ERROR]:: IMAGE COULDN'T LOAD");
	// }
}

void 
RTexture::loadFromFile( std::string const& path ) 
{
    this->setName(path);
    this->loadTexture(path);
}
