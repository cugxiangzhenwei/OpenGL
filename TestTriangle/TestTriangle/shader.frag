#version 330 core

out vec4 color;
in vec4 myColor;

void main()
{
//    color = vec4(0.0f, 0.5f, 0.2f, 0.4f);
	  color = vec4(myColor.x,myColor.y,myColor.z,myColor.w);
}