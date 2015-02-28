#include "texturemaker.hpp"

fea::Texture makeTexture(std::string path)
{
    fea::Texture texture;
    uint32_t width;
    uint32_t height;

    std::vector<unsigned char> image; //the raw pixels

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error)
    {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(4);
    }

    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    texture.create(width, height, &image[0], false, true);
    return texture;
}
