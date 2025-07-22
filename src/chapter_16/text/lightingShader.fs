#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutoff; // angle of a spot light's cone
  float outerCutoff; // angle of spot light's outer dimmer cone

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // attenuation constants
  float kc;
  float kl;
  float kq;
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

  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction)); // - to make direction point from fragment towards source
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

  vec3 ambient = light.ambient * diffuseColor;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseColor;

  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm); // - because lightdir Points from fragment to light source
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specularColor = vec3(texture(material.specular, texCoord));
  vec3 specular = light.specular * spec * specularColor;

  float d = length(light.position - fragPos);
  float attenuation = 1.0/(light.kc + light.kl * d + light.kq * d*d);

  vec3 color = ambient + intensity * attenuation * (diffuse + specular);
  FragColor = vec4(color, 1.0);
}
