#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 lightingColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
  vec3 fragPos = vec3(model * vec4(aPos, 1.0));
  gl_Position = projection * view * vec4(fragPos, 1.0);

  // Phong shading in vertex shader =^= Gouraud shading

  // TODO matrix inverse is costly, would be better to use a uniform here
  mat3 normal_matrix = mat3(transpose(inverse(model))); // transform normal vectors from local to world space
  vec3 normal = normal_matrix * aNormal;

  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm); // - because lightdir Points from fragment to light source
  int shininess = 32;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * lightColor;

  lightingColor = ambient + diffuse + specular;
}
