class Shaders 
{
public:
	static inline const char *vertShader() { 
		return R"glsl(
			#version 430 core

			in vec4 coord;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 proj;

			void main() {
				gl_Position = proj * view * model * vec4(coord.xyz, 1.0);
			}
		)glsl";
	};

	static inline const char *fragShader() { 
		return R"glsl(
			#version 430 core

			out vec4 outColor;

			void main() {
				outColor = vec4(1.0, 0.0, 0.0, 1.0);
			}
		)glsl";
	};
};