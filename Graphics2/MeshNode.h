#pragma once
#include "Core.h"
#include "Mesh.h"
#include "Material.h"
#include "SceneNode.h"

/**
 * Represents a mesh that can be transformed and drawn.
 */
class MeshNode : public SceneNode
{

public:
	MeshNode() : _mesh(std::make_shared<Mesh>()), _material(std::make_shared<Material>())
	{}

	virtual bool Initialise() = 0;	
	virtual void Render();
	virtual void Shutdown();

	void Reset();

	const Mesh& GetMesh() const;
	const Material& GetMaterial() const;

protected:
	MeshPtr _mesh;
	MaterialPtr _material;
};

