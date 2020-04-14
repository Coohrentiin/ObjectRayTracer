#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>
#include "Ray.hpp"
#include "Texture.hpp"
using namespace std;

class Element
{
public:
  // Constructeurs
  Element();
  Element(float x, float y, float z);
  Element(float x, float y, float z, int colors[3]);
  Element(int colors[3]);
  Element(float point[3]);
  Element(float point[3], int colors[3]);
  Element(float point[3], int colors[3], float transparency);
  ~Element();

  //ascessor and mutator
  void setX(float x);
  void setY(float y);
  void setZ(float z);
  void setColors(int colors[3]);
  void setCoordonate(float point[3]);
  void setAll(float point[3], int colors[3]);

  void setTransparency(float t);
  void setReflexion(float reflex);
  void setRefraction(float refrac);

  virtual Ray getNormal(float point[3]);
  float* getPoint();
  int *getColors();
  int getType();
  float getTransparency();
  Texture getTexture();
  int getHasTexture();
  virtual bool intersect(Ray ray, float &i);
  virtual bool getTextureUV(float[], float &, float &, int []);

protected:
  float m_point[3];
  int m_colors[3];
  int m_type;
  float m_transparency=0;
  float m_reflexion=0;
  float m_refraction=0;
  int m_hasTexture = 0;
  Texture m_texture;
};


class Light:public Element
{
public:
  //constructor destructor
  Light();
  Light(float x, float y, float z);
  Light(float x, float y, float z, int colors[3]);
  Light(float point[3]);
  Light(float point[3], int colors[3]);
  Light(float intensity);
  Light(int colors[3], float intensity);
  Light(float x, float y, float z, float intensity);
  Light(float x, float y, float z, int colors[3], float intensity);
  Light(float point[3], int colors[3], float intensity);
  ~Light();

  // fonctions
  // bool intersect(Element* Eye, float ray[3]);

protected:
  float m_intensity;
};
#endif