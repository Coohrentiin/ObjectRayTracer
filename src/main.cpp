#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "scene.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Class_Object/Texture.hpp"
#include <chrono>

#include "tinyxml2/tinyxml2.h"
#include <stdio.h>

using namespace tinyxml2;

// use tinyxml to get all attribute of the element in the scene
int dump_attribs_to_stdout(XMLElement *pElement, float args[])
{
	if (!pElement)
		return 0;

	const XMLAttribute *pAttrib = pElement->FirstAttribute();
	int i = 0;
	float ival;
	while (pAttrib)
	{

		if (pAttrib->QueryFloatValue(&ival) == XML_SUCCESS)
		{
			args[i] = ival;
		}
		i++;
		pAttrib = pAttrib->Next(); // get next attribute
	}
	return i;
}
//special to load textures
int dump_attribs_to_stdout_texture(XMLElement *pElement, float args[])
{
	if (!pElement)
		return 0;

	const XMLAttribute *pAttrib = pElement->FirstAttribute();
	int i = 0;
	int ival;
	const char* name = pElement->Attribute("path");
	std::string str = name;
	std::string to_copy = "cp " + str + " input/temp.jpg";
	// std::cout << to_copy << std::endl;
	const char *u = to_copy.c_str();
	std::system(u);

	// std::cout << name << std::endl;
	while (pAttrib)
	{
		// printf("%s", pAttrib->Name());
		if (pAttrib->QueryIntValue(&ival) == XML_SUCCESS)
		{
			// printf(" %s=%f", pAttrib->Name(), ival);
			args[i] = ival;
		}
		i++;
		pAttrib = pAttrib->Next();
	}
	return i;
}

int main(int argc, char const *argv[])
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	XMLDocument doc;
	// get xml config file
	if (argc == 1){
	doc.LoadFile("input/default.xml");
	}
	else{
		doc.LoadFile(argv[1]);
	}

	// init xml file
	XMLHandle hDoc(&doc);
	XMLElement *child;
	XMLHandle hRoot(0), hObject(0);

	child = hDoc.FirstChildElement().ToElement();
	hRoot = XMLHandle(child);

	XMLElement *hElem = hRoot.FirstChildElement().ToElement();
	std::string title(hElem->Value());

	if (title.compare("scene") == 0)
	{
	}
	else
	{
		// should be problematic
		std::cout << "please define scene first" << std::endl;
	}

	// get scene argument
	float args[4];
	dump_attribs_to_stdout(hElem, args);
	Scene scene = Scene(args[0], args[1], args[2], args[3]);
	vector<Sphere *> spheres;
	vector<Surface *> surfaces;
	vector<unsigned char *> textures;
	Light light;
	while (hElem)
	{
		std::string title(hElem->Value());
		hObject = XMLHandle(hElem);
		if (title.compare("sphere") == 0)
		{
			float args[8];
			dump_attribs_to_stdout(hElem, args);
			float position[3] = {args[0], args[1], args[2]};
			int color[3] = {int(args[3]), int(args[4]), int(args[5])};
			spheres.push_back(new Sphere(position, color, args[6], args[7]));
		}
		if (title.compare("light") == 0)
		{
			float args[7];
			dump_attribs_to_stdout(hElem, args);
			float position[3] = {args[0], args[1], args[2]};
			int color[3] = {int(args[3]), int(args[4]), int(args[5])};
			light = Light(position, color, args[6]);
			scene.add_light(light);
		}
		if (title.compare("surface") == 0)
		{
			float args[14];
			dump_attribs_to_stdout_texture(hElem, args);
			float position[3] = {args[0], args[1], args[2]};
			int color[3] = {int(args[3]), int(args[4]), int(args[5])};
			float direction[3] = {args[6], args[7], args[8]};
			if (args[10] != 0)
			{
				int nx1 = (int)(args[11]), ny1 = (int)(args[12]);
				int nn = (int)(args[13]);
				
				// std::ofstream("input/copy2.jpg", std::ios::binary) << std::ifstream(filename, std::ios::binary).rdbuf();
				unsigned char *tex2 = stbi_load("input/temp.jpg", &nx1, &ny1, &nn, 0);
				surfaces.push_back(new Surface(position, tex2, direction, 0, args[11], args[12]));
			}
			else
			{

				surfaces.push_back(new Surface(position, color, direction, args[9]));
			}
		}
		hElem = hElem->NextSiblingElement();
	}

	// add sphere
	vector<Sphere *>::iterator i;
	for (i = spheres.begin(); i != spheres.end(); ++i)
	{
		scene.add_element(*i);
	}

	// add surface
	vector<Surface *>::iterator k;
	for (k = surfaces.begin(); k != surfaces.end(); ++k)
	{
		scene.add_element(*k);
	}
	scene.render_scene();

	    end = std::chrono::system_clock::now();
	    std::chrono::duration<double> elapsed_seconds = end - start;
	    std::cout << "Temps Ray Tracing : " << elapsed_seconds.count()
	              << std::endl;

	return 0;
}