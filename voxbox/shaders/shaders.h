class Shaders 
{
public:
	static inline const char *vertShader() { 
		return R"glsl(
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
		)glsl";
	};

	static inline const char *fragShader() { 
		return R"glsl(
			#version 430 core

			in vec3 Normal;
			in vec3 FragPos;
			in float Type;

			uniform vec3 cameraPos;
			uniform vec3 lightColor;
			uniform vec3 lightPos;

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
				float ambientStrength = 0.15;
				vec3 ambient = ambientStrength * lightColor;				

				//Diffuse
				//Normal computation for diffuse
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(lightPos - FragPos);
				//Diffuse lighting
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = diff * lightColor;

				//Specular highlighting
				vec3 viewDir = normalize(cameraPos - FragPos);
				vec3 reflectDir = reflect(-lightDir, norm);
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPower);
				vec3 specular = specStrength * spec * lightColor;

				vec3 result = (ambient + diffuse + specular) * objectColor;

				outColor = vec4(result, 1.0);
			}
		)glsl";
	};
};