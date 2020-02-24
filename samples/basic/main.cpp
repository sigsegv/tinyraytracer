#include <tgaimage.h>

void render() {
    const int width = 1024;
    const int height = 768;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            framebuffer.set(i, j, TGAColor(static_cast<unsigned char>(255 * (j / static_cast<float>(height))), 
                static_cast<unsigned char>(255 * (i / static_cast<float>(width))), static_cast<unsigned char>(0), 255));
        }
    }
    framebuffer.write_tga_file("framebuffer.tga");
}

int main(int argc, char** argv)
{
    render();
    return 0;
}
