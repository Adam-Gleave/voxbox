#include <glm/glm.hpp>
#include <cstring>
#include "chunk.h"
using namespace glm;

typedef tvec3<GLbyte> byte3;

Chunk::Chunk() {
	//Populate block array with default (empty) blocks
	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				_blocks[x][y][z] = Block();
			}
		}
	}

	_elements = 0;
	_changed = true;
	glGenBuffers(1, &_vbo);
}

Chunk::~Chunk() {
	glDeleteBuffers(1, &_vbo);
}

GLbyte Chunk::getBlock(int x, int y, int z) {
	return _blocks[x][y][z].getType();
}

void Chunk::setBlock(int x, int y, int z, GLbyte type) {
	_blocks[x][y][z].setType(type);
}

void Chunk::update() {
	//Create a new array to store vertex coords (4 bytes each)
	byte3 verts[CHUNK_X * CHUNK_Y * CHUNK_Z * 36];
	int vertCount = 0;

	_changed = false;

	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				GLbyte type = _blocks[x][y][z].getType();

				//Don't add vertices of empty (default) blocks
				if (type == 0) {
					continue;
				}

				//Left side (looking from -z)
				verts[vertCount++] = byte3(x, y, z);
				verts[vertCount++] = byte3(x, y, z + 1);
				verts[vertCount++] = byte3(x, y + 1, z);
				verts[vertCount++] = byte3(x, y + 1, z);
				verts[vertCount++] = byte3(x, y, z + 1);
				verts[vertCount++] = byte3(x, y + 1, z + 1);

				//Right side
				verts[vertCount++] = byte3(x + 1, y, z);
				verts[vertCount++] = byte3(x + 1, y + 1, z);
				verts[vertCount++] = byte3(x + 1, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z);

				//Front side
				verts[vertCount++] = byte3(x, y, z);
				verts[vertCount++] = byte3(x, y + 1, z);
				verts[vertCount++] = byte3(x + 1, y, z);
				verts[vertCount++] = byte3(x + 1, y, z);
				verts[vertCount++] = byte3(x + 1, y + 1, z);
				verts[vertCount++] = byte3(x, y + 1, z);

				//Back side
				verts[vertCount++] = byte3(x, y, z + 1);
				verts[vertCount++] = byte3(x, y + 1, z + 1);
				verts[vertCount++] = byte3(x + 1, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);
				verts[vertCount++] = byte3(x, y + 1, z + 1);

				//Bottom side
				verts[vertCount++] = byte3(x, y, z);
				verts[vertCount++] = byte3(x + 1, y, z);
				verts[vertCount++] = byte3(x, y, z + 1);
				verts[vertCount++] = byte3(x, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y, z + 1);
				verts[vertCount++] = byte3(x + 1, y, z);

				//Top side
				verts[vertCount++] = byte3(x, y + 1, z);
				verts[vertCount++] = byte3(x + 1, y + 1, z);
				verts[vertCount++] = byte3(x, y + 1, z + 1);
				verts[vertCount++] = byte3(x, y + 1, z + 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z);
			}
		}
	}

	_elements = vertCount;
	glBindBuffer(1, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _elements * sizeof(*verts), verts, GL_STATIC_DRAW);
}

void Chunk::render(Renderer *renderer) {
	if (_changed) {
		update();
	}

	//glEnable(GL_CULL_FACE | GL_DEPTH_TEST);
	glBindBuffer(1, _vbo);

	GLint positionAttrib = glGetAttribLocation(renderer->_shaderProgram, "coord");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_BYTE, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, _elements);
}