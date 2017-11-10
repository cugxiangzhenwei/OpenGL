// GLEW
#if 0
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <strstream>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
//顶点位置数组  
float positionData[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f };
//颜色数组  
float colorData[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f };

GLchar * GetShaderString(const char * pszFile)
{
	std::ifstream in(pszFile);
	if (!in.is_open())
	{
		std::cout << "Failed to Load Shader file"<< pszFile;
		return NULL;
	}
	std::istreambuf_iterator<char> beg(in), end;
	std::string str(beg, end);
	int ilen = str.length()+1;
	GLchar * pStr = (GLchar*)(malloc(ilen*sizeof(GLchar)));
	strcpy_s(pStr, ilen * sizeof(GLchar),str.c_str());
	in.close();
	return pStr;
}
GLuint ShaderComplier(GLenum ShaderType,const char * pszShaderFile)
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
void OutPutVersion()
{
	//1、查看GLSL和OpenGL的版本  
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "GL Vendor    :" << vendor << std::endl;
	std::cout << "GL Renderer  : " << renderer << std::endl;
	std::cout << "GL Version (string)  : " << version << std::endl;
	std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;
	std::cout << "GLSL Version : " << glslVersion << std::endl;
}
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	OutPutVersion();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);

	GLuint vertexShader = ShaderComplier(GL_VERTEX_SHADER,"basic.vert");
	GLuint fragmentShader = ShaderComplier(GL_FRAGMENT_SHADER,"basic.frag");

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(shaderProgram, 0, "VertexPosition");
	glBindAttribLocation(shaderProgram, 1, "VertexColor");

	GLint success = 0;
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK shaderProgam\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create and populate the buffer objects  
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	GLuint vaoHandle;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//调用glVertexAttribPointer之前需要进行绑定操作  
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle); //vboHandles[0]
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);  //加载数据到VBO  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);  //vboHandles[1]
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glEnableVertexAttribArray(0);//顶点坐标  
	glEnableVertexAttribArray(1);//顶点颜色  

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT); //状态应用函数
		glUseProgram(shaderProgram);
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); //4. 解绑VAO

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
#endif