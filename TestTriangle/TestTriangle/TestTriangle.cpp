// GLEW
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
GLfloat vertices[] = {
	-5.0f, -5.0f, 0.0f,
	5.0f, -5.0f, 0.0f,
	0.0f,  5.0f, 0.0f
};
GLchar * vertexShaderSource = NULL;
GLchar * fragmentShaderSource = NULL;

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
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);


	// 0. 复制顶点数组到缓冲中供OpenGL使用
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	GLuint vertexShader = ShaderComplier(GL_VERTEX_SHADER,"shader.vs");
	GLuint fragmentShader = ShaderComplier(GL_FRAGMENT_SHADER,"shader.frag");

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

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

	#define vao_count (5)

	GLuint VAO[vao_count];
	glGenVertexArrays(5, VAO);
	// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
	// 1. 绑定VAO
	glBindVertexArray(VAO[0]);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f,0.3f, 0.3f, 1.0f); // 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT); //状态应用函数

		glUseProgram(shaderProgram);
		// 更新uniform颜色
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLfloat RedValue = (sin(timeValue) / 5) + 0.75;
		GLfloat BlueVlaue = fabs(cos(timeValue));

		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, RedValue, greenValue, BlueVlaue, 1.0f);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); //4. 解绑VAO
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}