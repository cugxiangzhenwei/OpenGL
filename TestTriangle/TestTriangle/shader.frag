#version 330 core

out vec4 color;
uniform vec4 ourColor; // ��OpenGL����������趨�������

void main()
{
//    color = vec4(0.0f, 0.5f, 0.2f, 0.4f);
	  color =  ourColor;
}