// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static float g_fV = 0.5;
#define  M_PI  3.1415926

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
	// �ر�Ӧ�ó���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		g_fV += 0.1;
		if (g_fV > 1.0)
			g_fV = 1.0;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		g_fV -= 0.1;
		if (g_fV < 0.0)
			g_fV = 0.0;
	}
}
GLfloat vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
};

GLuint indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
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
typedef enum 
{
	POS_LEFT_TOP,
	POS_RIGHT_TOP,
	POS_RIGHT_BOTTOM,
	POS_LEFT_BOTTOM
}ENUM_PIC_POS;

void DrawImage(ENUM_PIC_POS pos, GLuint shaderProgram,GLuint vao)
{
	// ����ʱ��ŵ���С
	glm::mat4 trans;
	float fx = 0.0;
	float fy = 0.0;
	float fScale = sin((GLfloat)glfwGetTime());
	if (pos == POS_LEFT_TOP)
	{
		fx = -0.5f;
		fy = 0.5f;
	}
	else if (pos == POS_RIGHT_TOP)
	{
		fx = 0.5f;
		fy = 0.5f;
	}
	else if (pos == POS_RIGHT_BOTTOM)
	{
		fx = 0.5f;
		fy = -0.5f;
	}
	else if (pos == POS_LEFT_BOTTOM)
	{
		fx = -0.5f;
		fy = -0.5f;
	}
	trans = glm::translate(trans, glm::vec3(fx, fy, 0.0f));
	trans = glm::scale(trans, glm::vec3(fScale, fScale, fScale));

	GLint transFormLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transFormLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); //4. ���VAO
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


	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	GLuint vertexShader = ShaderComplier(GL_VERTEX_SHADER, "shader.vs");
	GLuint fragmentShader = ShaderComplier(GL_FRAGMENT_SHADER, "shader.frag");

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

	glUseProgram(shaderProgram);

#define vao_count (5)

	GLuint VAO[vao_count];
	glGenVertexArrays(5, VAO);
	// ..:: ��ʼ�����루ֻ����һ�� (�����������Ƶ���ı�)�� :: ..
	// 1. ��VAO
	glBindVertexArray(VAO[0]);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO�������ݶ�ȡ
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	// ���ز���������
	//int width, height;
	unsigned char* image1 = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	// ���ز���������
	//int width, height;
	unsigned char* image2 = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);


	// 3. ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	{
		glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		vec = trans * vec;
		std::cout << vec.x << vec.y << vec.z << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f,0.3f, 0.3f, 1.0f); // ״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT); //״̬Ӧ�ú���

		glActiveTexture(GL_TEXTURE0); //�ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);//һ�������Ĭ������Ԫ��0������Ĭ�ϵļ�������Ԫ


		glActiveTexture(GL_TEXTURE1); //�ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1); //���������������һ��λ��ֵ�������Ļ������ܹ���һ��Ƭ����ɫ�������ö������


		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "imgf");
		glUniform2f(vertexColorLocation, g_fV,1.0);

		DrawImage(POS_LEFT_TOP, shaderProgram,VAO[0]);
		DrawImage(POS_RIGHT_TOP, shaderProgram, VAO[0]);
		DrawImage(POS_RIGHT_BOTTOM, shaderProgram, VAO[0]);
		DrawImage(POS_LEFT_BOTTOM, shaderProgram, VAO[0]);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}