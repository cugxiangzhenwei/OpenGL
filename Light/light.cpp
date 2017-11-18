#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include "CommonFunc.h"
#include "cubic_vertex_data.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
int main(int, char *[])
{
	int width = 800;
	int height = 600;
	GLFWwindow * window = SetUpOpenGLWnd("LearnOpenGL", width, height);
	if (window == NULL)
		return 0;

	GLuint vertexShader = ShaderComplier(GL_VERTEX_SHADER, "basic_lighting.vs");
	GLuint fragmentShader = ShaderComplier(GL_FRAGMENT_SHADER, "basic_lighting.fs");
	GLuint lightvertexShader = ShaderComplier(GL_VERTEX_SHADER, "lamp.vs");
	GLuint lightfragmentShader = ShaderComplier(GL_FRAGMENT_SHADER, "lamp.fs");

	Shader cubicShader("basic_lighting.vs","basic_lighting.fs");
	Shader lampShader("lamp.vs","lamp.fs");

	GLuint VAO,VBO,lightVAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

		cubicShader.use();
		cubicShader.setMat4("model", model);
		cubicShader.setMat4("view", view);
		cubicShader.setMat4("projection", projection);
		cubicShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		cubicShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// 绘制模拟的灯
		lampShader.use();
		lampShader.setVec3("lightPos", lightPos);
	
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
	

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	return 0;
}