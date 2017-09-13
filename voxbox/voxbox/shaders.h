class Shaders 
{
public:
	static inline const char *vertShader() 
	{ 
		return R"glsl(
			#version 150 core

			in vec3 coord3d;
			in vec3 color;

			uniform mat4 view;
			uniform mat4 proj;

			out vec3 Color;

			void main()
			{
				Color = color;
				gl_Position = proj * view * vec4(coord3d, 1.0);
			}
		)glsl";
	};

	static inline const char *fragShader() { 
		return R"glsl(
			#version 150 core

			in vec3 Color;

			out vec4 outColor;

			void main() 
			{
				outColor = vec4(Color, 1.0);
			}
		)glsl";
	};
};