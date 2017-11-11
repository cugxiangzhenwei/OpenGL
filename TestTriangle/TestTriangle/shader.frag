#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

//uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform vec2  imgf;

void main()
{
   // color = texture(ourTexture, TexCoord);
	//color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
	//color = vec4(ourColor,1.0);
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord),imgf.x);
}