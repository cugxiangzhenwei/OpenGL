#include "CommonFunc.h"
#include <fstream>
#include <iostream>
GLchar * GetShaderString(const char * pszFile)
{
	std::ifstream in(pszFile);
	if (!in.is_open())
	{
		std::cout << "Failed to Load Shader file" << pszFile;
		return NULL;
	}
	std::istreambuf_iterator<char> beg(in), end;
	std::string str(beg, end);
	int ilen = str.length() + 1;
	GLchar * pStr = (GLchar*)(malloc(ilen * sizeof(GLchar)));
	strcpy_s(pStr, ilen * sizeof(GLchar), str.c_str());
	in.close();
	return pStr;
}
GLuint ShaderComplier(GLenum ShaderType, const char * pszShaderFile)
{
	GLuint Shader;
	Shader = glCreateShader(ShaderType);
	GLchar * pStrContent = GetShaderString(pszShaderFile);
	if (pStrContent == NULL)
		return -1;

	glShaderSource(Shader, 1, &pStrContent, NULL);
	glCompileShader(Shader);
	free(pStrContent);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return Shader;
}
void SetShaderMatrix(GLuint shaderProgram, GLchar * matrixName, const glm::mat4 & mat)
{
	GLint lFormLoc = glGetUniformLocation(shaderProgram, matrixName);
	glUniformMatrix4fv(lFormLoc, 1, GL_FALSE, glm::value_ptr(mat));
}
void SetShaderVec3(GLuint shaderProgram, GLchar * vec3Name,GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint lFormLoc = glGetUniformLocation(shaderProgram, vec3Name);
	glUniform3f(lFormLoc,v0,v1,v2);
}
void SetShaderVec3(GLuint shaderProgram, GLchar * vec3Name, const glm::vec3 &value)
{
	GLint lFormLoc = glGetUniformLocation(shaderProgram, vec3Name);
	glUniform3fv(lFormLoc, 1, &value[0]);
}


GLFWwindow* SetUpOpenGLWnd(const char * pszWinTitle, int iWidth, int iHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, pszWinTitle, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
	return window;
}