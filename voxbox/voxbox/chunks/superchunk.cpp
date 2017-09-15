#include "superchunk.h"

SuperChunk::SuperChunk() {
	for (int i = 0; i < CHUNKS_WORLD_X; ++i) {
		for (int j = 0; j < CHUNKS_WORLD_Y; ++j) {
			for (int k = 0; k < CHUNKS_WORLD_Z; ++k) {
				//Work out offset of chunk in blocks
				int x = i * CHUNK_X;
				int y = j * CHUNK_Y;
				int z = k * CHUNK_Z;

				//Populate array with chunks
				_chunks[i][j][k] = new Chunk(x, y, z);
				//Temp function: Fill chunk with blocks
				populateChunk(_chunks[i][j][k]);
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

void SuperChunk::populateChunk(Chunk *chunk) {
	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				//Temporary -- set all blocks to render (type 1)
				chunk->setBlock(x, y, z, 1);
			}
		}
	}
}

Chunk *SuperChunk::getChunk(int x, int y, int z) {
	return _chunks[x][y][z];
}
