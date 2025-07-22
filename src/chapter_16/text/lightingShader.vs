#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  fragPos = vec3(model * vec4(aPos, 1.0));
  gl_Position = projection * view * vec4(fragPos, 1.0);
  // TODO matrix inverse is costly, would be better to use a uniform here
  mat3 normal_matrix = mat3(transpose(inverse(model))); // transform normal vectors from local to world space
  normal = normal_matrix * aNormal;
  texCoord = aTexCoord;
}
