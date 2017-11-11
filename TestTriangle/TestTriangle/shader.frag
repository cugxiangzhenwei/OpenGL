#version 330 core

out vec4 color;
uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
//    color = vec4(0.0f, 0.5f, 0.2f, 0.4f);
	  color =  ourColor;
}