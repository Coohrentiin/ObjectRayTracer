#include <cmath>
#include <iostream>
#include "Sphere.hpp"
using namespace std;


// methods
bool Sphere::intersect(Ray ray, float &i)
{
    Ray v = Ray(m_point, ray.m_origin);
    const float b = 2 * ray.dotIntersection(v);
    const float c = v.dot(v) - m_radius * m_radius;
    float delta = b * b - 4 * c;
    if (delta < 1e-4)
        return false;

    const float t1 = (-b - sqrt(delta)) / 2;
    const float t2 = (-b + sqrt(delta)) / 2;
    if (t1 <= 0.1 && t2 <= 0.1){
        return false;
    }
    i = (t1 < t2) ? t1 : t2; // get first closest intersection with the sphere
    i = (t1 <= 0.1) ? t2 : t1; // check the element is not behind
    return true;
}

Ray Sphere::getNormal(float point[3]){
    Ray to_return = Ray(m_point, point);
    return to_return;
}

void Sphere::setRadius(float radius){
    m_radius=radius;
}

void Sphere::setColors(float color[3]){
    for(int i=0;i<3;i++){
        m_colors[i]=(int) color[i];
    }
}

bool Sphere::getTextureUV(float point[3], float &u, float &v, int color[])
{
    if (m_hasTexture) // not implemented yet...
    {
        color[0] = m_colors[0];
        point[0] = 0;
        u = 0;
        v = 0;
        return true;
    }
    color = m_colors;
    return false;
}