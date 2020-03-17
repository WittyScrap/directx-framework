#include "CubeNode.h"

bool CubeNode::Initialise()
{
	// Set vertices
	_mesh->SetVertices
	({
		Vector3(-1.0f, -1.0f, 1.0f),
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, 1.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f),

		Vector3(-1.0f, -1.0f, -1.0f),
		Vector3(-1.0f, 1.0f, -1.0f),
		Vector3(1.0f, -1.0f, -1.0f),
		Vector3(1.0f, 1.0f, -1.0f),

		Vector3(-1.0f, 1.0f, -1.0f),
		Vector3(-1.0f, 1.0f, 1.0f),
		Vector3(1.0f, 1.0f, -1.0f),
		Vector3(1.0f, 1.0f, 1.0f),

		Vector3(-1.0f, -1.0f, -1.0f),
		Vector3(1.0f, -1.0f, -1.0f),
		Vector3(-1.0f, -1.0f, 1.0f),
		Vector3(1.0f, -1.0f, 1.0f),

		Vector3(1.0f, -1.0f, -1.0f),
		Vector3(1.0f, 1.0f, -1.0f),
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f),

		Vector3(-1.0f, -1.0f, -1.0f),
		Vector3(-1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, 1.0f, -1.0f),
		Vector3(-1.0f, 1.0f, 1.0f)
	});

	// Set triangles
	_mesh->SetIndices
	({
		0, 1, 2,       // side 1
		2, 1, 3,
		4, 5, 6,       // side 2
		6, 5, 7,
		8, 9, 10,      // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	});

	// Apply changes
	_mesh->Apply();

	// Set default shader.
	_material->SetShader(Shader::Load(L"shader.hlsl"));

	// I hope?
	return true;
}
