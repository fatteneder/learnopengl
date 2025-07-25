#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
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

  FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}
