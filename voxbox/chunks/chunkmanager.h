#include <FastNoise.h>
#include "chunk.h"
using namespace glm;

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

	Chunk *getChunk(int x, int y, int z);
	void render(Renderer *renderer, vec3 position);
	void manageChunks(vec3 position);

private:
	void populateChunk(Chunk *chunk, int xOff, int yOff, int zOff);
	Chunk *_chunks[CHUNKS_WORLD_X][CHUNKS_WORLD_Y][CHUNKS_WORLD_Z];
	vec2 _xBuffer[CHUNKS_WORLD_Y][CHUNKS_WORLD_Z];
	FastNoise _noise;
};
