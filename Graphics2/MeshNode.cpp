#include "MeshNode.h"

void MeshNode::Render()
{
}

void MeshNode::Shutdown()
{
}

void MeshNode::Reset()
{
	_mesh.reset();
}

const Mesh& MeshNode::GetMesh() const
{
	return *_mesh;
}

const Material& MeshNode::GetMaterial() const
{
	return *_material;
}
