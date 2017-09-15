#define GLEW_STATIC
#include <GL/glew.h>

class Block {
public:
	Block() { _type = 0; }
	~Block() { }

	inline GLbyte getType() { return _type; }
	inline void setType(GLbyte type) { _type = type; }

private:
	GLbyte _type;
};