#include "TerrainNode.h"

bool TerrainNode::Initialise()
{
	return Generate();
}

bool TerrainNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();
	terrainData->SetMode(_draw);

	for (size_t it_x = 0; it_x < _width; it_x++)
	{
		for (size_t it_y = 0; it_y < _height; it_y++)
		{
			float x = static_cast<float>(it_x) - static_cast<float>(_width) / 2.f;
			float y = static_cast<float>(it_y) - static_cast<float>(_height) / 2.f;

			float height = 0;

			switch (_mode)
			{
			case TerrainMode::TM_Flat:
				height = GenerateFlat(it_x, it_y);
				break;

			case TerrainMode::TM_PerlinNoise:
				height = GenerateFromPerlin(it_x, it_y);
				break;

			case TerrainMode::TM_TextureSample:
				height = GenerateFromTexture(it_x, it_y);
				break;
			}

			terrainData->AddVertex(Vertex{
				XMFLOAT3{ x, height, y },
				XMFLOAT3{ 0, 1, 0},
				XMFLOAT2{ 0, 0 }
			});

			terrainData->AddVertex(Vertex{
				XMFLOAT3{ x + 1, height, y },
				XMFLOAT3{ 0, 1, 0 },
				XMFLOAT2{ 0, 0 }
			});

			terrainData->AddVertex(Vertex{
				XMFLOAT3{ x + 1, height, y + 1 },
				XMFLOAT3{ 0, 1, 0 },
				XMFLOAT2{ 0, 0 }
			});

			terrainData->AddVertex(Vertex{
				XMFLOAT3{ x, height, y + 1 },
				XMFLOAT3{ 0, 1, 0 },
				XMFLOAT2{ 0, 0 }
			});

			UINT index = (UINT)terrainData->GetVertices().size();

			terrainData->AddIndex(index + 2);
			terrainData->AddIndex(index + 1);
			terrainData->AddIndex(index);

			terrainData->AddIndex(index);
			terrainData->AddIndex(index + 3);
			terrainData->AddIndex(index + 2);
		}
	}

	terrainData->Apply();
	SetMesh(terrainData);

	return true;
}

FLOAT TerrainNode::GenerateFromTexture(size_t it_x, size_t it_y)
{
	return 0;
}

FLOAT TerrainNode::GenerateFromPerlin(size_t it_x, size_t it_y)
{
	return 0;
}

FLOAT TerrainNode::GenerateFlat(size_t it_x, size_t it_y)
{
	return _constantValue;
}
