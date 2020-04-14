CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

all: main

main: Class_Object/element.cpp 
	g++ $(CXX_FLAGS) -o objectRay.exe Class_Object/element.cpp main.cpp scene.cpp Class_Object/Sphere.cpp Class_Object/Surface.cpp ./xml_first_test/tinyxml2/tinyxml2.cpp
	g++ $(CXX_FLAGS) -o tiny.out tiny.cpp ./xml_first_test/tinyxml2/tinyxml2.cpp    
clean:
	@rm -fr *.o *.exe *.ppm *~

help:
	@echo "		nothing, we should add things here ^^ !"
