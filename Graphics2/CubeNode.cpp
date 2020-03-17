#include "CubeNode.h"

bool CubeNode::Initialise()
{
	// Single triangle test.
	_mesh->AddTriangle(Vector3::UpVector, Vector3::DownVector + Vector3::RightVector, Vector3::DownVector + Vector3::LeftVector);
	_mesh->Apply();

	// Set default shader.
	_material->SetShader(Shader::Load(L"shader.hlsl"));

	// I hope?
	return true;
}
