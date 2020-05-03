#include "BasicNoise.h"

FLOAT BasicNoise::GetNoiseValue(FLOAT x, FLOAT y, FLOAT z) const
{
	float value = _noise.fractal(_octaves,
		x / _noiseScale + _noiseOffsetX,
		y / _noiseScale + _noiseOffsetY,
		z / _noiseScale + _noiseOffsetY) * _peakHeight * static_cast<int>(_direction);

	return value;
}