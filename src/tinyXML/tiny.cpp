#include "tinyxml2/tinyxml2.h"
#include "../scene.hpp"
#include <stdio.h>
#include <iostream>

using namespace tinyxml2;

int dump_attribs_to_stdout(XMLElement *pElement, float args[])
{
	if (!pElement)
		return 0;

	const XMLAttribute *pAttrib = pElement->FirstAttribute();
	int i = 0;
	float ival;
	double dval;
	while (pAttrib)
	{
		// printf("%s%s: value=[%s]", pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryFloatValue(&ival) == XML_SUCCESS){
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
	XMLDocument doc;
	doc.LoadFile("ex1.xml");
	XMLHandle hDoc(&doc);
	XMLElement* child;
	XMLHandle hRoot(0), hObject(0);

	child=hDoc.FirstChildElement().ToElement();
	hRoot=XMLHandle(child);

	XMLElement* hElem=hRoot.FirstChildElement().ToElement();
	std::string title(hElem->Value());
	while (hElem){
		std::string title(hElem->Value());
		hObject=XMLHandle(hElem);
		if (title.compare("scene") == 0){
			float args[2];
			int a = dump_attribs_to_stdout(hElem, args);
			Scene scene = Scene(args[0], args[1]);
			scene.render_scene();
		}
			// std::string word=hObject.ToElement()->GetText();
			// std::cout<<word<<std::endl;
			hElem = hElem->NextSiblingElement();
	}
	std::cout<<"ok"<<std::endl;
	return 0;
}

