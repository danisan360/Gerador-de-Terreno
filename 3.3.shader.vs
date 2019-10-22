#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ourColor;

uniform sampler2D texture1;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
	//gl_Position = vec4(aPos.x, aPos.z, 1.0, 1.0);
	gl_Position = projection * view * model * vec4(aPos.x, (texture(texture1,aTexCoord).r *2.25) , aPos.z, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	ourColor = aColor;
}