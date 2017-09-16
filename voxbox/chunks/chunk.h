#define CHUNK_X 32
#define CHUNK_Y 32
#define CHUNK_Z 32

#define CHUNKS_WORLD_X 8
#define CHUNKS_WORLD_Y 2
#define CHUNKS_WORLD_Z 8

#include <glm/gtc/matrix_transform.hpp>
#include "block.h"
#include "../renderer/renderer.h"

typedef glm::tvec3<GLbyte> byte3;

class Chunk {
public:
	Chunk(int xOffset, int yOffset, int zOffset);
	~Chunk();

	GLbyte getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, GLbyte type);
	void updateMesh();
	void updateBuffers(Renderer *renderer);
	void render(Renderer *renderer);

private:
	byte3 verts[CHUNK_X * CHUNK_Y * CHUNK_Z * 36 * 2];
	Block _blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	GLuint _vao;
	GLuint _vbo;
	int _offset[3];
	int _elements;
	bool _changed;
};