#include <math.h>
#include <algorithm>
#include <vector>
#include <tgaimage.h>
#include <vector2.hpp>
#include <vector3.hpp>

using vector3f = vector3<float>;
using vector2f = vector2<float>;

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

struct material_t {
    vector3f diffuse_colour;
    vector2f albedo;
    float specular_exponent;
    material_t(const vector3f& diffuse_colour, const vector2f& albedo, float speculat_exponent)
        : diffuse_colour(diffuse_colour), albedo(albedo), specular_exponent(specular_exponent)
    {

    }
};

struct light {
    vector3f position;
    float intensity;
    light(const vector3f& position, float intensity) : position(position), intensity(intensity)
    {

    }
};

struct sphere {
    vector3f centre;
    float radius;
    material_t material;
    sphere(const vector3f& centre, float radius, const material_t& material)
        : centre(centre), radius(radius), material(material)
    {

    }
    bool intersect(const vector3f& o, const vector3f& dir, float &t) const
    {
        return ray_intersect(o, dir, centre, radius, t);
    }
};

vector3f cast_ray(const vector3f& orig, const vector3f& dir, std::vector<sphere>& spheres, const light& light)
{
    vector3f d = dir.unit();
    float diffuse_light_intensity = 0.0;
    float sphere_dist = std::numeric_limits<float>::max();
    for (const sphere& sphere : spheres)
    {
        if (sphere.intersect(orig, dir, sphere_dist))
        {
            const vector3f p = orig + (dir * sphere_dist);
            const vector3f diffuse_light_dir = (light.position - p).unit();
            const vector3f normal = (p - sphere.centre).unit();
            const float diffuse_light_intensity = light.intensity * std::max(0.0f, diffuse_light_dir.dot(normal));
            return sphere.material.diffuse_colour * diffuse_light_intensity;
        }
    }
    return vector3f{ 0.2, 0.2, 0.2 };
}

void render(std::vector<sphere>& spheres, const light& light) {
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
            vector3f colour = cast_ray(vector3f{ 0,0,0 }, dir, spheres, light);
            unsigned char r = static_cast <unsigned char>(255.0 * colour[0]);
            unsigned char g = static_cast <unsigned char>(255.0 * colour[1]);
            unsigned char b = static_cast <unsigned char>(255.0 * colour[2]);
            framebuffer.set(i, j, TGAColor(r, g, b, 255));
        }
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
}

int main(int argc, char** argv)
{
    std::vector<sphere> spheres;
    // sort closest to farthest
    spheres.push_back(sphere({ -3, 0, -16 }, 2.0, material_t({ 0.0,0.0,0.8 }, { 0.0, 0.0 }, 2.0)));
    spheres.push_back(sphere({ -2, -5, -20 }, 2.0, material_t({ 0.0,0.8,0.0 }, { 0.0, 0.0 }, 2.0)));
    spheres.push_back(sphere({ 2, 2.5, -24 }, 2.0, material_t({ 1.0,0.0,0.0 }, { 0.0, 0.0 }, 2.0)));
    spheres.push_back(sphere({ 1, 1, -30 }, 2.0, material_t({ 0.0,0.8,0.8 }, { 0.0, 0.0 }, 2.0)));
    
    light l0({3,10,0}, 0.8);

    render(spheres, l0);
    return 0;
}
