#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
  vec2 zoomMin = vec2(0.49, 0.49);
  vec2 zoomMax = vec2(0.51, 0.51);
  vec2 zoomedTexCoord = zoomMin + (zoomMax - zoomMin) * TexCoord;
  FragColor = mix(texture(texture0, zoomedTexCoord),
                  texture(texture1, zoomedTexCoord), 0.2); // * vec4(ourColor, 1.0);
}
