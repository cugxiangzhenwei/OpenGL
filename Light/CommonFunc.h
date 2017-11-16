#pragma once
#define GLEW_STATIC
#include "GL\glew.h"
GLchar * GetShaderString(const char * pszFile);
GLuint ShaderComplier(GLenum ShaderType, const char * pszShaderFile);