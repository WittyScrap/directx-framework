#include "NoiseManager.h"
#include "BasicNoise.h"

FLOAT NoiseManager::GetNoiseValue(FLOAT radius, FLOAT x, FLOAT y, FLOAT z) const
{
	float t = 0.f;

	for (const auto& noise : _noises)
	{
		switch (noise->GetNoiseBlendMode())
		{
		case NoiseBlendMode::BM_Additive:
			t += noise->GetNoiseValue(x, y, z);
			break;

		case NoiseBlendMode::BM_Multiply:
			t *= noise->GetNoiseValue(x, y, z);
			break;
		}
	}

	t += radius;

	clamp(t, _minHeight, radius + _maxHeight);

	return t;
}
