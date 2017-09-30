#define CHUNK_X 32
#define CHUNK_Y 32
#define CHUNK_Z 32

#define CHUNKS_WORLD_X 3
#define CHUNKS_WORLD_Y 2
#define CHUNKS_WORLD_Z 3

#define WORLD_HEIGHT_CENTER ((CHUNKS_WORLD_Y * CHUNK_Y) / 2)

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "block.h"
#include "../renderer/renderer.h"
#include "density.h"
#include "qef.h"

typedef glm::tvec3<GLbyte> byte3;
typedef glm::tvec4<GLbyte> byte4;

struct Vertex {
	byte4 coords;
	byte3 normals;
};

class Chunk {
public:
	Chunk(int xOffset, int yOffset, int zOffset);
	~Chunk();

	GLbyte getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, GLbyte type);
	void updateMesh();
	void updateBuffers(Renderer *renderer);
	void render(Renderer *renderer);
	int getX() { return _offset[0]; };
	int getY() { return _offset[1]; };
	int getZ() { return _offset[2]; };
	float gridVertDensities[CHUNK_X + 1][CHUNK_Y + 1][CHUNK_Z + 1];
	vec3 getIntersectionPoint(const vec3 &pos1, const vec3 &pos2, const int edgeRef);
	vec3 getIntersectionNormal(const vec3 &pos);

private:
	DensityFunction densityFunction = DensityFunction();
	Vertex _verts[CHUNK_X * CHUNK_Y * CHUNK_Z * 36];
	Block *_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	GLuint _vao;
	GLuint _vbo;
	int _offset[3];
	int _elements;
	bool _changed;
};