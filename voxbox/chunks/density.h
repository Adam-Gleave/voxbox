#include "FastNoise.h"

class DensityFunction {
public:
	DensityFunction() {
		//Initialise noise function
		_noise.SetNoiseType(FastNoise::SimplexFractal);
		_noise.SetSeed(825631987);
		_noise.SetFractalType(FastNoise::RigidMulti);
		_noise.SetFrequency(0.005);
		_noise.SetFractalOctaves(6);
		_noise.SetFractalLacunarity(1.9);
	}

	~DensityFunction() {};

	float getDensity(float x, float y, float z, int *isSolid) {
		float density = getDensity(x, y, z);

		if (density < 0) {
			*isSolid += 1;
		}

		return density;
	}

	float getDensity(float x, float y, float z) {
		//Values used in density function
		int midpoint = 32;
		float density = 0.25;
		float heightDifference = -(float)(y - midpoint) / midpoint;

		density += _noise.GetNoise(x, y, z) * 1.3;

		//Hard floor below certain y value
		if (y < 26) {
			heightDifference *= 1.15 * (26 - y);
		}

		//Finalise density value
		density -= heightDifference;

		return density;
	}

	float getDensity(const vec3 vectorFloat) {
		const float x = vectorFloat.x;
		const float y = vectorFloat.y;
		const float z = vectorFloat.z;

		return getDensity(x, y, z);
	}

	FastNoise _noise;
};
