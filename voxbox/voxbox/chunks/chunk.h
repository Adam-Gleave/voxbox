#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

#include "block.h"
#include "../renderer/renderer.h"

class Chunk {
public:
	Chunk();
	~Chunk();

	GLbyte getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, GLbyte type);
	void update();
	void render(Renderer *renderer);

private:
	Block _blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	GLuint _vbo;
	int _elements;
	bool _changed;
};
