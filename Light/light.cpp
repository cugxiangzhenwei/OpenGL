#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "CommonFunc.h"
#include "cubic_vertex_data.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
int main(int, char *[])
{
	int width = 800;
	int height = 600;
	GLFWwindow * window = SetUpOpenGLWnd("LearnOpenGL", width, height);
	if (window == NULL)
		return 0;

	GLuint vertexShader = ShaderComplier(GL_VERTEX_SHADER, "shader.vs");
	GLuint fragmentShader = ShaderComplier(GL_FRAGMENT_SHADER, "shader.frag");
	GLuint lightfragmentShader = ShaderComplier(GL_FRAGMENT_SHADER, "light.frag");

	GLuint cubicShaderProgram;
	cubicShaderProgram = glCreateProgram();

	GLuint lightShaderProgram;
	lightShaderProgram = glCreateProgram();

	glAttachShader(cubicShaderProgram, vertexShader);
	glAttachShader(cubicShaderProgram, fragmentShader);

	glAttachShader(lightShaderProgram, vertexShader);
	glAttachShader(lightShaderProgram, lightfragmentShader);

	GLint success = 0;
	glLinkProgram(cubicShaderProgram);
	glGetProgramiv(cubicShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(cubicShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK cubicShaderProgram\n" << infoLog << std::endl;
	}
	glLinkProgram(lightShaderProgram);
	glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(lightShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::LINK lightShaderProgram\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(lightfragmentShader);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cuibc_vertices), cuibc_vertices, GL_STATIC_DRAW);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec3 lightPos(0.8f, 0.65f, 0.8f);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// 绘制旋转的立方体
		//旋转
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime()* glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//透视投影
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), width*1.0f / height, 0.1f, 100.0f);

		glUseProgram(cubicShaderProgram);
		SetShaderMatrix(cubicShaderProgram, "model", model);
		SetShaderMatrix(cubicShaderProgram, "view", view);
		SetShaderMatrix(cubicShaderProgram, "projection", projection);
		SetShaderVec3(cubicShaderProgram, "objectColor", 1.0f, 0.5f, 0.31f);
		SetShaderVec3(cubicShaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// 绘制模拟的灯
		glUseProgram(lightShaderProgram);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		SetShaderMatrix(lightShaderProgram, "model", model);
		SetShaderMatrix(lightShaderProgram, "view", view);
		SetShaderMatrix(lightShaderProgram, "projection", projection);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	return 0;
}