#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform	Material material;

uniform vec3 lightPos0;
uniform vec3 camPos;

vec3 calculateDiffuseLight() {
	vec3 posToLight = normalize(lightPos0 - vs_position);
	vec3 diffuseColor = material.diffuse;
	float diffuse = clamp(dot(posToLight, vs_normal), 0, 1);
	vec3 diffuseLight = diffuseColor * diffuse;
	return diffuseLight;
}

vec3 calculateSpecularLight() {
	vec3 lightToPos = normalize(vs_position - lightPos0);
	vec3 reflectDir = normalize(reflect(lightToPos, normalize(vs_normal)));
	vec3 posToView = normalize(camPos - vs_position);
	float specular = pow(max(dot(posToView, reflectDir), 0), 50);
	vec3 specularLight = material.specular * specular * texture(material.specularTex, vs_texcoord).rgb;
	return specularLight;
}

void main() {
	vec3 ambientLight = material.ambient;
	vec3 diffuseLight = calculateDiffuseLight();
	vec3 specularLight = calculateSpecularLight();
	vec4 light = vec4(ambientLight, 1.f) + vec4(diffuseLight, 1.f) + vec4(specularLight, 1.f);
	fs_color = vec4(1.25f) * texture(material.diffuseTex, vs_texcoord) * light;
}