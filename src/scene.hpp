#ifndef SCENE_H
#define SCENE_H

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "Class_Object/element.hpp"
#include "Class_Object/Sphere.hpp"
#include "Class_Object/Surface.hpp"
#include <memory>
class Scene
{
protected:
    int bg_color[3];
    float m_eye_position[3];

    vector<Element> collection;
    vector<Light> lightCollection;

public:
    vector<Element*> m_elements;
    int m_width;
    int m_height;
    float m_res;

    Scene();
    Scene(int width, int height);
    Scene(int width, int height, float eye_position, float res);
    void render_scene();
    void add_element(Element *newElement);
    void add_light(Light newLight);
    void send_Ray(Ray ray, int nb, int result[3]);

    void set_dimension(int width, int height);
    
};
#endif