#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  // attenuation constants
  float kc;
  float kl;
  float kq;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutoff; // angle of a spot light's cone
  float outerCutoff; // angle of spot light's outer dimmer cone

  // attenuation constants
  float kc;
  float kl;
  float kq;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define N_POINT_LIGHTS 4

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoord)
{
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 diffuseColor = vec3(texture(material.diffuse, texCoord));
  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diff * diffuseColor;
  vec3 specularColor = vec3(texture(material.specular, texCoord));
  vec3 specular = light.specular * spec * specularColor;

  return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 diffuseColor = vec3(texture(material.diffuse, texCoord));
  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diff * diffuseColor;
  vec3 specularColor = vec3(texture(material.specular, texCoord));
  vec3 specular = light.specular * spec * specularColor;

  float d = length(light.position - fragPos);
  float attenuation = 1.0/(light.kc + light.kl * d + light.kq * d*d);

  return attenuation * (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 diffuseColor = vec3(texture(material.diffuse, texCoord));
  vec3 ambient = light.ambient * diffuseColor;
  vec3 diffuse = light.diffuse * diff * diffuseColor;
  vec3 specularColor = vec3(texture(material.specular, texCoord));
  vec3 specular = light.specular * spec * specularColor;

  float d = length(light.position - fragPos);
  float attenuation = 1.0/(light.kc + light.kl * d + light.kq * d*d);

  float theta = dot(lightDir, normalize(-light.direction)); // - to make direction point from fragment towards source
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

  return ambient + intensity * attenuation * (diffuse + specular);
}

void main()
{
  vec3 norm = normalize(normal);
  vec3 viewDir = normalize(viewPos - fragPos);

  vec3 color = calcDirLight(dirLight, norm, viewDir, texCoord);
  for (int i = 0; i < N_POINT_LIGHTS; i++)
    color += calcPointLight(pointLights[i], norm, fragPos, viewDir, texCoord);
  color += calcSpotLight(spotLight, norm, fragPos, viewDir, texCoord);

  fragColor = vec4(color, 1.0);
}
