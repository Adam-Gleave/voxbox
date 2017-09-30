#version 430 core

in vec4 coord;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out float Type;

void main() {
	gl_Position = proj * view * model * vec4(coord.xyz, 1.0);

	//Information to pass to fragment shader
	//Work out vector of fragment to model
	FragPos = vec3(model * vec4(coord.xyz, 1.0));
	Normal = normal;
	Type = coord.w;
}