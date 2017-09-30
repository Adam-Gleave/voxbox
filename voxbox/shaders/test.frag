#version 430 core

in vec3 Normal;
in vec3 FragPos;
in float Type;

uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 lightDir;

out vec4 outColor;

void main() {
	vec3 objectColor;	
	float specStrength = 0.8;
	int specPower;			

	//Determine color based on block type
	if (round(Type) == 3) {
		objectColor = vec3(0.9, 0.9, 0.9);
		specStrength = 0.95;
		specPower = 230; 
	} 
	else if (round(Type) == 2) {
		objectColor = vec3(0.45, 0.45, 0.45);
		specStrength = 0.4;
		specPower = 32;
	}
	else {
		//Base brown color for testing (type 1)
		objectColor = vec3(0.8, 0.35, 0.21);
		specStrength = 0.6;
		specPower = 96;
	}				

	//Ambient lighting
	float ambientStrength = 0.45;
	vec3 ambient = ambientStrength * lightColor;				

	//Diffuse
	//Normal computation for diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(-lightDir);

	//Diffuse lighting
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular highlighting
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
	vec3 specular = specStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	outColor = vec4(result, 1.0);
}