#version 330 core

layout (location = 0) in vec3 position;
uniform vec3 posOffset;

void main()
{
    //gl_Position = vec4(position.x, position.y, position.z, 10.0);
	gl_Position = vec4(position.x + posOffset.x , position.y + posOffset.y , position.z + posOffset.z , 10.0);
}