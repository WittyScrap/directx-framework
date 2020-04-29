#include "PlanetNode.h"
#include <fstream>

#define clamp(a, x, y) { a = ((a) > (x) ? ((a) < (y) ? (a) : (y)) : (x)); }

bool PlanetNode::Initialise()
{
	return Generate();
}

bool PlanetNode::Generate()
{
	shared_ptr<Mesh> terrainData = make_shared<Mesh>();

	// Generate 6 faces

	GenerateFace(terrainData.get(), Vector3::UpVector,			Vector3::RightVector);		// Up
	GenerateFace(terrainData.get(), Vector3::ForwardVector,		Vector3::RightVector);		// Forward
	GenerateFace(terrainData.get(), Vector3::DownVector,		Vector3::RightVector);		// Down
	GenerateFace(terrainData.get(), Vector3::BackwardVector,	Vector3::RightVector);		// Back
	GenerateFace(terrainData.get(), Vector3::RightVector,		Vector3::BackwardVector);	// Right
	GenerateFace(terrainData.get(), Vector3::LeftVector,		Vector3::ForwardVector);	// Left

	terrainData->SetMode(_draw);
	terrainData->Apply();

	SetMesh(terrainData);

	return true;
}

void PlanetNode::GenerateFace(Mesh* const target, Vector3 normal, Vector3 right) const
{
	/* Phase 1: Generate subdivided cube face mesh */

	const Vector3&& forward = Vector3::Cross(right, normal);
	const UINT&& startIndex = (UINT)target->GetVertices().size();

	vector<Vector3> generatedVertices;
	vector<UINT>	generatedIndices;

	for (UINT it_y = 0; it_y < _resolution; it_y++)
	{
		for (UINT it_x = 0; it_x < _resolution; it_x++)
		{
			FLOAT x = GetNormalizedValue(it_x, _resolution);
			FLOAT y = GetNormalizedValue(it_y, _resolution);

			generatedVertices.push_back(right * x + normal + forward * y);
		}
	}

	// Generate indices

	for (size_t it = 0; it < generatedVertices.size() - _resolution; ++it) // Remove the last row
	{
		const size_t it_x = it % _resolution;
		const size_t it_y = it / _resolution;

		const size_t lt_x = it_x + 1;
		const size_t bt_y = it_y + 1;

		if (lt_x < _resolution)
		{
			const size_t it_left = lt_x + _resolution * it_y;		// Index for vertex to the LEFT of this one
			const size_t it_bottomLeft = lt_x + _resolution * bt_y; // Index for vertex to the BOTTOM-LEFT of this one
			const size_t it_bottom = it_x + _resolution * bt_y;		// Index for vertex to the BOTTOM of this one

			/* First triangle */

			generatedIndices.push_back((UINT)it_bottomLeft);
			generatedIndices.push_back((UINT)it_left);
			generatedIndices.push_back((UINT)it);

			/* Second triangle */

			generatedIndices.push_back((UINT)it);
			generatedIndices.push_back((UINT)it_bottom);
			generatedIndices.push_back((UINT)it_bottomLeft);
		}
	}

	/* Phase 2: Inflate vertices to create planetoid shape */

	for (size_t i = 0; i < generatedVertices.size(); ++i)
	{
		generatedVertices[i].Normalize();
		generatedVertices[i] *= _radius;

		// Save onto actual mesh
		target->AddVertex(Vertex{
			generatedVertices[i].ToDX3(),
			generatedVertices[i].Normalized().ToDX3(),
			{ 0, 0 }
		});
	}

	for (size_t i = 0; i < generatedIndices.size(); ++i)
	{
		target->AddIndex(generatedIndices[i] + startIndex);
	}
}

constexpr FLOAT PlanetNode::GetNormalizedValue(const UINT& value, const UINT& range) const
{
	if (value == 0)
	{
		return 0.f;
	}

	if (value == 1)
	{
		return 1.f;
	}

	return static_cast<float>(value) / static_cast<float>(range) * 2 - 1;
}

FLOAT PlanetNode::GetNoiseValue(FLOAT x, FLOAT y, FLOAT z) const
{
	return _constantValue;
}