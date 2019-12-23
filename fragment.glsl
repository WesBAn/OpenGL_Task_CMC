#version 330 core

//in vec4 vertexColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
out vec4 color;

//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

void main()
{
    color = vec4(lightColor * objectColor, 1.0f);
//    color = texture(ourTexture, TexCoord);
//      color = texture(ourTexture, TexCoord) * vertexColor;
//    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}