#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
//out vec3 ourColor;

out vec4 pos;

vec2 bTexCoord;
vec4 globalPos;

uniform sampler2D texture1;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

float MapInRange(float x, float in_min, float in_max, float out_min, float out_max){
    if(x < in_min) x = in_min;
    if(x > in_max) x = in_max;
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void main(){
	bTexCoord = vec2(1 - MapInRange(aPos.x, -14.8, 14.8, 0, 1),MapInRange(aPos.z, -14.8, 14.8, 0, 1));
	//gl_Position = vec4(aPos.x, aPos.z, 1.0, 1.0);
	globalPos = model * vec4(aPos.x, (texture(texture1,bTexCoord) *5.25).r , aPos.z, 1.0);
	gl_Position = projection * view * model * globalPos;
	TexCoord = vec2( bTexCoord.x, bTexCoord.y);
	pos = globalPos;
	
}