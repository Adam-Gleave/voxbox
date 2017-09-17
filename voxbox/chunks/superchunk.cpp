#include "superchunk.h"

SuperChunk::SuperChunk() {
	_noise.SetNoiseType(FastNoise::SimplexFractal);
	_noise.SetSeed(98572398);
	_noise.SetFractalType(FastNoise::RigidMulti);
	_noise.SetFrequency(0.005);
	_noise.SetFractalOctaves(5);

	for (int i = 0; i < CHUNKS_WORLD_X; ++i) {
		for (int j = 0; j < CHUNKS_WORLD_Y; ++j) {
			for (int k = 0; k < CHUNKS_WORLD_Z; ++k) {
				//Work out offset of chunk in blocks
				int x = i * CHUNK_X;
				int y = j * CHUNK_Y;
				int z = k * CHUNK_Z;

				//Populate array with chunks
				_chunks[i][j][k] = new Chunk(x, y, z);

				populateChunk(_chunks[i][j][k], x, y, z);
			}
		}
	}
}

SuperChunk::~SuperChunk() {
	for (int x = 0; x < CHUNKS_WORLD_X; ++x) {
		for (int y = 0; y < CHUNKS_WORLD_Y; ++y) {
			for (int z = 0; z < CHUNKS_WORLD_Z; ++z) {
				delete _chunks[x][y][z];
			}
		}
	}
}

void SuperChunk::render(Renderer *renderer) {
	for (int x = 0; x < CHUNKS_WORLD_X; ++x) {
		for (int y = 0; y < CHUNKS_WORLD_Y; ++y) {
			for (int z = 0; z < CHUNKS_WORLD_Z; ++z) {
				//Run render function on each chunk
				_chunks[x][y][z]->render(renderer);
			}
		}
	}
}

void SuperChunk::populateChunk(Chunk *chunk, int xOff, int yOff, int zOff) {
	for (int x = 0; x < CHUNK_X; ++x) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				//Retrieve a coordinate-based height value using simplex noise
				int xcoord = x + xOff;
				int zcoord = z + zOff;
				float height = _noise.GetNoise(xcoord, zcoord);
				
				//Work out y height of blocks in x/z coordinate
				int blocksVertical = WORLD_HEIGHT_CENTER + floor(height * CHUNK_Y);

				//Set blocks under height range as active
				for (int y = 0; y < CHUNK_Y; ++y) {
					if (y < blocksVertical - yOff) {
						if (y + yOff > 40) {
							chunk->setBlock(x, y, z, 3);
						}
						else if (y + yOff < 24) {
							chunk->setBlock(x, y, z, 2);
						}
						else {
							chunk->setBlock(x, y, z, 1);
						}
					}
					else {
						chunk->setBlock(x, y, z, 0);
					}
				}
		}
	}
}

Chunk *SuperChunk::getChunk(int x, int y, int z) {
	return _chunks[x][y][z];
}
