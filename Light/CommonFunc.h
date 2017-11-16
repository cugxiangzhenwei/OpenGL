#pragma once
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm\gtc\type_ptr.hpp"
GLchar * GetShaderString(const char * pszFile);
GLuint ShaderComplier(GLenum ShaderType, const char * pszShaderFile);
void SetShaderMatrix(GLuint shaderProgram, GLchar * matrixName, const glm::mat4 & mat);
GLFWwindow* SetUpOpenGLWnd(const char * pszWinTitle, int iWidth, int iHeight);