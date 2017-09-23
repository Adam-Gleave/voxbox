#pragma warning(disable:4996)

#include "chunkmanager.h"
using namespace glm;

ChunkManager::ChunkManager() {
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

ChunkManager::~ChunkManager() {
	for (int x = 0; x < CHUNKS_WORLD_X; ++x) {
		for (int y = 0; y < CHUNKS_WORLD_Y; ++y) {
			for (int z = 0; z < CHUNKS_WORLD_Z; ++z) {
				delete _chunks[x][y][z];
			}
		}
	}
}

void ChunkManager::render(Renderer *renderer, vec3 position) {
	manageChunks(position);

	for (int x = 0; x < CHUNKS_WORLD_X; ++x) {
		for (int y = 0; y < CHUNKS_WORLD_Y; ++y) {
			for (int z = 0; z < CHUNKS_WORLD_Z; ++z) {
				//Run render function on each chunk
				_chunks[x][y][z]->render(renderer);
			}
		}
	}
}

void ChunkManager::manageChunks(vec3 position) {
	//int x = round(position.x);
	//int y = round(position.y);
	//int z = round(position.z);

	////Movement in +x direction, generate new set of chunks
	//if (x > _chunks[4][0][4]->getX()) {
	//	//Obtain x offset of furthest block in +x
	//	int xOffset = _chunks[CHUNKS_WORLD_X - 1][0][0]->getX();

	//	//Delete chunks furthest in -x
	//	for (int y = 0; y < CHUNKS_WORLD_Y; y++) {
	//		for (int z = 0; z < CHUNKS_WORLD_Z; z++) {
	//			delete _chunks[0][y][z];
	//		}
	//	}

	//	//Shift the contents of the chunk array down 1 x dimension
	//	memcpy(&_chunks[0][0][0], &_chunks[1][0][0], 5 * (sizeof(_chunks) / 6));
	//	
	//	for (int y = 0; y < CHUNKS_WORLD_Y; y++) {
	//		for (int z = 0; z < CHUNKS_WORLD_Z; z++) {
	//			int yOffset = y * CHUNK_Y;
	//			int zOffset = z * CHUNK_Z;

	//			_chunks[CHUNKS_WORLD_X - 1][y][z] = new Chunk(xOffset + CHUNK_X, yOffset, zOffset);
	//			populateChunk(_chunks[CHUNKS_WORLD_X - 1][y][z], xOffset + CHUNK_X, yOffset, zOffset);
	//		}
	//	}
	//}

	////Movement in -x direction, generate new set of chunks
	//if (x < _chunks[2][0][4]->getX()) {
	//	//Obtain x offset of furthest block in -x
	//	int xOffset = _chunks[0][0][0]->getX();

	//	//Delete chunks furthest in -x
	//	for (int y = 0; y < CHUNKS_WORLD_Y; y++) {
	//		for (int z = 0; z < CHUNKS_WORLD_Z; z++) {
	//			delete _chunks[CHUNKS_WORLD_X - 1][y][z];
	//		}
	//	}

	//	//Shift the contents of the chunk array up 1 x dimension
	//	memcpy(&_chunks[1][0][0], &_chunks[0][0][0], 5 * (sizeof(_chunks) / 6));

	//	for (int y = 0; y < CHUNKS_WORLD_Y; y++) {
	//		for (int z = 0; z < CHUNKS_WORLD_Z; z++) {
	//			int yOffset = y * CHUNK_Y;
	//			int zOffset = z * CHUNK_Z;

	//			_chunks[0][y][z] = new Chunk(xOffset - CHUNK_X, yOffset, zOffset);
	//			populateChunk(_chunks[0][y][z], xOffset - CHUNK_X, yOffset, zOffset);
	//		}
	//	}
	//}
}

void ChunkManager::populateChunk(Chunk *chunk, int xOff, int yOff, int zOff) {
	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				int xcoord = x + xOff;
				int ycoord = y + yOff;
				int zcoord = z + zOff;

				int vertSolidCount = 0;

				if (ycoord == 63) {
					auto debug = true;
				}

				//Determine the density of each vertex of the voxel
				chunk->gridVertDensities[x][y][z] = densityFunction.getDensity(xcoord - 0.5, ycoord - 0.5, zcoord - 0.5, &vertSolidCount);
				chunk->gridVertDensities[x + 1][y][z] = densityFunction.getDensity(xcoord + 0.5, ycoord - 0.5, zcoord - 0.5, &vertSolidCount);
				chunk->gridVertDensities[x + 1][y][z + 1] = densityFunction.getDensity(xcoord + 0.5, ycoord - 0.5, zcoord + 0.5, &vertSolidCount);
				chunk->gridVertDensities[x][y][z + 1] = densityFunction.getDensity(xcoord - 0.5, ycoord - 0.6, zcoord + 0.5, &vertSolidCount);
				chunk->gridVertDensities[x][y + 1][z] = densityFunction.getDensity(xcoord - 0.5, ycoord + 0.5, zcoord - 0.5, &vertSolidCount);
				chunk->gridVertDensities[x + 1][y + 1][z] = densityFunction.getDensity(xcoord + 0.5, ycoord + 0.5, zcoord - 0.5, &vertSolidCount);
				chunk->gridVertDensities[x + 1][y + 1][z + 1] = densityFunction.getDensity(xcoord + 0.5, ycoord + 0.5, zcoord + 0.5, &vertSolidCount);
				chunk->gridVertDensities[x][y + 1][z + 1] = densityFunction.getDensity(xcoord - 0.5, ycoord + 0.5, zcoord + 0.5, &vertSolidCount);

				bool solid = false;

				if (vertSolidCount >= 2) {
					solid = true;
				}				

				uint8_t type = 0;

				if (solid) {
					if (ycoord > 52) {
						type = 3;
					}
					else if (ycoord < 28) {
						type = 2;
					}
					else {
						type = 1;
					}
				}

				chunk->setBlock(x, y, z, type);
			}
		}
	}
}

Chunk *ChunkManager::getChunk(int x, int y, int z) {
	return _chunks[x][y][z];
}
