#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//draws a square
class Square
{

private:
	const char* vertexShaderSource = nullptr;
	const char* fragmentShaderSource = nullptr;


	void checkShaderCompilation(GLuint* shader);
	void processInput(GLFWwindow* window);

public: 
	Square();

	void drawShape(GLFWwindow* window);
};

