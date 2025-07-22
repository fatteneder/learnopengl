#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
  vec3 diffuseColor = vec3(texture(material.diffuse, texCoord));

  vec3 ambient = light.ambient * diffuseColor;

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseColor;

  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm); // - because lightdir Points from fragment to light source
  int shininess = 32;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specularColor = vec3(texture(material.specular, texCoord));
  vec3 specular = light.specular * spec * specularColor;

  vec3 emission = vec3(texture(material.emission, texCoord));

  FragColor = vec4(max(emission, ambient + diffuse + specular), 1.0);
}
