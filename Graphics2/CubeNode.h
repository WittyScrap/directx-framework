#pragma once
#include "MeshNode.h"

/**
 * Represents a cube mesh.
 *
 */
class CubeNode : public MeshNode
{
public:
    /**     Constructors     */
    CubeNode()              : MeshNode(L"Cube Node")              {}
    CubeNode(wstring name)  : MeshNode(name)                      {}

    virtual bool            Initialise()                   override;
    virtual void            Update()                       override;

            void            SetRotationSpeed(const float& speed);
	        const float&    GetRotationSpeed() const;

	static  vector<Vertex>  GetVertices();
	static  vector<UINT>    GetIndices();

private:
    float           _currentRotation{ 0 };
    float           _rotationSpeed{ 1 };
};

