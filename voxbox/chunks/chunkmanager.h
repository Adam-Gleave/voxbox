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
	DensityFunction densityFunction = DensityFunction();
	void populateChunk(Chunk *chunk, int xOff, int yOff, int zOff);
	vec2 _xBuffer[CHUNKS_WORLD_Y][CHUNKS_WORLD_Z];
	FastNoise _noise;
	FastNoise _3dNoise_1;
	FastNoise _3dNoise_2;
	Chunk *_chunks[CHUNKS_WORLD_X][CHUNKS_WORLD_Y][CHUNKS_WORLD_Z];
};
