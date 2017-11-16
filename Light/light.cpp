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

	glUseProgram(shaderProgram);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
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

		SetShaderMatrix(shaderProgram, "model", model);
		SetShaderMatrix(shaderProgram, "view", view);
		SetShaderMatrix(shaderProgram, "projection", projection);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	return 0;
}