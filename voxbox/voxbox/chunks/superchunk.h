#define CHUNKS_WORLD_X 2
#define CHUNKS_WORLD_Y 1
#define CHUNKS_WORLD_Z 2

#include "chunk.h"

class SuperChunk {
public:
	SuperChunk();
	~SuperChunk();

	Chunk *getChunk(int x, int y, int z);
	void render(Renderer *renderer);

private:
	void populateChunk(Chunk *chunk);
	Chunk *_chunks[CHUNKS_WORLD_X][CHUNKS_WORLD_Y][CHUNKS_WORLD_Z];
};
