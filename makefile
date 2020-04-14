CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -fopenmp

all: main

main: src/Class_Object/element.cpp 
	g++ $(CXX_FLAGS) -o objectRay.exe src/Class_Object/element.cpp src/main.cpp src/scene.cpp src/Class_Object/Sphere.cpp src/Class_Object/Surface.cpp src/tinyxml2/tinyxml2.cpp
clean:
	@rm -fr *.o *.exe output/*.ppm *~

help:
	@echo "Do not forget to make a make clean regularly"