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
static float g_fAnle = 25.0;
#define  M_PI  3.1415926

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		g_fAnle += 5.0; //一次调整5度
		if (g_fAnle > 360.0)
			g_fAnle = 360.0;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		g_fAnle -= 5.0; //一次调整5度
		if (g_fAnle < -360.0)
			g_fAnle = -360.0;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		g_fV += 0.1;
		if (g_fV > 1.0)
		{
			g_fV = 1.0;
		}
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		g_fV -= 0.1;
		if (g_fV < 0.0)
			g_fV = 0.0;
	}
}
void mouse_event_callback(GLFWwindow* window, int button, int action, int modifierKey)
{
	if (button == 0)
	{
		int a = 0;
	}
}
GLfloat vertices[] =
 {
	// Front face				
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	// Right face
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,

	// Back face
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	// Left face
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,

	// Top face
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,

	// Bottom face
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,    
};

GLfloat texture_coord[] =
{
	// Front face	
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,
	1.0f,1.0f,

	// Right face
	1.0f,1.0f,
	0.0f,1.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,

	// Back face
	1.0f,1.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	1.0f,0.0f,
	0.0f,0.0f,
	0.0f,1.0f,

	// Left face
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	0.0f,1.0f,
	1.0f,1.0f,


	// Top face
	0.0f,0.0f,
	0.0f,1.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	1.0f,1.0f,
	1.0f,0.0f,

	// Bottom face
	0.0f,1.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	0.0f,1.0f,
	0.0f,0.0f
};
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
	// 随着时间放到缩小
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
	glBindVertexArray(0); //4. 解绑VAO
}
void SetShaderMat(GLuint shaderProgram,const GLchar * matName,const glm::mat4 & mat)
{
	GLint matlFormLoc = glGetUniformLocation(shaderProgram, matName);
	glUniformMatrix4fv(matlFormLoc, 1, GL_FALSE, glm::value_ptr(mat));
}
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
void processInput(GLFWwindow *window)
{
	float cameraSpeed = 3.5f * deltaTime; // 移动速度乘以上次绘制所需的时间，使得移动速度均匀
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
	glfwSetMouseButtonCallback(window, mouse_event_callback);


	// 0. 复制顶点数组到缓冲中供OpenGL使用
	GLuint VBO[5];
	glGenBuffers(5, VBO);
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);*/



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
	// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
	// 1. 绑定VAO
	glBindVertexArray(VAO[0]);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	// 加载并生成纹理
	//int width, height;
	unsigned char* image1 = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	// 加载并生成纹理
	//int width, height;
	unsigned char* image2 = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);//规定逆序为正面

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f,0.3f, 0.3f, 1.0f); // 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);//一个纹理的默认纹理单元是0，它是默认的激活纹理单元


		glActiveTexture(GL_TEXTURE1); //在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1); //给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理


		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "imgf");
		glUniform2f(vertexColorLocation, g_fV,1.0);

		////旋转
	/*	glm::mat4 model;
		model = glm::rotate(model, glm::radians(g_fAnle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime()* glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
		/*glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));*/

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		/*float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/

		//透视投影
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), width*1.0f / height, 0.1f, 100.0f);

		SetShaderMat(shaderProgram, "view",view);
		SetShaderMat(shaderProgram, "projection", projection);

		glBindVertexArray(VAO[0]);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 2 == 0)
				model = glm::rotate(model, (float)glfwGetTime()* glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			SetShaderMat(shaderProgram, "model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0); //4. 解绑VAO
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}