#include "element.hpp"

class Sphere : public Element
{
public:
    //constructor destructor
    Sphere() : Element(), m_radius(0){};
    Sphere(float position[3], int colors[3], float radius, float transparency) : Element(position, colors), m_radius(radius)
    {
        m_type = 1;
        m_transparency = transparency;
    };
    ~Sphere(){};

    bool intersect(Ray ray, float &i);
    Ray getNormal(float point[3]);
    void setRadius(float radius);
    void setColors(float color[3]);
    bool getTextureUV(float point[3], float &u, float &v, int color[]);

protected:
    float m_radius;
};