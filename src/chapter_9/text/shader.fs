#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
  FragColor = mix(texture(texture0, TexCoord),
                  texture(texture1, TexCoord), 0.2);
}
