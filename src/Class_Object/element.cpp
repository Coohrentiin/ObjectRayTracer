#include <cmath>
#include <iostream>
#include "element.hpp"
// #include "vector_collection.hpp"
using namespace std;

/**************Constructor destructor*********************/
Element::Element() : m_point{0, 0, 0}, m_colors{255, 255, 255} {}
Element::~Element(){}
Element::Element(float x, float y, float z) : m_point{x,y,z}{}
Element::Element(float x, float y, float z, int colors[3]) : m_point{x, y, z}, m_colors{colors[0], colors[1], colors[2]} {}
Element::Element(int colors[3]) : m_colors{colors[0], colors[1], colors[2]} {}
Element::Element(float point[3]) : m_point{point[0], point[1], point[2]}{}
Element::Element(float point[3], int colors[3]) : m_point{point[0], point[1], point[2]}, m_colors{colors[0], colors[1], colors[2]} {}
Element::Element(float point[3], int colors[3], float transparency) : m_point{point[0], point[1], point[2]}, m_colors{colors[0], colors[1], colors[2]}, m_transparency(transparency) {}

Light::Light() : Element(), m_intensity(0){m_type = 0;}
Light::~Light() {}
Light::Light(float x, float y, float z) : Element(x, y, z), m_intensity(100) { m_type = 0; }
Light::Light(float x, float y, float z, int colors[3]) : Element(x, y, z, colors), m_intensity(100) { m_type = 0; }
Light::Light(float point[3]) : Element(point), m_intensity(100) { m_type = 0; }
Light::Light(float point[3], int colors[3]) : Element(point, colors), m_intensity(100) { m_type = 0; }
Light::Light(float intensity) : Element(), m_intensity(intensity) { m_type = 0; }
Light::Light(int colors[3], float intensity) : Element(colors), m_intensity(intensity) { m_type = 0; }
Light::Light(float x, float y, float z, float intensity) : Element(x, y, z), m_intensity(intensity) { m_type = 0; }
Light::Light(float x, float y, float z, int colors[3], float intensity) : Element(x, y, z, colors), m_intensity(intensity) { m_type = 0; }
Light::Light(float point[3], int colors[3], float intensity) : Element(point, colors), m_intensity(intensity) { m_type = 0; }
/*****************  Ascessor Mutator  *********************/
void Element::setX(float x){
    m_point[0] = x;
}

void Element::setY(float y){
    m_point[1] = y;
}

void Element::setZ(float z){
    m_point[2] = z;
}

void Element::setColors(int colors[]){
    for (int i = 0; i < 3; i++)
    {
        m_colors[i] = (int) colors[i];
    }
}

void Element::setCoordonate(float point[]){
    for (int i = 0; i < 3; i++)
    {
        m_point[i] = point[i]; 
    }
}

void Element::setAll(float point[3], int colors[3]){
    for (int i = 0; i < 3; i++)
    {
        m_point[i] = point[i];
    }
    for (int i = 0; i < 3; i++)
    {
        m_colors[i] = colors[i];
    }
}

void Element::setTransparency(float t){
    m_transparency=t;
}
void Element::setReflexion(float reflex){
    m_reflexion=reflex;
}
void Element::setRefraction(float refrac){
    m_refraction=refrac;
}

float* Element::getPoint(){
    return(m_point);
}

int* Element::getColors(){
    return(m_colors);
}

int Element::getType()
{
    return (m_type);
}

float Element::getTransparency()
{
    return (m_transparency);
}

Texture Element::getTexture(){
    return (m_texture);
}

// return the corresponding color pixel corresponding to the image jpg 
bool Element::getTextureUV(float[], float &, float &, int [])
{
    return false;
}

int Element::getHasTexture()
{
    return (m_hasTexture);
}

// function that checks if a ray intersect with its element
// i is the multiplier corresponding to the intersection point
bool Element::intersect(Ray ray, float &i)
{
    std::cout << "hello_parent" << std::endl;
    i = 0;
    ray.printRay();
    return false;
}

// return normal of a given point on the object
Ray Element::getNormal(float point[3]){
    point[1] = point[1];
    Ray ray = Ray();
    return ray;
}