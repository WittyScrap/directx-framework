#include "TerrainNode.h"
#include <fstream>

#define clamp(a, x, y) { a = ((a) > (x) ? ((a) < (y) ? (a) : (y)) : (x)); }

bool TerrainNode::Initialise()
{
	return Generate();
}

bool TerrainNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();
	terrainData->SetMode(_draw);

	for (size_t it_y = 0; it_y < _height; it_y++)
	{
		for (size_t it_x = 0; it_x < _width; it_x++)
		{
			float x = static_cast<float>(it_x) - static_cast<float>(_width) / 2.f;
			float y = static_cast<float>(it_y) - static_cast<float>(_height) / 2.f;

			float&& height	= GetHeight(it_x, it_y);

			float&& left	= GetHeight(it_x - 1, it_y);
			float&& right	= GetHeight(it_x + 1, it_y);
			float&& down	= GetHeight(it_x, it_y + 1);
			float&& up		= GetHeight(it_x, it_y - 1);

			Vector3 normal;

			normal.SetX(left - right);
			normal.SetY(down - up);
			normal.SetZ(2.f);
			normal.Normalize();
			
			terrainData->AddVertex(Vertex{
				XMFLOAT3{ x, height, y },
				normal.ToDX3(),
				XMFLOAT2{ (float)it_x / _width, (float)it_y / _height }
			});
		}
	}

	for (size_t it = 0; it < terrainData->GetVertices().size() - _width; ++it) // We subtract width to ignore the last row
	{
		const size_t it_x = it % _width;
		const size_t it_y = it / _width;

		const size_t lt_x = it_x + 1;
		const size_t bt_y = it_y + 1;

		if (lt_x < _width)
		{
			const size_t it_left = lt_x + _width * it_y; // Index for vertex to the LEFT of this one
			const size_t it_bottomLeft = lt_x + _width * bt_y; // Index for vertex to the BOTTOM-LEFT of this one
			const size_t it_bottom = it_x + _width * bt_y; // Index for vertex to the BOTTOM of this one

			/* First triangle */

			terrainData->AddIndex((UINT)it_bottomLeft);
			terrainData->AddIndex((UINT)it_left);
			terrainData->AddIndex((UINT)it);

			/* Second triangle */

			terrainData->AddIndex((UINT)it);
			terrainData->AddIndex((UINT)it_bottom);
			terrainData->AddIndex((UINT)it_bottomLeft);
		}
	}

	terrainData->Apply();
	SetMesh(terrainData);

	return true;
}

FLOAT TerrainNode::GenerateFromTexture(size_t it_x, size_t it_y)
{
	clamp(it_x, 0, _width - 1);
	clamp(it_y, 0, _height - 1);

	size_t it = (it_x * _width) + it_y;
	return _heightMap[it] * _peakHeight;
}

FLOAT TerrainNode::GenerateFromNoise(size_t it_x, size_t it_y)
{
	return (FLOAT)_noise.GetPerlin((FN_DECIMAL)it_x + _noiseOffsetX, (FN_DECIMAL)it_y + _noiseOffsetY) * _peakHeight;
}

FLOAT TerrainNode::GenerateFlat(size_t it_x, size_t it_y)
{
	return _constantValue;
}

FLOAT TerrainNode::GetHeight(size_t it_x, size_t it_y)
{
	switch (_mode) 
	{
	case TerrainMode::Flat:
		return GenerateFlat(it_x, it_y);

	case TerrainMode::Procedural:
		return GenerateFromNoise(it_x, it_y);

	case TerrainMode::TextureSample:
		return GenerateFromTexture(it_x, it_y);

	default:
		return 0;
	}
}

bool TerrainNode::LoadHeightMap(wstring heightMapFilename)
{
	_heightMap.clear();

	unsigned int mapSize = _width * _height;
	USHORT* rawFileValues = new USHORT[mapSize];

	std::ifstream inputHeightMap;
	inputHeightMap.open(heightMapFilename.c_str(), std::ios_base::binary);
	if (!inputHeightMap)
	{
		return false;
	}

	inputHeightMap.read((char*)rawFileValues, (long long)mapSize * 2);
	inputHeightMap.close();

	// Normalise BYTE values to the range 0.0f - 1.0f;
	for (unsigned int i = 0; i < mapSize; i++)
	{
		_heightMap.push_back((float)rawFileValues[i] / 65536);
	}

	delete[] rawFileValues;
	return true;
}