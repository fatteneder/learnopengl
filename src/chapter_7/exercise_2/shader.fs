#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
  FragColor = mix(texture(texture0, TexCoord/2.0),
                  texture(texture1, TexCoord), 0.2) * vec4(ourColor, 1.0);
}
