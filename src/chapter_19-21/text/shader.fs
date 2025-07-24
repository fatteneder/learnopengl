#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1; // texture_diffuseNr is naming convetion from mesh.hpp

void main()
{
  fragColor = texture(texture_diffuse1, texCoord);
}
