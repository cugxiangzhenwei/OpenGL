#pragma once
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm\gtc\type_ptr.hpp"
GLchar * GetShaderString(const char * pszFile);
GLuint ShaderComplier(GLenum ShaderType, const char * pszShaderFile);
void SetShaderMatrix(GLuint shaderProgram, GLchar * matrixName, const glm::mat4 & mat);
void SetShaderVec3(GLuint shaderProgram, GLchar * vec3Name, GLfloat v0, GLfloat v1, GLfloat v2);
void SetShaderVec3(GLuint shaderProgram, GLchar * vec3Name, const glm::vec3 &value);
GLFWwindow* SetUpOpenGLWnd(const char * pszWinTitle, int iWidth, int iHeight);
GLuint loadTexture(char const * path);