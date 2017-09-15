#include <glm/glm.hpp>
#include <cstring>
#include "chunk.h"
using namespace glm;

Chunk::Chunk(int xOffset, int yOffset, int zOffset) {
	//Populate block array with default (empty) blocks
	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				_blocks[x][y][z] = Block();
			}
		}
	}

	_offset[0] = xOffset;
	_offset[1] = yOffset;
	_offset[2] = zOffset;
	_elements = 0;
	_changed = true;
}

Chunk::~Chunk() {
	glDeleteBuffers(1, &_vbo);
}

void Chunk::render(Renderer *renderer) {
	if (_changed) {
		updateMesh();
		updateBuffers(renderer);
	}

	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, (_elements / 2));
}

GLbyte Chunk::getBlock(int x, int y, int z) {
	return _blocks[x][y][z].getType();
}

void Chunk::setBlock(int x, int y, int z, GLbyte type) {
	_changed = true;
	_blocks[x][y][z].setType(type);
}

void Chunk::updateMesh() {
	int vertCount = 0;

	_changed = false;

	for (int i = 0; i < CHUNK_X; ++i) {
		for (int j = 0; j < CHUNK_Y; ++j) {
			for (int k = 0; k < CHUNK_Z; ++k) {
				GLbyte type = _blocks[i][j][k].getType();

				//Don't add vertices of empty (default) blocks
				if (type == 0) {
					continue;
				}

				int x = i + _offset[0];
				int y = j + _offset[1];
				int z = k + _offset[2];

				verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 0, -1);
				verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(0, 0, -1);
				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, 0, -1);
				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, 0, -1);
				verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 0, -1),
				verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 0, -1),

				verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, 0, 1);
				verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, 0, 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 0, 1);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 0, 1),
				verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(0, 0, 1),
				verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, 0, 1);

				verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(-1, 0, 0);
				verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(-1, 0, 0);
				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(-1, 0, 0);
				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(-1, 0, 0);
				verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(-1, 0, 0);
				verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(-1, 0, 0);

				verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(1, 0, 0);
				verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(1, 0, 0);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(1, 0, 0);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(1, 0, 0);
				verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(1, 0, 0);
				verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(1, 0, 0);

				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, -1, 0);
				verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(0, -1, 0);
				verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, -1, 0);
				verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, -1, 0);
				verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, -1, 0);
				verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, -1, 0);

				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 1, 0);
				verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 1, 0);
				verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 1, 0);
				verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 1, 0);
				verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(0, 1, 0);
				verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 1, 0);
			}
		}
	}

	_elements = vertCount;
}

void Chunk::updateBuffers(Renderer *renderer) {
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glBufferData(GL_ARRAY_BUFFER, _elements * 3, verts, GL_STATIC_DRAW);

	GLint positionAttrib = glGetAttribLocation(renderer->_shaderProgram, "coord");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_BYTE, GL_FALSE, 6, 0);

	GLint normalAttrib = glGetAttribLocation(renderer->_shaderProgram, "normal");
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_BYTE, GL_FALSE, 6, (void*)3);
}