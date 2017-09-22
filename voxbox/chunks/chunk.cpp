#include <glm/glm.hpp>
#include <cstring>
#include "chunk.h"
using namespace glm;

Chunk::Chunk(int xOffset, int yOffset, int zOffset) {
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
	glDrawArrays(GL_TRIANGLES, 0, _elements);
}

GLbyte Chunk::getBlock(int x, int y, int z) {
	return _blocks.at(x, y, z);
}

void Chunk::setBlock(int x, int y, int z, GLbyte type) {
	_changed = true;
	_blocks.set(x, y, z, type);
}

void Chunk::updateMesh() {
	int vertCount = 0;

	_changed = false;

	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				GLbyte type = _blocks.at(x, y, z);

				//Don't add vertices of empty (default) blocks
				if (type == 0) {
					continue;
				}

				//Cull faces that are occluded by other blocks
				if (z == 0 || _blocks.at(x, y, z - 1) == 0) {
					byte3 normals = byte3(0, 0, -1);

					//Add vertex coordinates to vertex buffer
					verts[vertCount].coords = byte4(x + 1, y + 1, z, type);		
					//Add surface normals and block type to vertex buffer
					verts[vertCount++].normals = normals;

					//Repeat for each vertex
					verts[vertCount].coords = byte4(x + 1, y, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z, type);				
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z, type);			
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z, type);		
					verts[vertCount++].normals = normals;
				}

				if (z == CHUNK_Z - 1 || _blocks.at(x, y, z + 1) == 0) {
					byte3 normals = byte3(0, 0, 1);

					verts[vertCount].coords = byte4(x, y, z + 1, type);			
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y, z + 1, type);		
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);	
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);	
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z + 1, type);		
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z + 1, type);			
					verts[vertCount++].normals = normals;
				}

				if (x == 0 || _blocks.at(x - 1, y, z) == 0) {
					byte3 normals = byte3(-1, 0, 0);

					verts[vertCount].coords = byte4(x, y + 1, z + 1, type);		
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z, type);			
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z, type);				
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z, type);				
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z + 1, type);
					verts[vertCount++].normals = normals;
				}

				if (x == CHUNK_X - 1 || _blocks.at(x + 1, y, z) == 0) {
					byte3 normals = byte3(1, 0, 0);

					verts[vertCount].coords = byte4(x + 1, y, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);	
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y, z + 1, type);
					verts[vertCount++].normals = normals;
					
					verts[vertCount].coords = byte4(x + 1, y, z, type);
					verts[vertCount++].normals = normals;
				}

				if (y == 0 || _blocks.at(x, y - 1, z) == 0) {
					byte3 normals = byte3(0, -1, 0);

					verts[vertCount].coords = byte4(x, y, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y, z, type);
					verts[vertCount++].normals = normals;
				}

				if (y == CHUNK_Y - 1 || _blocks.at(x, y + 1, z) == 0) {
					byte3 normals = byte3(0, 1, 0);

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x, y + 1, z + 1, type);
					verts[vertCount++].normals = normals;

					verts[vertCount].coords = byte4(x + 1, y + 1, z + 1, type);
					verts[vertCount++].normals = normals;
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

	glBufferData(GL_ARRAY_BUFFER, _elements * 7, verts, GL_STATIC_DRAW);

	GLint positionAttrib = glGetAttribLocation(renderer->_shaderProgram, "coord");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 4, GL_BYTE, GL_FALSE, 7, 0);

	GLint normalAttrib = glGetAttribLocation(renderer->_shaderProgram, "normal");
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_BYTE, GL_FALSE, 7, (void*)4);
}