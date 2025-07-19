#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
  // to flip texture0 vertically
  vec2 yInvTexCoord = vec2(TexCoord.x, -TexCoord.y);
  FragColor = mix(texture(texture0, TexCoord),
                  texture(texture1, yInvTexCoord), 0.2) * vec4(ourColor, 1.0);
}
