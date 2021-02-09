#include <math.h>
#include <tgaimage.h>
#include <vector3.hpp>

using vector3f = vector3<float>;

bool ray_intersect(const vector3f& o, const vector3f& dir, const vector3f& c, float r, float& t)
{
    const float r2 = r * r;
    const vector3f d = dir.unit();
    const vector3f l = c - o;
    const float s = l.dot(d);
    const float lmag = l.magnitude();
    const float l2 = lmag * lmag;
    if (s < 0.0 && l2 < r2) return false;
    const float m2 = l2 - (s * s);
    if (m2 > r2) return false;
    const float q = std::sqrtf(r2 - m2);
    if (l2 > r2) t = s - q;
    else t = s + q;
    return true;
}

struct sphere {
    vector3f centre;
    float radius;
    bool intersect(const vector3f& o, const vector3f& dir, float &t) const
    {
        return ray_intersect(o, dir, centre, radius, t);
    }
};

vector3f cast_ray(const vector3f& orig, const vector3f& dir, const sphere& sphere)
{
    float sphere_dist = std::numeric_limits<float>::max();
    if (!sphere.intersect(orig, dir, sphere_dist))
    {
        return vector3f{ 0.2, 0.7, 0.8 };
    }
    return vector3f{ 0.4, 0.4, 0.3 };
}

void render(const sphere& sphere) {
    const int width = 1024;
    const int height = 768;
    TGAImage framebuffer(width, height, TGAImage::RGB);
    framebuffer.clear();
    
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            framebuffer.set(i, j, TGAColor(255, 0, 255, 255));
        }
    }
    
    const float fov = 120.0;

    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            float x = (2.0*(i + 0.5) / static_cast<float>(width) - 1) * std::tan(fov / 2.0)*width / static_cast<float>(height);
            float y = -(2.0*(j + 0.5) / static_cast<float>(height) - 1) * std::tan(fov / 2.0);
            vector3f dir = vector3f{ x, y, -1 }.unit();
            vector3f colour = cast_ray(vector3f{ 0,0,0 }, dir, sphere);
            unsigned char r = static_cast <unsigned char>(255.0 * colour[0]);
            unsigned char g = static_cast <unsigned char>(255.0 * colour[1]);
            unsigned char b = static_cast <unsigned char>(255.0 * colour[2]);
            framebuffer.set(i, j, TGAColor(r, g, b, 255));
        }
    }
    
    framebuffer.write_tga_file("framebuffer-00.tga");
}

int main(int argc, char** argv)
{
    sphere s0; s0.centre = { -3, 0, -16 }; s0.radius = 2.0;

    render(s0);
    return 0;
}
