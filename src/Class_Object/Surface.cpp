#include <cmath>
#include <math.h>
#include <iostream>
#include "Surface.hpp"

// methods

bool Surface::intersect(Ray ray, float &i)
{
    float dotprod2 = ray.m_normalized[0] * m_direction[0] + ray.m_normalized[1] * m_direction[1] + ray.m_normalized[2] * m_direction[2];
    if (dotprod2 < 0)
    {
        float temp_ray[3];
        temp_ray[0] = m_point[0] - ray.m_origin[0];
        temp_ray[1] = m_point[1] - ray.m_origin[1];
        temp_ray[2] = m_point[2] - ray.m_origin[2];
        float dotprod1 = temp_ray[0] * m_direction[0] + temp_ray[1] * m_direction[1] + temp_ray[2] * m_direction[2];
        i = -dotprod1 / dotprod2;
        if (i < 0)
        {
            i = -i;
        }
        return true;
    }
    else
    {
        return false;
    }
    i = 1000;
}

void Surface::setDirection(float direction[3]){
    for(int i=0;i<3;i++){    
        m_direction[i]=direction[i];
    }
}

Ray Surface::getNormal(float point[3])
{
    float new_point[3];
    new_point[0] = point[0] + m_direction[0];
    new_point[1] = point[1] + m_direction[1];
    new_point[2] = point[2] + m_direction[2];
    Ray to_return = Ray(point, new_point);
    return to_return;
}

void Surface::setColors(float color[3]){
    for(int i=0;i<3;i++){
        m_colors[i]=(int) color[i];
    }
}

bool Surface::getTextureUV(float point[3], float &u, float &v, int color[])
{
    if (m_hasTexture) // double check element has a texture
    {

        Ray new_ray = Ray(m_point, point);
        u = m_first_dimension.dotIntersection(new_ray); // get the contribution of first dimension
        v = m_second_dimension.dotIntersection(new_ray); // get the contribution of first dimension
        int n = 300; // n is the size of the image in the virtual world. Bigger n gives bigger image (and less)
        u = fmod(u, n); 
        v = fmod(v, n);
        u = u / n;
        v = v / n;
        if (u < 0) // if u or v is negative, get the correspondant point on image
        {
            u = 1 + u;
        }
        if (v < 0)
        {
            v = 1 + v;
        }
        m_texture.getValueUV(u, v, color);
        return true;
    }
    color = m_colors;
    return false;
}