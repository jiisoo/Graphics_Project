all:
	g++ main.cpp Camera.cpp Object.cpp Shader.cpp -o objloader -I/usr/include/GL -lGL -lGLEW -lglut -std=c++11
