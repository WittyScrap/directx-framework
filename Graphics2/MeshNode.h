#pragma once
#include "SceneNode.h"
#include "DirectXCore.h"
#include "Material.h"
#include "Mesh.h"
#include <vector>

/**
 * Represents a node that can retain any type
 * of geometry.
 *
 */
class MeshNode : public SceneNode
{
public:
    /* --- Constructors --- */
                 MeshNode()                     : SceneNode(L"Mesh Node") {}
                 MeshNode(const wstring& name)  : SceneNode(name)         {}

    virtual      ~MeshNode()                                              {}

    virtual void Update(FXMMATRIX& currentWorldTransformation)  override;
    virtual void Render()                                       override;
    virtual void Shutdown()                                     override;

            void SetMaterial(const shared_ptr<Material>& material);
            void SetMesh(const shared_ptr<Mesh>& mesh);

            const shared_ptr<Mesh>      GetMesh() const;
            const shared_ptr<Material>  GetMaterial() const;

protected:

    static  ComPtr<ID3D11Device>                GetDevice();
    static  ComPtr<ID3D11DeviceContext>         GetDeviceContext();

private:
            shared_ptr<Mesh>                    _mesh = nullptr;
            shared_ptr<Material>                _material = nullptr;
};

