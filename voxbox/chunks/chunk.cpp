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

	GLint modelUniform = glGetUniformLocation(renderer->_shaderProgram, "model");
	mat4 model = translate(mat4(1.0), vec3(_offset[0], _offset[1], _offset[2]));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(model));

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

	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				GLbyte type = _blocks[x][y][z].getType();

				//Don't add vertices of empty (default) blocks
				if (type == 0) {
					continue;
				}

				//Cull faces that are occluded by other blocks
				if (z == 0 || _blocks[x][y][z - 1].getType() == 0) {
					verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 0, -1);
					verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(0, 0, -1);
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, 0, -1);
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, 0, -1);
					verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 0, -1);
					verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 0, -1);
				}

				if (z == CHUNK_Z - 1 || _blocks[x][y][z + 1].getType() == 0) {
					verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, 0, 1);
					verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, 0, 1);
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 0, 1);
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 0, 1);
					verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(0, 0, 1);
					verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, 0, 1);
				}

				if (x == 0 || _blocks[x - 1][y][z].getType() == 0) {
					verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(-1, 0, 0);
					verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(-1, 0, 0);
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(-1, 0, 0);
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(-1, 0, 0);
					verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(-1, 0, 0);
					verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(-1, 0, 0);
				}

				if (x == CHUNK_X - 1 || _blocks[x + 1][y][z].getType() == 0) {
					verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(1, 0, 0);
					verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(1, 0, 0);
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(1, 0, 0);
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(1, 0, 0);
					verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(1, 0, 0);
					verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(1, 0, 0);
				}

				if (y == 0 || _blocks[x][y - 1][z].getType() == 0) {
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, -1, 0);
					verts[vertCount++] = byte3(x + 1, y, z);			verts[vertCount++] = byte3(0, -1, 0);
					verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, -1, 0);
					verts[vertCount++] = byte3(x + 1, y, z + 1);		verts[vertCount++] = byte3(0, -1, 0);
					verts[vertCount++] = byte3(x, y, z + 1);			verts[vertCount++] = byte3(0, -1, 0);
					verts[vertCount++] = byte3(x, y, z);				verts[vertCount++] = byte3(0, -1, 0);
				}

				if (y == CHUNK_Y - 1 || _blocks[x][y + 1][z].getType() == 0) {
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 1, 0);
					verts[vertCount++] = byte3(x + 1, y + 1, z);		verts[vertCount++] = byte3(0, 1, 0);
					verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 1, 0);
					verts[vertCount++] = byte3(x, y + 1, z);			verts[vertCount++] = byte3(0, 1, 0);
					verts[vertCount++] = byte3(x, y + 1, z + 1);		verts[vertCount++] = byte3(0, 1, 0);
					verts[vertCount++] = byte3(x + 1, y + 1, z + 1);	verts[vertCount++] = byte3(0, 1, 0);
				}
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