#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords; 

struct Material {
    float ka;
    vec3 ambient;

    float kd;
    vec3 diffuse;
    sampler2D diffuseMap;
    bool useDiffuseMap;

    float ks;
    vec3 specular;
    sampler2D specularMap;
    bool useSpecularMap;

    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 color;       
};

#define MAX_POINT_LIGHTS 100
#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_SPOT_LIGHTS 50

  
uniform vec3 viewPos;

uniform int numPointLights;
uniform int numDirLights;
uniform int numSpotLights;

uniform DirLight dirLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
// uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================

    vec3 result = vec3(0.0);

    // phase 1: directional lighting
    for(int i = 0; i < numDirLights; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);

    // phase 2: point lights
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    // phase 3: spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient;
    if (material.useDiffuseMap)
        ambient = vec3(texture(material.diffuseMap, TexCoords));
    else
        ambient = material.ambient;

    vec3 diffuse;
    if (material.useDiffuseMap)
        diffuse = light.color * diff * vec3(texture(material.diffuseMap, TexCoords));
    else
        diffuse = light.color * diff * material.diffuse;

    vec3 specular;
    if (material.useSpecularMap)
        specular = light.color * spec * vec3(texture(material.specularMap, TexCoords));
    else
        specular = light.color * spec * material.specular;

    return (material.ka * ambient + material.kd * diffuse + material.ks * specular);
}


// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
  
    // combine results
    vec3 ambient;
    if (material.useDiffuseMap)
        ambient = vec3(texture(material.diffuseMap, TexCoords));
    else
        ambient = material.ambient;

    vec3 diffuse;
    if (material.useDiffuseMap)
        diffuse = light.color * diff * vec3(texture(material.diffuseMap, TexCoords));
    else
        diffuse = light.color * diff * material.diffuse;

    vec3 specular;
    if (material.useSpecularMap)
        specular = light.color * spec * vec3(texture(material.specularMap, TexCoords));
    else
        specular = light.color * spec * material.specular;

    diffuse *= attenuation;
    specular *= attenuation;
    return (material.ka * ambient + material.kd * diffuse + material.ks * specular);
}

// // calculates the color when using a spot light.
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     // attenuation
//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//     // spotlight intensity
//     float theta = dot(lightDir, normalize(-light.direction)); 
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//     // combine results
//     vec3 ambient = light.color * vec3(texture(material.diffuse, TexCoords));
//     vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, TexCoords));
//     vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoords));
//     ambient *= attenuation * intensity;
//     diffuse *= attenuation * intensity;
//     specular *= attenuation * intensity;
//     return (material.ka * ambient + material.kd * diffuse + material.ks * specular);
// }