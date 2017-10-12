#include <glm/glm.hpp>
#include <cstring>
#include "chunk.h"
#include <list>
using namespace glm;

const float QEF_ERROR = 1e-20f;
const int QEF_SWEEPS = 4;

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
	return _blocks[x][y][z]->getType();
}

void Chunk::setBlock(int x, int y, int z, GLbyte type) {
	_changed = true;
	_blocks[x][y][z] = new Block();
	_blocks[x][y][z]->setType(type);
}

vec3 Chunk::getIntersectionPoint(const vec3 &pos1, const vec3 &pos2, const int edgeRef) {	
	const float density1 = densityFunction.getDensity(pos1.x, pos1.y, pos1.z);
	const float density2 = densityFunction.getDensity(pos2.x, pos2.y, pos2.z);
	float difference = density2 - density1;
	float distance = -density1 / difference;
	vec3 point;

	//Create coordinate vector from point
	if (edgeRef >= 0 && edgeRef < 4) {
		point = vec3(pos1.x + distance, pos1.y, pos1.z);
	}
	else if (edgeRef >= 4 && edgeRef < 8) {
		point = vec3(pos1.x, pos1.y + distance, pos1.z);
	}
	else if (edgeRef >= 8 && edgeRef < 12) {
		point = vec3(pos1.x, pos1.y, pos1.z + distance);
	}

	return point;
}

//Calculate normal by testing density function at intervals in each axis
vec3 Chunk::getIntersectionNormal(const vec3 &pos) {
	const float interval = 0.001f;
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	const float dx = densityFunction.getDensity(x + interval, y, z) - densityFunction.getDensity(x - interval, y, z);
	const float dy = densityFunction.getDensity(x, y + interval, z) - densityFunction.getDensity(x, y - interval, z);
	const float dz = densityFunction.getDensity(x, y, z + interval) - densityFunction.getDensity(x, y, z - interval);

	return normalize(vec3(dx, dy, dz));
}

void Chunk::updateMesh() {
	int vertCount = 0;

	_changed = false;

	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				//Maximum number of intersections to be a surface
				const int MAX_CROSSINGS = 6;

				//The vectore offset of each vertex from the x, y, z corner
				const ivec3 VERTEX_OFFSETS[8]{
					ivec3(0, 0, 0),	//"Origin" of voxel
					ivec3(1, 0, 0),
					ivec3(1, 0, 1),
					ivec3(0, 0, 1),
					ivec3(0, 1, 0),
					ivec3(1, 1, 0),
					ivec3(1, 1, 1),
					ivec3(0, 1, 1)
				};

				vec3 globalOffset = {
					_offset[0] + x,
					_offset[1] + y,
					_offset[2] + z
				};

				//Get vertex information
				float vertices[8];

				for (int i = 0; i < 8; i++) {
					//Calculate coordinates of each vertex and 
					int xOffset = VERTEX_OFFSETS[i].x;
					int yOffset = VERTEX_OFFSETS[i].y;
					int zOffset = VERTEX_OFFSETS[i].z;

					vertices[i] = gridVertDensities[x + xOffset][y + yOffset][z + zOffset];
				}

				//Map of vertices that correspond to each edge
				uint8_t edgeVertexMap[12][2] = {
					{ 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 },		// x-axis
					{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 },		// y-axis 
					{ 0, 3 }, { 1, 2 }, { 4, 7 }, { 5, 6 }		// z-axis
				};

				int edgeCount = 0;

				vec3 normals[MAX_CROSSINGS];
				vec3 positions[MAX_CROSSINGS];
				vec3 averageNormal = vec3(0.0f);
				svd::QefSolver qef;

				//Iterate through each edge, taking the vertex densities
				for (int i = 0; i < 12 && edgeCount < MAX_CROSSINGS; i++) {
					const int corner1 = edgeVertexMap[i][0];
					const int corner2 = edgeVertexMap[i][1];

					const float material1 = vertices[corner1];
					const float material2 = vertices[corner2];

					//If the edge is all part of mesh or air, it has no boundary
					if (material1 < 0 && material2 < 0 ||
						material1 >= 0 && material2 >= 0) {
						continue;
					}

					_blocks[x][y][z]->drawInfo.corners.set(corner1, (material1 < 0));
					_blocks[x][y][z]->drawInfo.corners.set(corner2, (material2 < 0));

					//Work out position of each corner in world as a vector
					const vec3 position1 = globalOffset + (vec3)VERTEX_OFFSETS[corner1];
					const vec3 position2 = globalOffset + (vec3)VERTEX_OFFSETS[corner2];

					//Calculate hermite data from these positions
					const vec3 intersectionPoint = getIntersectionPoint(position1, position2, i);
					const vec3 normals = getIntersectionNormal(intersectionPoint);
					averageNormal += normals;
					qef.add(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z,
						normals.x, normals.y, normals.z);

					edgeCount++;
				}

				if (edgeCount > 0) {
					svd::Vec3 qefPosition;
					qef.solve(qefPosition, QEF_ERROR, QEF_SWEEPS, QEF_ERROR);

					_blocks[x][y][z]->drawInfo.position = vec3(qefPosition.x, qefPosition.y, qefPosition.z);

					const vec3 min = globalOffset;
					const vec3 max = globalOffset + vec3(1.0f, 1.0f, 1.0f);
					const vec3 position = _blocks[x][y][z]->drawInfo.position;

					if ((position.x < min.x || position.x > max.x) ||
						(position.y < min.y || position.y > max.y) ||
						(position.z < min.z || position.z > max.z)) {
						const auto& massPoint = qef.getMassPoint();
						_blocks[x][y][z]->drawInfo.position = vec3(massPoint.x, massPoint.y, massPoint.z);
					}

					_blocks[x][y][z]->drawInfo.normals = normalize(averageNormal / (float)edgeCount);
					_blocks[x][y][z]->drawInfo.index = vertCount;

					vec3 position = _blocks[x][y][z]->drawInfo.position;
					_verts[vertCount].coords = vec4(position.x, position.y, position.z, _blocks[x][y][z]->getType());
					_verts[vertCount++].normals = _blocks[x][y][z]->drawInfo.normals;
				}
			}
		}
		_elements = vertCount;
	}

	for (int x = 0; x < CHUNK_X; ++x) {
		for (int y = 0; y < CHUNK_Y; ++y) {
			for (int z = 0; z < CHUNK_Z; ++z) {
				if (_blocks[x][y][z]->drawInfo.index < 0) {
					continue;
				}


			}
		}
	}
}

void Chunk::updateBuffers(Renderer *renderer) {
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glBufferData(GL_ARRAY_BUFFER, _elements * 7, _verts, GL_STATIC_DRAW);

	GLint positionAttrib = glGetAttribLocation(renderer->_shaderProgram, "coord");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

	GLint normalAttrib = glGetAttribLocation(renderer->_shaderProgram, "normal");
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
}
