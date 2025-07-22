#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  // here we transfer fragPos, lightPos and normal from local to view space!!!
  fragPos = vec3(view * model * vec4(aPos, 1.0));
  // TODO matrix inverse is costly, would be better to use a uniform here
  // normal matrix transforms normal vector from local space to view space (or world space),
  // see for derivation https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
  mat3 normal_matrix = mat3(transpose(inverse(view * model)));
  normal = normal_matrix * aNormal;
  LightPos = vec3(view * vec4(lightPos, 1.0));
}
