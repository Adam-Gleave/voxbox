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
