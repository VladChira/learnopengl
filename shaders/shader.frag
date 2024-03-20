#version 330 core
out vec4 FragColor;

in vec3 Normal; 
in vec3 FragPos;
in vec2 TexCoords; 

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    float ka;
    sampler2D diffuse;
    float kd;
    vec3 specular;
    float ks;
    float shininess;
}; 
  
uniform Material material;

void main()
{
    // ambient
    vec3 ambient = (lightColor * vec3(texture(material.diffuse, TexCoords)));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * vec3(texture(material.diffuse, TexCoords)));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = material.ka * ambient + material.kd * diffuse + material.ks * specular;
    FragColor = vec4(result, 1.0);
}