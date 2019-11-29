#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pos;

uniform sampler2D texture2;

void main(){

   FragColor = texture(texture2, TexCoord);
}