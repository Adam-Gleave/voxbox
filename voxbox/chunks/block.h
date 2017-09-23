#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

struct DrawInfo {
	vec3 position;
	vec3 normals;
};

class Block {
public:
	Block() { _type = 0; }
	~Block() { }

	inline GLbyte getType() { return _type; }
	inline void setType(GLbyte type) { _type = type; }
	DrawInfo drawInfo;

private:
	GLbyte _type;
};