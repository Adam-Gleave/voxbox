class Shaders 
{
public:
	static inline const char *vertShader() { 
		return R"glsl(
			#version 430 core

			in vec3 coord;
			in vec3 normal;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 proj;

			out vec3 FragPos;
			out vec3 Normal;

			void main() {
				gl_Position = proj * view * model * vec4(coord, 1.0);

				//Information to pass to fragment shader
				//Work out vector of fragment to model
				FragPos = vec3(model * vec4(coord, 1.0));
				Normal = normal;
			}
		)glsl";
	};

	static inline const char *fragShader() { 
		return R"glsl(
			#version 430 core

			in vec3 Normal;
			in vec3 FragPos;

			uniform vec3 lightColor;
			uniform vec3 lightPos;

			out vec4 outColor;

			void main() {
				//Base brown color for testing
				vec3 objectColor = vec3(1.0, 0.5, 0.31);
				
				//Ambient lighting
				float ambientStrength = 0.1;
				vec3 ambient = ambientStrength * lightColor;				

				//Normal computation
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(lightPos - FragPos);

				//Diffuse lighting
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = diff * lightColor;
				vec3 result = (ambient + diffuse) * objectColor;

				outColor = vec4(result, 1.0);
			}
		)glsl";
	};
};