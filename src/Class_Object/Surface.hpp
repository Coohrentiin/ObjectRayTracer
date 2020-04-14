#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include "element.hpp"

class Surface : public Element
{
public:
    //constructor destructor
    Surface() : Element(), m_direction{0, 0, 1} {}; // put size to -1 for infinity
    Surface(float position[3], int colors[3], float direction[3], float transparency) : Element(position, colors)
    {
        m_type = 1;
        m_transparency = transparency;
        m_direction[0] = direction[0];
        m_direction[1] = direction[1];
        m_direction[2] = direction[2];
        m_hasTexture = 0;
    };

    Surface(float position[3], unsigned char *tex, float direction[3], float transparency, int nx, int ny) : Element(position)
    {
        m_type = 1;
        m_transparency = transparency;
        m_direction[0] = direction[0];
        m_direction[1] = direction[1];
        m_direction[2] = direction[2];
        m_colors[0] = 0;
        m_colors[1] = 0;
        m_colors[2] = 0;
        m_texture = Texture(tex, nx, ny);
        m_hasTexture = 1;

        float a;
        a = m_direction[0] * m_point[0];
        a = a + m_direction[1] * m_point[1];
        a = a + m_direction[2] * m_point[2];
        float new_point[3];
        if (m_direction[0] != 0)
        {
            a = a / m_direction[0];
            new_point[0] = a - m_direction[1] / m_direction[0] - m_direction[2] / m_direction[0];
            new_point[1] = 1;
            new_point[2] = 1;
        }
        else if (m_direction[1] != 0)
        {
            a = a / m_direction[1];
            new_point[1] = a - m_direction[0] / m_direction[1] - m_direction[2] / m_direction[1];
            new_point[0] = 1;
            new_point[2] = 1;
        }
        else if (m_direction[2] != 0)
        {
            a = a / m_direction[2];
            new_point[2] = a - m_direction[0] / m_direction[2] - m_direction[1] / m_direction[2];
            new_point[0] = 1;
            new_point[1] = 1;
        }
        else
        {
            std::cout << "Normal Vector of plane shouldn't be (0, 0, 0)'" << std::endl;
        }
        m_first_dimension = Ray(m_point, new_point);
        Ray normal = this->getNormal(m_point);
        m_second_dimension = m_first_dimension.crossProduct(normal);
    };
    ~Surface(){};

    bool intersect(Ray ray, float &i);
    Ray getNormal(float point[3]);
    void setDirection(float direction[3]);
    void setColors(float color[3]);
    bool getTextureUV(float point[], float &u, float &v, int color[]);

protected:
    float m_direction[3];
    Ray m_first_dimension;
    Ray m_second_dimension;
};


#endif