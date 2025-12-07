#version 330 core

out vec4 frag_color;

struct Material {
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

// Le Soleil
struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// La Lampe (Pirozhok)
struct PointLight {
	vec3 position;  // Position de la lampe

	float constant; // Pour l'atténuation (distance)
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform DirectionalLight dirLight;
uniform PointLight pointLight; // On ajoute notre lampe
uniform Material material;
uniform vec3 viewPos;

// Fonction pour calculer la lumière directionnelle
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}

// Fonction pour calculer la lumière ponctuelle (Lampe)
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// Atténuation (Loi inverse du carré de la distance physique)
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 1. On calcule le soleil
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// 2. On AJOUTE la lumière du Pirozhok
	result += CalcPointLight(pointLight, norm, FragPos, viewDir);

	frag_color = vec4(result, 1.0);
}